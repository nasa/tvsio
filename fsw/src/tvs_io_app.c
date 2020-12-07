/*=======================================================================================
** File Name:  tvs_io_app.c
**
** Title:  Function Definitions for TVS_IO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  This source file contains all necessary function definitions to run TVS_IO
**           application.
**
** Functions Defined:
**    Function X - Brief purpose of function X
**    Function Y - Brief purpose of function Y
**    Function Z - Brief purpose of function Z
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to all functions in the file.
**    2. List the external source(s) and event(s) that can cause the funcs in this
**       file to execute.
**    3. List known limitations that apply to the funcs in this file.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/

/*
** Pragmas
*/

/*
** Include Files
*/
#include <string.h>
#include <pthread.h>

#include "cfe.h"

#include "tvs_io_platform_cfg.h"
#include "tvs_io_mission_cfg.h"
#include "tvs_io_app.h"

#include "cfe_platform_cfg.h"

#include "stdio.h"
/*
** Local Defines
*/

/*
** Local Structure Declarations
*/

/*
** External Global Variables
*/

/*
** Global Variables
*/
TVS_IO_AppData_t  g_TVS_IO_AppData;

/*
** Local Variables
*/

/*
** Local Function Definitions
*/

int32 InitConnectionInfo()
{
    g_TVS_IO_AppData.servers = (TVS_IO_TrickServer_t *)malloc( sizeof(TVS_IO_TrickServer_t) * TVS_NUM_SIM_CONN );
    memset(&g_TVS_IO_AppData.servers[0], 0, sizeof(TVS_IO_TrickServer_t) * TVS_NUM_SIM_CONN);

    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        g_TVS_IO_AppData.servers[conn].serv_addr.sin_family = AF_INET;
        g_TVS_IO_AppData.servers[conn].serv_addr.sin_port = htons(TVS_SERVER_PORTS[conn]);

        if (inet_pton(AF_INET, TVS_SERVER_IPS[conn], &g_TVS_IO_AppData.servers[conn].serv_addr.sin_addr) <= 0)
        {
            OS_printf("\ninet_pton error occured initializing connection %d - %s:%d!\n", conn, TVS_SERVER_IPS[conn], TVS_SERVER_PORTS[conn]);
            return -1;
        }
    }
    return 1;
}

//TODO should probably find a way to not continuously open sockets in the case of multiple sim connections with one connection down
int32 ConnectToTrickVariableServer()
{
    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        OS_printf("TVS_IO: Attempting to connect to TVS connection %d - %s:%d\n", conn, TVS_SERVER_IPS[conn], TVS_SERVER_PORTS[conn]);

        if ((g_TVS_IO_AppData.servers[conn].socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            OS_printf("TVS_IO: Error creating TVS connection %d - %s:%d!\n", conn, TVS_SERVER_IPS[conn], TVS_SERVER_PORTS[conn]);
            return -1;
        }

        if (connect(g_TVS_IO_AppData.servers[conn].socket, (struct sockaddr *)&g_TVS_IO_AppData.servers[conn].serv_addr, sizeof(struct sockaddr_in)) < 0)
        {
            OS_printf("TVS_IO: Error: Connect to TVS %d - %s:%d Failed with error: %s\n", conn, TVS_SERVER_IPS[conn], TVS_SERVER_PORTS[conn], strerror(errno));
            return -1;
        }

        OS_printf("TVS_IO: Connection to TVS %d - %s:%d successful!\n", conn, TVS_SERVER_IPS[conn], TVS_SERVER_PORTS[conn]);
    }
    return 1;
}

int32 SendInitMessages()
{
    TVS_IO_Mapping *mappings = g_TVS_IO_AppData.mappings;

    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        SendTvsMessage(conn, TVS_PAUSE_CMD);
        SendTvsMessage(conn, TVS_SET_BINARY_NO_NAMES);
        SendTvsMessage(conn, TVS_SET_COPY_MODE_CMD);
        SendTvsMessage(conn, TVS_SET_WRITE_MODE_CMD);
    }

    // send out application-specific init messages...
    for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
    {
        if (mappings[i].flowDirection & TrickToCfs)
        {
            char **initMessages = mappings[i].initMessages;

            for (int j = 0; j < mappings[i].memberCount; ++j)
            {
                SendTvsMessage(mappings[i].connectionIndex, initMessages[j]);
            }
        }
    }

    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        SendTvsMessage(conn, TVS_UNPAUSE_CMD);
    }

    return 1;
}

int32 TryReadMessage()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);

    uint32 vars_received; 
    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn) {
        // Because it is possible for trick to send multiple messages for large buffers, 
        // we loop and count the number of vars received for each connection to make sure we get it all.
        vars_received = 0; 
        g_TVS_IO_AppData.frameDataBuffers[conn].frameBufferLength = 0;
        while (vars_received < TVS_IO_TOTAL_VARS_CONN[conn])
        {
            int headerLength = 0;
            char buffer[8192]; // Max message size trick will send

            // Read the 12 byte header from the socket
            while (headerLength < 12)
            {
                int bytesRead = read(g_TVS_IO_AppData.servers[conn].socket, buffer + headerLength, 12 - headerLength);
                if (bytesRead <= 0)
                {
                    close(g_TVS_IO_AppData.servers[conn].socket);
                    return -1;
                }
                else
                {
                    headerLength += bytesRead;
                }
            }

            int message_indicator = -1, message_size = -1, n_vars = -1;

            memcpy(&message_indicator, &buffer[0], 4);
            memcpy(&message_size, &buffer[4], 4); //NOTE message size does NOT include the 4 byte message_indicator
            memcpy(&n_vars, &buffer[8], 4);
        
            vars_received += n_vars;

            message_size -= 8; // chop off the header bytes from msg size

            int payloadBytesRead = 0;

            // read payload into buffer
            while (payloadBytesRead < message_size)
            {
                int bytesRead = read(g_TVS_IO_AppData.servers[conn].socket, buffer + payloadBytesRead, message_size - payloadBytesRead);

                if (bytesRead <= 0)
                {
                    close(g_TVS_IO_AppData.servers[conn].socket);
                    //TODO add a warning message here
                    return -1;
                }
                else
                {
                    payloadBytesRead += bytesRead;
                }
            }

            // copy data from read buffer into frame data buffer
            memcpy(g_TVS_IO_AppData.frameDataBuffers[conn].frameBuffer + g_TVS_IO_AppData.frameDataBuffers[conn].frameBufferLength,
                    buffer, payloadBytesRead);

            g_TVS_IO_AppData.frameDataBuffers[conn].frameBufferLength += payloadBytesRead;
        }
    }

    // process the frame data one mapping at a time
    TVS_IO_Mapping *mappings = g_TVS_IO_AppData.mappings;

    //uint32_t byteOffsets[TVS_NUM_SIM_CONN] = { 0 };
    uint32_t byteOffsets[TVS_NUM_SIM_CONN];
    for(int i =0; i <TVS_NUM_SIM_CONN; ++i) {
        byteOffsets[i] = 0;
    }
    uint8_t connIdx = 0; //shorthand

    for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
    {
        if (mappings[i].flowDirection & TrickToCfs)
        {
            connIdx = mappings[i].connectionIndex;
            void *unpackedDataBuffer = mappings[i].unpackedDataBuffer;

            byteOffsets[connIdx] += mappings[i].unpack(unpackedDataBuffer, g_TVS_IO_AppData.frameDataBuffers[connIdx].frameBuffer + byteOffsets[connIdx]);

            CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) mappings[i].unpackedDataBuffer);
            OS_printf("***** TVSIO ***** func: %s line: %d send Msg \n", __func__, __LINE__);
            CFE_SB_SendMsg((CFE_SB_Msg_t*)mappings[i].unpackedDataBuffer);
        }
    }

    return 1;
}


int32 SendTvsCommand(char *commandString)
{
    // TODO: error checking... broken connections, etc.
    write(g_TVS_IO_AppData.servers[0].socket, commandString, strlen(commandString));
    return 1;
}

int32 SendTvsMessage(int conn, char *commandString)
{
    // TODO: error checking... broken connections, etc.
    write(g_TVS_IO_AppData.servers[conn].socket, commandString, strlen(commandString));
    return 1;
}

void ReceiveTaskRun()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    while(1)
    {
        int32 success = TryReadMessage();

        if (success < 0)
        {
            while (ConnectToTrickVariableServer() < 0)
            {
                OS_TaskDelay(3000); // wait a few secs and try again...
            }

            SendInitMessages();
        }
    }
}

/*=====================================================================================
** Name: TVS_IO_InitEvent
**
** Purpose: To initialize and register event table for TVS_IO application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_EVS_Register
**    CFE_ES_WriteToSysLog
**
** Called By:
**    TVS_IO_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_TVS_IO_AppData.EventTbl
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TVS_IO_InitEvent()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    int32  iStatus=CFE_SUCCESS;

    /* Create the event table */
    memset((void*)g_TVS_IO_AppData.EventTbl, 0x00, sizeof(g_TVS_IO_AppData.EventTbl));

    g_TVS_IO_AppData.EventTbl[0].EventID = TVS_IO_RESERVED_EID;
    g_TVS_IO_AppData.EventTbl[1].EventID = TVS_IO_INF_EID;
    g_TVS_IO_AppData.EventTbl[2].EventID = TVS_IO_INIT_INF_EID;
    g_TVS_IO_AppData.EventTbl[3].EventID = TVS_IO_ILOAD_INF_EID;
    g_TVS_IO_AppData.EventTbl[4].EventID = TVS_IO_CDS_INF_EID;
    g_TVS_IO_AppData.EventTbl[5].EventID = TVS_IO_CMD_INF_EID;

    g_TVS_IO_AppData.EventTbl[ 6].EventID = TVS_IO_ERR_EID;
    g_TVS_IO_AppData.EventTbl[ 7].EventID = TVS_IO_INIT_ERR_EID;
    g_TVS_IO_AppData.EventTbl[ 8].EventID = TVS_IO_ILOAD_ERR_EID;
    g_TVS_IO_AppData.EventTbl[ 9].EventID = TVS_IO_CDS_ERR_EID;
    g_TVS_IO_AppData.EventTbl[10].EventID = TVS_IO_CMD_ERR_EID;
    g_TVS_IO_AppData.EventTbl[11].EventID = TVS_IO_PIPE_ERR_EID;
    g_TVS_IO_AppData.EventTbl[12].EventID = TVS_IO_MSGID_ERR_EID;
    g_TVS_IO_AppData.EventTbl[13].EventID = TVS_IO_MSGLEN_ERR_EID;

    /* Register the table with CFE */
    iStatus = CFE_EVS_Register(g_TVS_IO_AppData.EventTbl,
                               TVS_IO_EVT_CNT, CFE_EVS_BINARY_FILTER);
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TVS_IO - Failed to register with EVS (0x%08X)\n", iStatus);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: TVS_IO_InitPipe
**
** Purpose: To initialize all message pipes and subscribe to messages for TVS_IO application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_CreatePipe
**    CFE_SB_Subscribe
**    CFE_ES_WriteToSysLog
**
** Called By:
**    TVS_IO_InitApp
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_TVS_IO_AppData.usSchPipeDepth
**    g_TVS_IO_AppData.cSchPipeName
**    g_TVS_IO_AppData.SchPipeId
**    g_TVS_IO_AppData.usCmdPipeDepth
**    g_TVS_IO_AppData.cCmdPipeName
**    g_TVS_IO_AppData.CmdPipeId
**    g_TVS_IO_AppData.usTlmPipeDepth
**    g_TVS_IO_AppData.cTlmPipeName
**    g_TVS_IO_AppData.TlmPipeId
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TVS_IO_InitPipe()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    int32  iStatus=CFE_SUCCESS;

    /* Init schedule pipe */
    g_TVS_IO_AppData.usSchPipeDepth = TVS_IO_SCH_PIPE_DEPTH;
    memset((void*)g_TVS_IO_AppData.cSchPipeName, '\0', sizeof(g_TVS_IO_AppData.cSchPipeName));
    strncpy(g_TVS_IO_AppData.cSchPipeName, "TVS_IO_SCH_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to Wakeup messages */
    iStatus = CFE_SB_CreatePipe(&g_TVS_IO_AppData.SchPipeId,
                                 g_TVS_IO_AppData.usSchPipeDepth,
                                 g_TVS_IO_AppData.cSchPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_SB_SubscribeEx(TVS_IO_WAKEUP_MID, g_TVS_IO_AppData.SchPipeId, CFE_SB_Default_Qos, 1);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("TVS_IO - Sch Pipe failed to subscribe to TVS_IO_WAKEUP_MID. (0x%08X)\n", iStatus);
            goto TVS_IO_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("TVS_IO - Failed to create SCH pipe (0x%08X)\n", iStatus);
        goto TVS_IO_InitPipe_Exit_Tag;
    }

    /* Init command pipe */
    g_TVS_IO_AppData.usCmdPipeDepth = TVS_IO_CMD_PIPE_DEPTH ;
    memset((void*)g_TVS_IO_AppData.cCmdPipeName, '\0', sizeof(g_TVS_IO_AppData.cCmdPipeName));
    strncpy(g_TVS_IO_AppData.cCmdPipeName, "TVS_IO_CMD_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to command messages */
    iStatus = CFE_SB_CreatePipe(&g_TVS_IO_AppData.CmdPipeId,
                                 g_TVS_IO_AppData.usCmdPipeDepth,
                                 g_TVS_IO_AppData.cCmdPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Subscribe to command messages */
        iStatus = CFE_SB_Subscribe(TVS_IO_CMD_MID, g_TVS_IO_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("TVS_IO - CMD Pipe failed to subscribe to TVS_IO_CMD_MID. (0x%08X)\n", iStatus);
            goto TVS_IO_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(TVS_IO_SEND_HK_MID, g_TVS_IO_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("TVS_IO - CMD Pipe failed to subscribe to TVS_IO_SEND_HK_MID. (0x%08X)\n", iStatus);
            goto TVS_IO_InitPipe_Exit_Tag;
        }
        
    }
    else
    {
        CFE_ES_WriteToSysLog("TVS_IO - Failed to create CMD pipe (0x%08X)\n", iStatus);
        goto TVS_IO_InitPipe_Exit_Tag;
    }

    /* Init telemetry pipe */
    g_TVS_IO_AppData.usTlmPipeDepth = TVS_IO_TLM_PIPE_DEPTH;
    memset((void*)g_TVS_IO_AppData.cTlmPipeName, '\0', sizeof(g_TVS_IO_AppData.cTlmPipeName));
    strncpy(g_TVS_IO_AppData.cTlmPipeName, "TVS_IO_TLM_PIPE", OS_MAX_API_NAME-1);

    /* Subscribe to telemetry messages on the telemetry pipe */
    iStatus = CFE_SB_CreatePipe(&g_TVS_IO_AppData.TlmPipeId,
                                 g_TVS_IO_AppData.usTlmPipeDepth,
                                 g_TVS_IO_AppData.cTlmPipeName);
    if (iStatus == CFE_SUCCESS)
    {
        /* Add CFE_SB_Subscribe() calls for other apps' output data here.
        **
        ** Examples:
        **     CFE_SB_Subscribe(GNCEXEC_OUT_DATA_MID, g_TVS_IO_AppData.TlmPipeId);
        */
    }
    else
    {
        CFE_ES_WriteToSysLog("TVS_IO - Failed to create TLM pipe (0x%08X)\n", iStatus);
        goto TVS_IO_InitPipe_Exit_Tag;
    }

    g_TVS_IO_AppData.trickPipeDepth = TVS_IO_TRICK_PIPE_DEPTH;
    memset((void*)g_TVS_IO_AppData.trickPipeName, '\0', sizeof(g_TVS_IO_AppData.trickPipeName));
    strncpy(g_TVS_IO_AppData.trickPipeName, "TVS_IO_TRICK_PIPE", OS_MAX_API_NAME-1);

    TVS_IO_Mapping *mappings = g_TVS_IO_AppData.mappings;

    iStatus = CFE_SB_CreatePipe(&g_TVS_IO_AppData.trickPipeId,
                                g_TVS_IO_AppData.trickPipeDepth,
                                g_TVS_IO_AppData.trickPipeName);

    if (iStatus == CFE_SUCCESS)
    {
        for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
        {
            if (mappings[i].flowDirection & CfsToTrick)
            {
                iStatus = CFE_SB_Subscribe(mappings[i].msgId, g_TVS_IO_AppData.trickPipeId);

                if (iStatus != CFE_SUCCESS)
                {
                    CFE_ES_WriteToSysLog("TVS_IO - Trick msg pipe failed to subscribe to mid: (0x%08X)\n", mappings[i].msgId);
                }
            }
        }
    }

TVS_IO_InitPipe_Exit_Tag:
    return (iStatus);
}
    
/*=====================================================================================
** Name: TVS_IO_InitData
**
** Purpose: To initialize global variables used by TVS_IO application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_SB_InitMsg
**
** Called By:
**    TVS_IO_InitApp
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    g_TVS_IO_AppData.InData
**    g_TVS_IO_AppData.OutData
**    g_TVS_IO_AppData.HkTlm
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TVS_IO_InitData()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_TVS_IO_AppData.InData, 0x00, sizeof(g_TVS_IO_AppData.InData));

    /* Init output data */
    memset((void*)&g_TVS_IO_AppData.OutData, 0x00, sizeof(g_TVS_IO_AppData.OutData));
    CFE_SB_InitMsg(&g_TVS_IO_AppData.OutData,
                   TVS_IO_OUT_DATA_MID, sizeof(g_TVS_IO_AppData.OutData), TRUE);

    /* Init housekeeping packet */
    memset((void*)&g_TVS_IO_AppData.HkTlm, 0x00, sizeof(g_TVS_IO_AppData.HkTlm));
    CFE_SB_InitMsg(&g_TVS_IO_AppData.HkTlm,
                   TVS_IO_HK_TLM_MID, sizeof(g_TVS_IO_AppData.HkTlm), TRUE);

    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn) {
        g_TVS_IO_AppData.frameDataBuffers[conn].frameBuffer = (char*)malloc(TVS_IO_FRAME_DATA_BUFFER_SIZE);
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: TVS_IO_InitApp
**
** Purpose: To initialize all data local to and used by TVS_IO application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_WriteToSysLog
**    CFE_EVS_SendEvent
**    OS_TaskInstallDeleteHandler
**    TVS_IO_InitEvent
**    TVS_IO_InitPipe
**    TVS_IO_InitData
**
** Called By:
**    TVS_IO_AppMain
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TVS_IO_InitApp()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    int32  iStatus=CFE_SUCCESS;

    g_TVS_IO_AppData.uiRunStatus = CFE_ES_APP_RUN;

    iStatus = CFE_ES_RegisterApp();
    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TVS_IO - Failed to register the app (0x%08X)\n", iStatus);
        goto TVS_IO_InitApp_Exit_Tag;
    }

    TVS_IO_InitGeneratedCode(g_TVS_IO_AppData.mappings);

    if ((TVS_IO_InitEvent() != CFE_SUCCESS) || 
        (TVS_IO_InitPipe() != CFE_SUCCESS) || 
        (TVS_IO_InitData() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto TVS_IO_InitApp_Exit_Tag;
    }

    InitConnectionInfo();

    if (ConnectToTrickVariableServer() > 0)
    {
        SendInitMessages();
    }

    iStatus = CFE_ES_CreateChildTask(&g_TVS_IO_AppData.receiveTaskId,
                                        "TVS_IO_RcvTask",
                                        (CFE_ES_ChildTaskMainFuncPtr_t)&ReceiveTaskRun,
                                        NULL,
                                        TVS_IO_RCV_THREAD_STACK_SIZE,
                                        0, 0);

    if (iStatus != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("TVS_IO - Failed to create child task for processing incoming TVS data...\n");
    }

    /* Install the cleanup callback */
    OS_TaskInstallDeleteHandler((void*)&TVS_IO_CleanupCallback);

TVS_IO_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(TVS_IO_INIT_INF_EID, CFE_EVS_INFORMATION,
                          "TVS_IO - Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("TVS_IO - Application failed to initialize\n");
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: TVS_IO_CleanupCallback
**
** Purpose: To handle any neccesary cleanup prior to application exit
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TVS_IO_CleanupCallback()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    /* Add code to cleanup memory and other cleanup here */
}
    
/*=====================================================================================
** Name: TVS_IO_RcvMsg
**
** Purpose: To receive and process messages for TVS_IO application
**
** Arguments:
**    None
**
** Returns:
**    int32 iStatus - Status of initialization 
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    TVS_IO_ProcessNewCmds
**    TVS_IO_ProcessNewData
**    TVS_IO_SendOutData
**
** Called By:
**    TVS_IO_Main
**
** Global Inputs/Reads:
**    g_TVS_IO_AppData.SchPipeId
**
** Global Outputs/Writes:
**    g_TVS_IO_AppData.uiRunStatus
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
int32 TVS_IO_RcvMsg(int32 iBlocking)
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Msg_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(TVS_IO_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_RcvMsg(&MsgPtr, g_TVS_IO_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TVS_IO_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        MsgId = CFE_SB_GetMsgId(MsgPtr);
        switch (MsgId)
        {
            case TVS_IO_WAKEUP_MID:
                TVS_IO_ProcessNewCmds();
                TVS_IO_ProcessNewData();

                /* The last thing to do at the end of this Wakeup cycle should be to
                   automatically publish new output. */
                TVS_IO_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "TVS_IO - Recvd invalid SCH msgId (0x%08X)", MsgId);
                TVS_IO_ProcessNewData();
        }
    }
    else if (iStatus == CFE_SB_NO_MESSAGE)
    {
        /* If there's no incoming message, you can do something here, or nothing */
    }
    else
    {
        /* This is an example of exiting on an error.
        ** Note that a SB read error is not always going to result in an app quitting.
        */
        CFE_EVS_SendEvent(TVS_IO_PIPE_ERR_EID, CFE_EVS_ERROR,
			  "TVS_IO: SB pipe read error (0x%08X), app will exit", iStatus);
        g_TVS_IO_AppData.uiRunStatus= CFE_ES_APP_ERROR;
    }

    return (iStatus);
}
    
/*=====================================================================================
** Name: TVS_IO_ProcessNewData
**
** Purpose: To process incoming data subscribed by TVS_IO application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**
** Called By:
**    TVS_IO_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TVS_IO_ProcessNewData()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t  TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&TlmMsgPtr, g_TVS_IO_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            TlmMsgId = CFE_SB_GetMsgId(TlmMsgPtr);
            switch (TlmMsgId)
            {
                /* 
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         TVS_IO_ProcessNavData(TlmMsgPtr);
                **         break;
                */
                default:
                    CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "TVS_IO - Recvd invalid TLM msgId (0x%08X)", TlmMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(TVS_IO_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "TVS_IO: CMD pipe read error (0x%08X)", iStatus);
            g_TVS_IO_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: TVS_IO_ProcessNewCmds
**
** Purpose: To process incoming command messages for TVS_IO application
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_RcvMsg
**    CFE_SB_GetMsgId
**    CFE_EVS_SendEvent
**    TVS_IO_ProcessNewAppCmds
**    TVS_IO_ReportHousekeeping
**
** Called By:
**    TVS_IO_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    None
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TVS_IO_ProcessNewCmds()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    int iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t  CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_RcvMsg(&CmdMsgPtr, g_TVS_IO_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            CmdMsgId = CFE_SB_GetMsgId(CmdMsgPtr);
            switch (CmdMsgId)
            {
                case TVS_IO_CMD_MID:
                    TVS_IO_ProcessNewAppCmds(CmdMsgPtr);
                    break;

                case TVS_IO_SEND_HK_MID:
                    TVS_IO_ReportHousekeeping();
                    break;

                /* 
                **
                ** Example:
                **     case CFE_TIME_DATA_CMD_MID:
                **         TVS_IO_ProcessTimeDataCmd(CmdMsgPtr);
                **         break;
                */

                default:
                    CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_ERROR,
                                      "TVS_IO - Recvd invalid CMD msgId (0x%08X)", CmdMsgId);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(TVS_IO_PIPE_ERR_EID, CFE_EVS_ERROR,
                  "TVS_IO: CMD pipe read error (0x%08X)", iStatus);
            g_TVS_IO_AppData.uiRunStatus = CFE_ES_APP_ERROR;
            break;
        }
    }
}
    
/*=====================================================================================
** Name: TVS_IO_ProcessNewAppCmds
**
** Purpose: To process command messages targeting TVS_IO application
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr - new command message pointer
**
** Returns:
**    None
**
** Routines Called:
**    CFE_SB_GetCmdCode
**    CFE_EVS_SendEvent
**
** Called By:
**    TVS_IO_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    g_TVS_IO_AppData.HkTlm.usCmdCnt
**    g_TVS_IO_AppData.HkTlm.usCmdErrCnt
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TVS_IO_ProcessNewAppCmds(CFE_SB_Msg_t* MsgPtr)
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    uint32  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        uiCmdCode = CFE_SB_GetCmdCode(MsgPtr);
        switch (uiCmdCode)
        {
            case TVS_IO_NOOP_CC:
                g_TVS_IO_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(TVS_IO_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "TVS_IO - Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case TVS_IO_RESET_CC:
                g_TVS_IO_AppData.HkTlm.usCmdCnt = 0;
                g_TVS_IO_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(TVS_IO_CMD_INF_EID, CFE_EVS_INFORMATION,
                                  "TVS_IO - Recvd RESET cmd (%d)", uiCmdCode);
                break;

            

            default:
                g_TVS_IO_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_ERROR,
                                  "TVS_IO - Recvd invalid cmdId (%d)", uiCmdCode);
                break;
        }
    }
}
    
/*=====================================================================================
** Name: TVS_IO_ReportHousekeeping
**
** Purpose: To send housekeeping message
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TVS_IO_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  GSFC, Nexsys
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TVS_IO_ReportHousekeeping()
{
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TVS_IO_AppData.HkTlm);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TVS_IO_AppData.HkTlm);
}
    
/*=====================================================================================
** Name: TVS_IO_SendOutData
**
** Purpose: To publish 1-Wakeup cycle output data
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    TBD
**
** Called By:
**    TVS_IO_RcvMsg
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TVS_IO_SendOutData()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    CFE_SB_TimeStampMsg((CFE_SB_Msg_t*)&g_TVS_IO_AppData.OutData);
    CFE_SB_SendMsg((CFE_SB_Msg_t*)&g_TVS_IO_AppData.OutData);
}
    
/*=====================================================================================
** Name: TVS_IO_VerifyCmdLength
**
** Purpose: To verify command length for a particular command message
**
** Arguments:
**    CFE_SB_Msg_t*  MsgPtr      - command message pointer
**    uint16         usExpLength - expected command length
**
** Returns:
**    boolean bResult - result of verification
**
** Routines Called:
**    TBD
**
** Called By:
**    TVS_IO_ProcessNewCmds
**
** Global Inputs/Reads:
**    None
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
boolean TVS_IO_VerifyCmdLength(CFE_SB_Msg_t* MsgPtr,
                           uint16 usExpectedLen)
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    boolean bResult=FALSE;
    uint16  usMsgLen=0;

    if (MsgPtr != NULL)
    {
        usMsgLen = CFE_SB_GetTotalMsgLength(MsgPtr);

        if (usExpectedLen != usMsgLen)
        {
            CFE_SB_MsgId_t MsgId = CFE_SB_GetMsgId(MsgPtr);
            uint16 usCmdCode = CFE_SB_GetCmdCode(MsgPtr);

            CFE_EVS_SendEvent(TVS_IO_MSGLEN_ERR_EID, CFE_EVS_ERROR,
                              "TVS_IO - Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%d, expectedLen=%d",
                              MsgId, usCmdCode, usMsgLen, usExpectedLen);
            g_TVS_IO_AppData.HkTlm.usCmdErrCnt++;
        }
    }

    return (bResult);
}

/*=====================================================================================
** Name: TVS_IO_AppMain
**
** Purpose: To define TVS_IO application's entry point and main process loop
**
** Arguments:
**    None
**
** Returns:
**    None
**
** Routines Called:
**    CFE_ES_RegisterApp
**    CFE_ES_RunLoop
**    CFE_ES_PerfLogEntry
**    CFE_ES_PerfLogExit
**    CFE_ES_ExitApp
**    CFE_ES_WaitForStartupSync
**    TVS_IO_InitApp
**    TVS_IO_RcvMsg
**
** Called By:
**    TBD
**
** Global Inputs/Reads:
**    TBD
**
** Global Outputs/Writes:
**    TBD
**
** Limitations, Assumptions, External Events, and Notes:
**    1. List assumptions that are made that apply to this function.
**    2. List the external source(s) and event(s) that can cause this function to execute.
**    3. List known limitations that apply to this function.
**    4. If there are no assumptions, external events, or notes then enter NONE.
**       Do not omit the section.
**
** Algorithm:
**    Psuedo-code or description of basic algorithm
**
** Author(s):  Nexsys 
**
** History:  Date Written  2018-03-08
**           Unit Tested   yyyy-mm-dd
**=====================================================================================*/
void TVS_IO_AppMain()
{
    OS_printf("***** TVSIO ***** func: %s line: %d\n", __func__, __LINE__);
    /* Register the application with Executive Services */
    CFE_ES_RegisterApp();

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TVS_IO_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    if (TVS_IO_InitApp() != CFE_SUCCESS)
    {
        g_TVS_IO_AppData.uiRunStatus = CFE_ES_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(TVS_IO_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(TVS_IO_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(TVS_IO_MAIN_TASK_PERF_ID);
    }

    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Msg_t *trickCmdMsgPtr = NULL;

    TVS_IO_Mapping *mappings = g_TVS_IO_AppData.mappings;

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_TVS_IO_AppData.uiRunStatus) == TRUE)
    {
        OS_TaskDelay(100);

        while(1)
        {
            iStatus = CFE_SB_RcvMsg(&trickCmdMsgPtr, g_TVS_IO_AppData.trickPipeId, CFE_SB_POLL);

            if (iStatus == CFE_SUCCESS)
            {
                uint32 mid = CFE_SB_GetMsgId(trickCmdMsgPtr);
                uint16 cmdCode = CFE_SB_GetCmdCode(trickCmdMsgPtr);
                OS_printf("***** TVSIO ***** func: %s line: %d, mid %d, cmdCode %d\n", __func__, __LINE__,mid,cmdCode);

                for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
                {
                    if ((mappings[i].msgId == mid) && (mappings[i].flowDirection & CfsToTrick))
                    {
                        if (mappings[i].commandCode != cmdCode)
                        {
                            continue;
                        }

                        char **cmdBuffer = mappings[i].packedCommandBuffer;

                        mappings[i].pack(cmdBuffer, trickCmdMsgPtr);

                        for (int j = 0; j < mappings[i].memberCount; ++j) {
                            SendTvsMessage(mappings[i].connectionIndex, cmdBuffer[j]);
                        }
                        break;
                    }
                }
            }
            else if (iStatus == CFE_SB_NO_MESSAGE)
            {
                break;
            }
            else
            {
                OS_printf("TVS_IO: ERROR polling local sb pipe for trick cmd msgs...\n");
                break;
            }
        }
    }

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(TVS_IO_MAIN_TASK_PERF_ID);

    /* Exit the application */
    CFE_ES_ExitApp(g_TVS_IO_AppData.uiRunStatus);
} 
    
/*=======================================================================================
** End of file tvs_io_app.c
**=====================================================================================*/
    
