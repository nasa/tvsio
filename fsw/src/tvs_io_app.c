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
**   2020-12-XX | Nexsys | Multi-sim support capability added
**
**=====================================================================================*/
//GLOBAL TODO: Clean up comments/code docs -JWP
/*
** Pragmas
*/

/*
** Include Files
*/
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <math.h>

#include "cfe.h"

#include "tvs_io_platform_cfg.h"
#include "tvs_io_app.h"
#include "tvs_io_utils.h"

#include "cfe_platform_cfg.h"

#include "stdio.h"
/*
** Local Defines
*/
#define SEND_ERR_EXIT(connIdx, CMD) \
    if(SendTvsMessage(connIdx, CMD) < 0) \
    { \
        return; \
    }

/* Trick variable server message indicator values (see Trick docs - Variable-Server#ascii-format) */
#define VS_IP_ERROR      (-1) /* Protocol Error */
#define VS_VAR_LIST        0  /* A list of variable values */
#define VS_VAR_EXISTS      1  /* response to var_exists() */
#define VS_SIE_RESOURCE    2  /* response to send_sie_resource */
#define VS_LIST_SIZE       3  /* response to var_send_list_size/send_event_data */
#define VS_STDIO           4  /* values redirected from stdio */
#define VS_SEND_ONCE       5  /* response to var_send_once() */

/*
** Local Structure Declarations
*/
static const long long timeTicValue = 1000000;

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

/* Euclid's algorithm. GreatestCommonDivisor(0, b) == b, so it can be used as the
   running accumulator's seed value when folding over a list of rates. */
static long long GreatestCommonDivisor(long long a, long long b)
{
    while (b != 0)
    {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

/* Common handling for a TVS connection that has dropped: reports the event (using the
   caller's own event ID and function name so the log still points at the call site),
   closes and invalidates the socket, and schedules when to next print a connection
   message and attempt a reconnect. */
static void HandleServerDisconnect(int32 connIdx, uint16 eventId, const char *callerFuncName)
{
    if(eventId != (uint16)(-1)) 
    {
        CFE_EVS_SendEvent(eventId, CFE_EVS_EventType_ERROR, "%s: Error - TVS connection %d appears to have disconnected", callerFuncName, connIdx);
    }
    close(g_TVS_IO_AppData.servers[connIdx].socket);
    g_TVS_IO_AppData.servers[connIdx].socket = -1;
    g_TVS_IO_AppData.servers[connIdx].nextConnPrintTimeInSecs = g_TVS_IO_AppData.currVirtualTimeInTics + 60;
    g_TVS_IO_AppData.servers[connIdx].nextConnRetryInSecs = g_TVS_IO_AppData.currVirtualTimeInTics + 5;
}

/* Reads and discards numBytes from a TVS connection's socket, in chunks, used to skip
   over the body of a message type that the caller isn't interested in processing.
   Returns 0 on success. On a read error/disconnect, handles the disconnect itself and
   returns -1. */
static int32 DiscardBytesFromSocket(int32 connIdx, int numBytes)
{
    int bytesDiscarded = 0;
    while (bytesDiscarded < numBytes)
    {
        int toRead = numBytes - bytesDiscarded;
        if (toRead > (int)sizeof(g_TVS_IO_AppData.servers[connIdx].buffer))
        {
            toRead = (int)sizeof(g_TVS_IO_AppData.servers[connIdx].buffer);
        }

        int bytesRead = read(g_TVS_IO_AppData.servers[connIdx].socket, g_TVS_IO_AppData.servers[connIdx].buffer, toRead);
        if (bytesRead <= 0)
        {
            HandleServerDisconnect(connIdx, __LINE__, __func__);
            return -1;
        }
        bytesDiscarded += bytesRead;
    }
    return 0;
}

/* Reads numBytes from a TVS connection's socket into buffer, issuing repeated read()
   calls as needed until the full amount has been read. Unlike DiscardBytesFromSocket,
   this does not call HandleServerDisconnect itself on failure - it just returns -1 so
   the caller can report/handle the disconnect with its own event ID and context.
   Returns 0 on success, -1 on a read error or disconnect (read() returning <= 0). */
static int32 ReadExactlyNBytes(int32 connIdx, char * buffer, int32 numBytes)
{
    int totalBytesRead = 0;
    while (totalBytesRead < numBytes)
    {
        int toRead = numBytes - totalBytesRead;

        int bytesRead = read(g_TVS_IO_AppData.servers[connIdx].socket, buffer+totalBytesRead, toRead);
        if (bytesRead <= 0)
        {
            return -1;
        }
        totalBytesRead += bytesRead;
    }
    return 0;
}

/* Reads and discards the body of a single TVS variable-server message whose 4-byte
   message_indicator has already been consumed from the socket (passed in as
   message_indicator), per that type's own wire format. Used by callers that are only
   interested in one particular message type and need to skip past other message
   types already queued up on the connection. Returns the number of bytes discarded
   (>= 0) on success. On a read error/disconnect, handles the disconnect itself and
   returns -1. */
static int32 DiscardTvsMessageBody(int32 connIdx, int32_t message_indicator)
{
    switch (message_indicator)
    {
        case VS_VAR_EXISTS:
        {
            /* A single 1-byte existence flag follows the indicator; no size field. */
            const int size = 1;
            if (DiscardBytesFromSocket(connIdx, size) < 0)
            {
                return -1;
            }
            return size;
        }

        case VS_SEND_ONCE:
        case VS_VAR_LIST:
        {
            /* A 4-byte message_size follows the indicator (size includes itself),
               followed by (message_size - 4) bytes of payload. */
            int32_t remainingPktSize = 0;
            if(ReadExactlyNBytes(connIdx, (char *)&remainingPktSize, sizeof(remainingPktSize))) {
                HandleServerDisconnect(connIdx, __LINE__, __func__);
                return -1;
            }

            int32_t size = remainingPktSize - (int32_t)sizeof(remainingPktSize);
            if (DiscardBytesFromSocket(connIdx, size) < 0)
            {
                return -1;
            }
            return size;
        }

        default:
        {
            /* VS_SIE_RESOURCE, VS_LIST_SIZE, VS_STDIO are never asked for in TVS_IO. This means we got an erroneous message. 
            Assume a bad connection.*/
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_DEBUG,
                "%s: Discarded unexpected AND unhandled message type %d on connection %d. Diconnecting...", __func__,
                message_indicator, connIdx);
            HandleServerDisconnect(connIdx, -1, __func__);
            return -1;
        }
    }
}

/* Finds the soonest nextTimeToRunInTics among all connected servers. Returns 0
   if no server is currently connected. */
static long long calculateNextVirtualTimeTic(void)
{
    long long nextTimeInTics = INT64_MAX;
    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        if (g_TVS_IO_AppData.servers[conn].socket >= 0)
        {
            if (g_TVS_IO_AppData.servers[conn].nextTimeToRunInTics < nextTimeInTics)
            {
                nextTimeInTics = g_TVS_IO_AppData.servers[conn].nextTimeToRunInTics;
            }
        }
    }
    if (nextTimeInTics == INT64_MAX)
    {
        nextTimeInTics = 0;
    }
    return nextTimeInTics;
}

/* Connects (or reconnects) a single server. If the connection was just freshly
   (re)established, sends that connection's init messages. Returns true if the
   connection was already up going into this call (i.e. it's ready to be read
   from), false if it was just (re)established or the connect attempt failed. */
static bool ConnectServerByConn(int32 conn)
{
    int32 prevConnectionStatus = g_TVS_IO_AppData.servers[conn].socket;
    int32 iConnStatus = ConnectToTrickVariableServerByConn(conn);
    bool readyToRead = false;
    if (iConnStatus == TVSIO_CONN_SUCCESS)
    {
        if (g_TVS_IO_AppData.servers[conn].socket != prevConnectionStatus && g_TVS_IO_AppData.servers[conn].socket >= 0)
        {
            SendInitMessagesByConn(conn);
        } else {
            readyToRead = true;
        }
    }
    return readyToRead;
}

/* Connects/reconnects each server, sending init messages on a fresh connection
   or reading and scheduling the next read when it's due. */
static void ReadMessagesFromServers(void)
{
    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        if (ConnectServerByConn(conn))
        {
            if(g_TVS_IO_AppData.currVirtualTimeInTics >= g_TVS_IO_AppData.servers[conn].nextTimeToRunInTics)
            {
                TryReadMessageByConn(conn);
                g_TVS_IO_AppData.servers[conn].nextTimeToRunInTics = g_TVS_IO_AppData.currVirtualTimeInTics+g_TVS_IO_AppData.servers[conn].trickToCfsRateInTics;
            }
        }
    }
}

int32 InitConnectionInfo()
{
    /* Initialize trick variable server socket connections */
    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {        /* This also zeros the rest of the structure */
        g_TVS_IO_AppData.servers[conn] = (TVS_IO_TrickServer_t){ .socket = -1, .trickToCfsRateInTics = 100000 };
    }

    char envvar_name[64];
    const char *envvar_val;
    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        snprintf(envvar_name, sizeof(envvar_name), "TVS_%d_PORT", conn);
        uint16_t port = 0;
        if ((envvar_val = getenv(envvar_name)) && envvar_val[0])
        {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, "Found env variable %s, overwriting tvs_io_platform_cfg connection %d", envvar_name, conn);
            errno = 0;
            char *end;
            unsigned long tmp = strtoul(envvar_val, &end, 10);
            if (*end)
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, "Warning: ignoring trailing garbage \"%s\" in %s", end, envvar_name);
            }
            if (errno)
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, "Failed to parse %s: %s", envvar_name, strerror(errno));
            }
            else if (tmp == 0 || tmp > UINT16_MAX)
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, "%s %lu is out of range", envvar_name, tmp);
            }
            else
            {
                port = tmp;
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, "Using %s=%" PRIu16, envvar_name, port);
            }
        }
        if (!port)
        {
            port = TVS_SERVER_PORTS[conn];
        }
        g_TVS_IO_AppData.servers[conn].serv_addr.sin_family = AF_INET;
        g_TVS_IO_AppData.servers[conn].serv_addr.sin_port = htons(port);

        snprintf(envvar_name, sizeof(envvar_name), "TVS_%d_HOST", conn);
        bool have_host = false;
        if ((envvar_val = getenv(envvar_name)) && envvar_val[0])
        {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, "Found env variable %s, overwriting tvs_io_platform_cfg connection %d", envvar_name, conn);
            if (inet_pton(AF_INET, envvar_val, &g_TVS_IO_AppData.servers[conn].serv_addr.sin_addr) == 1)
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, "Using %s=%s", envvar_name, envvar_val);
                have_host = true;
            }
        }

        if (!have_host)
        {
            if (inet_pton(AF_INET, TVS_SERVER_IPS[conn], &g_TVS_IO_AppData.servers[conn].serv_addr.sin_addr) <= 0)
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, "inet_pton error occured initializing connection %d - %s:%" PRIu16 "!", conn, TVS_SERVER_IPS[conn], port);
                return -1;
            }
        }
    }
    return 1;
}

int32 ConnectToTrickVariableServerByConn(int32 connIdx)
{
    char addr_buff[INET_ADDRSTRLEN]; // buffer for message output
    uint16 port;
    OS_time_t currTime;
    int64 currTimeInSec;

    if (g_TVS_IO_AppData.servers[connIdx].socket < 0)
    {
        OS_GetLocalTime(&currTime);
        currTimeInSec = OS_TimeGetTotalSeconds(currTime);
        if(currTimeInSec < g_TVS_IO_AppData.servers[connIdx].nextConnRetryInSecs) {
            return TVSIO_CONN_FAILURE;
        }
        g_TVS_IO_AppData.servers[connIdx].nextConnRetryInSecs = currTimeInSec + 5;

        inet_ntop(AF_INET, &g_TVS_IO_AppData.servers[connIdx].serv_addr.sin_addr, addr_buff, sizeof(addr_buff));
        port = ntohs(g_TVS_IO_AppData.servers[connIdx].serv_addr.sin_port);

        if(currTimeInSec >= g_TVS_IO_AppData.servers[connIdx].nextConnPrintTimeInSecs) {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, 
                "Attempting to connect to TVS connection %d - %s:%d", connIdx, addr_buff, port);
        }

        if ((g_TVS_IO_AppData.servers[connIdx].socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            if(currTimeInSec >= g_TVS_IO_AppData.servers[connIdx].nextConnPrintTimeInSecs) {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR,
                    "Error creating socket for TVS %d - %s:%d: %s", connIdx, addr_buff, port, strerror(errno));
            }
            return TVSIO_CONN_FAILURE;
        }

        // socket is created, try to connect to trick, close the socket if trick sim doesn't connect
        int ret = connect(g_TVS_IO_AppData.servers[connIdx].socket, (struct sockaddr *)&g_TVS_IO_AppData.servers[connIdx].serv_addr, sizeof(struct sockaddr_in));
        if (ret < 0)
        {
            if(currTimeInSec >= g_TVS_IO_AppData.servers[connIdx].nextConnPrintTimeInSecs) {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, 
                    "Connect to TVS %d - %s:%d Failed with error: %s", connIdx, addr_buff, port, strerror(errno));
                g_TVS_IO_AppData.servers[connIdx].nextConnPrintTimeInSecs = currTimeInSec + 60;
            }
            close(g_TVS_IO_AppData.servers[connIdx].socket);
            g_TVS_IO_AppData.servers[connIdx].socket = -1;
            return TVSIO_CONN_FAILURE;
        } else {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_INFORMATION, 
                "Connection to TVS %d - %s:%d successful!", connIdx, addr_buff, port);
            g_TVS_IO_AppData.servers[connIdx].nextTimeToRunInTics = g_TVS_IO_AppData.currVirtualTimeInTics;
        }
        if(currTimeInSec >= g_TVS_IO_AppData.servers[connIdx].nextConnPrintTimeInSecs)
        {
            g_TVS_IO_AppData.servers[connIdx].nextConnPrintTimeInSecs = currTimeInSec + 60;
        }
    }
    
    return TVSIO_CONN_SUCCESS;
}

void CheckVariableExistence(int32 connIdx, const char *commandString) {
    if(g_TVS_IO_AppData.servers[connIdx].socket < 0)
    {
        return;
    }

    // Send the message to the server
    SEND_ERR_EXIT(connIdx, commandString);

    for(;;) {
        uint32_t indicator;
        if(ReadExactlyNBytes(connIdx, (char *)&indicator, 4))
        {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, 
                "%s: Error - Problem reading message indicator for variable '%s'", __func__, commandString);
            goto CheckVariableExistence_Err;
        }

        if(indicator != VS_VAR_EXISTS)
        {
            DiscardTvsMessageBody(connIdx, indicator);
        }

        // Read the 1-byte existence value
        uint8_t exists;
        if(ReadExactlyNBytes(connIdx, (char *)&exists, 1))
        {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, 
                "%s: Error - Problem reading existence byte for variable '%s'", __func__, commandString);
            goto CheckVariableExistence_Err;
        }

        // Check existence
        if (exists == 0) {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, 
                "%s: Error - Trick returned DOES NOT EXIST for variable check:", __func__);
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, 
                "Error Continued - %s", commandString);
            goto CheckVariableExistence_Err;
        }
        break;
    }

    return;

CheckVariableExistence_Err:
    HandleServerDisconnect(connIdx, __LINE__, __func__);
}

uint32_t TVS_UnpackRealTimeData(void *mystruct, void *buffer)
{
	TVS_IO_SimTime_t *mystructptr = (TVS_IO_SimTime_t *)mystruct;
	char *data = (char*)buffer;

	uint32 byteOffset = 0;
	int32 currentMemberLength = -1;
	uint32 currentMemberType = 0;

	currentMemberType = *((int32*) &data[byteOffset] );
	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	if (currentMemberType == 6)
	{
	   mystructptr->time_tic_value = TVS_UnpackSignedInteger( &data[byteOffset + 8], 4 );
	}
	byteOffset += 8 + currentMemberLength;

	currentMemberType = *((int32*) &data[byteOffset] );
	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	if (currentMemberType == 14)
	{
	   mystructptr->rt_frame_tics = TVS_UnpackSignedInteger( &data[byteOffset + 8], 8 );
	}
	byteOffset += 8 + currentMemberLength;

	return byteOffset;
}

void SendInitMessagesByConn(int32 connIdx)
{
    TVS_IO_Mapping *mappings = g_TVS_IO_AppData.mappings;
    TVS_IO_SimTime_t sTime = {0};

    SEND_ERR_EXIT(connIdx, TVS_GO_FAST_CMD)
    SEND_ERR_EXIT(connIdx, TVS_PAUSE_CMD)
    SEND_ERR_EXIT(connIdx, TVS_SET_BINARY_NO_NAMES)
    #if(TVS_BYTE_SWAP) // Caution! Experimental, use at your own risk!
        // trick variable server will byte swap data before sending to tvsio, enable in tvs_io_platform_cfg.h
        SEND_ERR_EXIT(coconnIdxnn, TVS_SET_BYTE_SWAP_CMD)
    #endif
    SEND_ERR_EXIT(connIdx, TVS_SET_COPY_MODE_CMD)
    SEND_ERR_EXIT(connIdx, TVS_SET_WRITE_MODE_CMD)
    // Get realtime frame settings
    SEND_ERR_EXIT(connIdx, TVS_RT_FRAME_SEND_ONCE_CMD)

    // Read the 4-byte message indicator, discarding any unexpected-but-handled
    // message types (e.g. a stray VS_VAR_EXISTS reply still working its way through
    // the socket) until the VS_SEND_ONCE response to the var_send_once request above
    // arrives.
    for (;;)
    {
        int32_t indicator;
        if(ReadExactlyNBytes(connIdx, (char *)&indicator, sizeof(indicator)))
        {
            CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR,
                "%s: Error - Problem reading message indicator var_send_once", __func__);
            goto SendInitMessagesByConn_Err;
        }

        if (indicator != VS_SEND_ONCE)
        {
            DiscardTvsMessageBody(connIdx, indicator);
            continue;
        } else 
        {
            int32_t msg_size;
            if(ReadExactlyNBytes(connIdx, (char *)&msg_size, sizeof(msg_size)))
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR,
                "%s: Error - Problem reading message size var_send_once", __func__);
                goto SendInitMessagesByConn_Err;
            }

            msg_size -= sizeof(msg_size);

            char * buffer = g_TVS_IO_AppData.servers[connIdx].buffer;
            ssize_t remainder = msg_size;
            if(ReadExactlyNBytes(connIdx, buffer, remainder))
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR,
                    "%s: Error - Problem reading value block from var_send_once", __func__);
                goto SendInitMessagesByConn_Err;
            }

            int32_t num_vars = TVS_UnpackSignedInteger( &buffer[0], 4 );
            if(num_vars != 2)
            {
                CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR,
                "%s: Error - Problem reading num vars for realtime tics requests", __func__);
                goto SendInitMessagesByConn_Err;
            } else {
                TVS_UnpackRealTimeData(&sTime, buffer+4);
            }
            // Parsing of real time values succeeded.
            break;
        }
    }

    // send out application-specific init messages...
    for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
    {
        if(mappings[i].connectionIndex == connIdx) {
            if (mappings[i].flowDirection & TrickToCfs)
            {
                for (int j = 0; j < mappings[i].memberCount; ++j)
                {
                    CheckVariableExistence(mappings[i].connectionIndex, mappings[i].validateMessages[j]);
                }

                const char **initMessages = mappings[i].initMessages;
                for (int j = 0; j < mappings[i].memberCount; ++j)
                {
                    if(SendTvsMessage(mappings[i].connectionIndex, initMessages[j]) < 0)
                    {
                        goto SendInitMessagesByConn_Err;
                    }
                }
            }
        }
    }

    // Process trick to cfs rate from mappings settings
    g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics = 0;
    for(size_t jj = 0; jj < TVS_IO_MAPPING_COUNT; ++jj)
    {
        TVS_IO_Mapping * currMap = &g_TVS_IO_AppData.mappings[jj];
        if(currMap->connectionIndex == connIdx && currMap->flowDirection != CfsToTrick && currMap->trickToCfsRate > 1.0e-6)
        {
            currMap->trickToCfsRateInTics = (long long)round(currMap->trickToCfsRate * timeTicValue);
            currMap->nextSendTic = currMap->trickToCfsRateInTics;
            g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics = GreatestCommonDivisor(g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics, currMap->trickToCfsRateInTics);
        }
    }
    if(g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics == 0)
    {
        g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics = (long long)round(TVS_DEFAULT_RATE*timeTicValue);
    }

    double simRtCycle = (double) sTime.rt_frame_tics/sTime.time_tic_value;
    long long simRtRateInTics = (long long)round(simRtCycle * timeTicValue);
    long long remainder = g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics % simRtRateInTics;
    double varCycle = (double)g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics/timeTicValue;
    if(remainder > 0)
    {
        CFE_EVS_SendEvent(__LINE__, CFE_EVS_EventType_ERROR, 
            "%s: Warning - Connection %d rate mismatch. Sim rate == %2.6f but requested rate is %2.6f\n", __func__, connIdx, simRtCycle, varCycle);
        g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics = simRtRateInTics;
        varCycle = (double)g_TVS_IO_AppData.servers[connIdx].trickToCfsRateInTics/timeTicValue;
    }
    char buff[64];
    snprintf(buff, sizeof(buff), "trick.var_cycle(%2.6f)\n", varCycle);
    SEND_ERR_EXIT(connIdx, buff); // Set the requested var server rate
    SEND_ERR_EXIT(connIdx, TVS_UNPAUSE_CMD);

    return;

SendInitMessagesByConn_Err:
    HandleServerDisconnect(connIdx, __LINE__, __func__);
}

int32 TryReadMessageByConn(int32 connIdx)
{
    /* Because it is possible for trick to send multiple messages for large buffers, 
       we loop and count the number of vars received for each connection to make sure we get it all. */
    if(g_TVS_IO_AppData.servers[connIdx].socket < 0)
    {
        return -1;
    }

    uint32 vars_received = 0;
    g_TVS_IO_AppData.frameDataBuffers[connIdx].frameBufferLength = 0;
    while (vars_received < TVS_IO_TOTAL_VARS_CONN[connIdx])
    {
        int32 message_indicator = -1;
        // char * buffer = &g_TVS_IO_AppData.servers[connIdx].buffer[0];
        if(ReadExactlyNBytes(connIdx, (char *)&message_indicator, sizeof(message_indicator)))
        {
            HandleServerDisconnect(connIdx, __LINE__, __func__);
            return -1;
        }

        if (message_indicator != VS_VAR_LIST)
        {
            DiscardTvsMessageBody(connIdx, message_indicator);
            continue;
        }

        // Read the remaining 8 bytes of the header: message_size and n_vars
        int32 message_size = -1, n_vars = -1;
        if(ReadExactlyNBytes(connIdx, (char *)&message_size, sizeof(message_size)))
        {
            HandleServerDisconnect(connIdx, __LINE__, __func__);
            return -1;
        }
        if(ReadExactlyNBytes(connIdx, (char *)&n_vars, sizeof(n_vars)))
        {
            HandleServerDisconnect(connIdx, __LINE__, __func__);
            return -1;
        }

        message_size -= 8; // chop off the header bytes from msg size

        int payloadBytesRead = 0;

        char * buffer = g_TVS_IO_AppData.frameDataBuffers[connIdx].frameBuffer;

        // read payload into buffer
        if(ReadExactlyNBytes(connIdx,  buffer + g_TVS_IO_AppData.frameDataBuffers[connIdx].frameBufferLength, message_size))
        {
            HandleServerDisconnect(connIdx, __LINE__, __func__);
            return -1;
        }

        vars_received += n_vars;

        g_TVS_IO_AppData.frameDataBuffers[connIdx].frameBufferLength += payloadBytesRead;
    }
    

    // process the frame data one mapping at a time
    TVS_IO_Mapping *mappings = g_TVS_IO_AppData.mappings;

    uint32_t byteOffsets = 0;

    /* Process the data from trick by placing into the MIDs and sending on the SB for other apps to pick up */
    for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
    {
        if(mappings[i].connectionIndex == connIdx) {
            if (mappings[i].flowDirection & TrickToCfs)
            {
                void *unpackedDataBuffer = mappings[i].unpackedDataBuffer;

                byteOffsets += mappings[i].unpack(unpackedDataBuffer, g_TVS_IO_AppData.frameDataBuffers[connIdx].frameBuffer + byteOffsets);

                CFE_SB_TimeStampMsg((CFE_MSG_Message_t *) mappings[i].unpackedDataBuffer);
                CFE_SB_TransmitMsg((CFE_MSG_Message_t*)mappings[i].unpackedDataBuffer, true);
                //Status = CFE_SB_TransmitMsg((CFE_MSG_Message_t*)mappings[i].unpackedDataBuffer, true); //TODO implement status error checking -JWP
            }
        }
    }

    return 1;
}

/* Sends a message to trick */
int32 SendTvsMessage(int conn, const char *commandString)
{
    if(g_TVS_IO_AppData.servers[conn].socket < 0)
    {
        return -1;
    }
    ssize_t sentBytes = write(g_TVS_IO_AppData.servers[conn].socket, commandString, strlen(commandString));
    if(sentBytes < 0)
    {
        HandleServerDisconnect(conn, __LINE__, __func__);
        return -1;
    }
    return 1;
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
int32 TVS_IO_InitEvent(void)
{
    /*
    ** Register with EVS
    */
    CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);

    return CFE_SUCCESS;
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
        iStatus = CFE_SB_SubscribeEx(CFE_SB_ValueToMsgId(TVS_IO_WAKEUP_MID), g_TVS_IO_AppData.SchPipeId, CFE_SB_DEFAULT_QOS, 1);

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
        iStatus = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TVS_IO_CMD_MID), g_TVS_IO_AppData.CmdPipeId);

        if (iStatus != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("TVS_IO - CMD Pipe failed to subscribe to TVS_IO_CMD_MID. (0x%08X)\n", iStatus);
            goto TVS_IO_InitPipe_Exit_Tag;
        }

        iStatus = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TVS_IO_SEND_HK_MID), g_TVS_IO_AppData.CmdPipeId);

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

    /* Subscribe to the MIDs we pull data from for sending to trick */
    if (iStatus == CFE_SUCCESS)
    {
        for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
        {
            if (mappings[i].flowDirection & CfsToTrick)
            {
                iStatus = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(mappings[i].msgId), g_TVS_IO_AppData.trickPipeId);

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
    int32  iStatus=CFE_SUCCESS;

    /* Init input data */
    memset((void*)&g_TVS_IO_AppData.InData, 0x00, sizeof(g_TVS_IO_AppData.InData));

    /* Init output data */
    memset((void*)&g_TVS_IO_AppData.OutData, 0x00, sizeof(g_TVS_IO_AppData.OutData));
    iStatus = CFE_MSG_Init((CFE_MSG_Message_t *)&g_TVS_IO_AppData.OutData,
                   CFE_SB_ValueToMsgId(TVS_IO_OUT_DATA_MID), sizeof(g_TVS_IO_AppData.OutData));

    /* Init housekeeping packet */
    memset((void*)&g_TVS_IO_AppData.HkTlm, 0x00, sizeof(g_TVS_IO_AppData.HkTlm));
    iStatus = CFE_MSG_Init((CFE_MSG_Message_t *)&g_TVS_IO_AppData.HkTlm,
                   CFE_SB_ValueToMsgId(TVS_IO_HK_TLM_MID), sizeof(g_TVS_IO_AppData.HkTlm));

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
    int32 iStatus = CFE_SUCCESS;

    g_TVS_IO_AppData.uiRunStatus = CFE_ES_RunStatus_APP_RUN;

    /* Calls generated code to initialize messages and buffers for each mapping in the tvm file(s) */
    TVS_IO_InitGeneratedCode(g_TVS_IO_AppData.mappings);

    /* Lots of initializing */
    if ((TVS_IO_InitEvent() != CFE_SUCCESS) || 
        (TVS_IO_InitPipe() != CFE_SUCCESS) || 
        (TVS_IO_InitData() != CFE_SUCCESS))
    {
        iStatus = -1;
        goto TVS_IO_InitApp_Exit_Tag;
    }

    /* Initialize socket connection data for trick variable servers */
    InitConnectionInfo();

    for (int conn = 0; conn < TVS_NUM_SIM_CONN; ++conn)
    {
        ConnectServerByConn(conn);
    }

TVS_IO_InitApp_Exit_Tag:
    if (iStatus == CFE_SUCCESS)
    {
        CFE_EVS_SendEvent(TVS_IO_INIT_INF_EID, CFE_EVS_EventType_INFORMATION, 
                        "Application initialized");
    }
    else
    {
        CFE_ES_WriteToSysLog("TVS_IO - Application failed to initialize\n");
    }


    return (iStatus);
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
    /* NOTE this is not where we receive trick or SB messages to send to trick */
    int32           iStatus=CFE_SUCCESS;
    CFE_SB_Buffer_t*   MsgPtr=NULL;
    CFE_SB_MsgId_t  MsgId;

    /* Stop Performance Log entry */
    CFE_ES_PerfLogExit(TVS_IO_MAIN_TASK_PERF_ID);

    /* Wait for WakeUp messages from scheduler */
    iStatus = CFE_SB_ReceiveBuffer(&MsgPtr, g_TVS_IO_AppData.SchPipeId, iBlocking);

    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TVS_IO_MAIN_TASK_PERF_ID);

    if (iStatus == CFE_SUCCESS)
    {
        iStatus = CFE_MSG_GetMsgId(&MsgPtr->Msg, &MsgId);
        switch (MsgId.Value)
        {
            case TVS_IO_WAKEUP_MID:
                TVS_IO_ProcessNewCmds();
                TVS_IO_ProcessNewData();

                /* The last thing to do at the end of this Wakeup cycle should be to
                   automatically publish new output. */
                TVS_IO_SendOutData();
                break;

            default:
                CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Recvd invalid SCH msgId (0x%08X)", MsgId.Value);
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
        CFE_EVS_SendEvent(TVS_IO_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
			  "TVS_IO: SB pipe read error (0x%08X), app will exit", iStatus);
        g_TVS_IO_AppData.uiRunStatus= CFE_ES_RunStatus_APP_ERROR;
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
    int iStatus = CFE_SUCCESS;
    CFE_SB_Buffer_t*   TlmMsgPtr=NULL;
    CFE_SB_MsgId_t     TlmMsgId;

    /* Process telemetry messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_ReceiveBuffer(&TlmMsgPtr, g_TVS_IO_AppData.TlmPipeId, CFE_SB_POLL);
        if (iStatus == CFE_SUCCESS)
        {
            iStatus = CFE_MSG_GetMsgId( &TlmMsgPtr->Msg, &TlmMsgId );
            switch (TlmMsgId.Value)
            {
                /* 
                **
                ** Example:
                **     case NAV_OUT_DATA_MID:
                **         TVS_IO_ProcessNavData(TlmMsgPtr);
                **         break;
                */
                default:
                    CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Recvd invalid TLM msgId (0x%08X)", TlmMsgId.Value);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(TVS_IO_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                  "TVS_IO: CMD pipe read error (0x%08X)", iStatus);
            g_TVS_IO_AppData.uiRunStatus = CFE_ES_RunStatus_APP_ERROR;
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
    int iStatus = CFE_SUCCESS;
    CFE_SB_Buffer_t*   CmdMsgPtr=NULL;
    CFE_SB_MsgId_t     CmdMsgId;

    /* Process command messages till the pipe is empty */
    while (1)
    {
        iStatus = CFE_SB_ReceiveBuffer(&CmdMsgPtr, g_TVS_IO_AppData.CmdPipeId, CFE_SB_POLL);
        if(iStatus == CFE_SUCCESS)
        {
            iStatus = CFE_MSG_GetMsgId(&CmdMsgPtr->Msg, &CmdMsgId);
            switch (CmdMsgId.Value)
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
                    CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "Recvd invalid CMD msgId (0x%08X)", CmdMsgId.Value);
                    break;
            }
        }
        else if (iStatus == CFE_SB_NO_MESSAGE)
        {
            break;
        }
        else
        {
            CFE_EVS_SendEvent(TVS_IO_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                  "TVS_IO: CMD pipe read error (0x%08X)", iStatus);
            g_TVS_IO_AppData.uiRunStatus = CFE_ES_RunStatus_APP_ERROR;
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
void TVS_IO_ProcessNewAppCmds(CFE_SB_Buffer_t* MsgPtr)
{
    //int32   Status;
    CFE_MSG_FcnCode_t  uiCmdCode=0;

    if (MsgPtr != NULL)
    {
        CFE_MSG_GetFcnCode(&MsgPtr->Msg, &uiCmdCode);
        switch (uiCmdCode)
        {
            case TVS_IO_NOOP_CC:
                g_TVS_IO_AppData.HkTlm.usCmdCnt++;
                CFE_EVS_SendEvent(TVS_IO_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "Recvd NOOP cmd (%d)", uiCmdCode);
                break;

            case TVS_IO_RESET_CC:
                g_TVS_IO_AppData.HkTlm.usCmdCnt = 0;
                g_TVS_IO_AppData.HkTlm.usCmdErrCnt = 0;
                CFE_EVS_SendEvent(TVS_IO_CMD_INF_EID, CFE_EVS_EventType_INFORMATION,
                                  "Recvd RESET cmd (%d)", uiCmdCode);
                break;

            

            default:
                g_TVS_IO_AppData.HkTlm.usCmdErrCnt++;
                CFE_EVS_SendEvent(TVS_IO_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "Recvd invalid cmdId (%d)", uiCmdCode);
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
    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*)&g_TVS_IO_AppData.HkTlm);
    CFE_SB_TransmitMsg((CFE_MSG_Message_t*)&g_TVS_IO_AppData.OutData, true);
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
    CFE_SB_TimeStampMsg((CFE_MSG_Message_t*)&g_TVS_IO_AppData.OutData);
    CFE_SB_TransmitMsg((CFE_MSG_Message_t*)&g_TVS_IO_AppData.OutData, true);
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
**    bool bResult - result of verification
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
bool TVS_IO_VerifyCmdLength(CFE_SB_Buffer_t* MsgPtr,
                           uint16 usExpectedLen)
{
    //int32   Status;
    bool bResult=false;
    CFE_MSG_Size_t    usMsgLen=0;
    CFE_MSG_FcnCode_t usCmdCode;
    CFE_SB_MsgId_t    MsgId;

    if (MsgPtr != NULL)
    {
        CFE_MSG_GetSize(&MsgPtr->Msg, &usMsgLen);

        if (usExpectedLen != usMsgLen)
        {
            CFE_MSG_GetMsgId(&MsgPtr->Msg, &MsgId);
            CFE_MSG_GetFcnCode(&MsgPtr->Msg, &usCmdCode);

            CFE_EVS_SendEvent(TVS_IO_MSGLEN_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Rcvd invalid msgLen: msgId=0x%08X, cmdCode=%d, "
                              "msgLen=%u, expectedLen=%d",
                              (uint32)MsgId.Value, usCmdCode, (uint32)usMsgLen, usExpectedLen);
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
    /* Start Performance Log entry */
    CFE_ES_PerfLogEntry(TVS_IO_MAIN_TASK_PERF_ID);

    /* Perform application initializations */
    /* This will spawn a child task which will handle reading from trick */
    if (TVS_IO_InitApp() != CFE_SUCCESS)
    {
        g_TVS_IO_AppData.uiRunStatus = CFE_ES_RunStatus_APP_ERROR;
    } else {
        /* Do not perform performance monitoring on startup sync */
        CFE_ES_PerfLogExit(TVS_IO_MAIN_TASK_PERF_ID);
        CFE_ES_WaitForStartupSync(TVS_IO_TIMEOUT_MSEC);
        CFE_ES_PerfLogEntry(TVS_IO_MAIN_TASK_PERF_ID);
    }

    int32 iStatus = CFE_SUCCESS;
    CFE_SB_Buffer_t *trickCmdMsgPtr = NULL;
    CFE_SB_MsgId_t   mid;
    CFE_MSG_FcnCode_t cmdCode;

    TVS_IO_Mapping *mappings = g_TVS_IO_AppData.mappings; // local convenience pointer

    /* Application main loop */
    while (CFE_ES_RunLoop(&g_TVS_IO_AppData.uiRunStatus))
    {
        g_TVS_IO_AppData.currVirtualTimeInTics = calculateNextVirtualTimeTic();
        ReadMessagesFromServers();

        /* This will handle sending data to trick */
        while(1)
        {
            /* Get the message from the software bus */
            iStatus = CFE_SB_ReceiveBuffer(&trickCmdMsgPtr, g_TVS_IO_AppData.trickPipeId, CFE_SB_POLL);

            if (iStatus == CFE_SUCCESS)
            {
                //TODO again,set but never used
                iStatus = CFE_MSG_GetMsgId(&trickCmdMsgPtr->Msg, &mid);
                iStatus = CFE_MSG_GetFcnCode(&trickCmdMsgPtr->Msg, &cmdCode);

                /* If this MID and CC are used for any mappings, pack the message and send it to trick */
                for (int i = 0; i < TVS_IO_MAPPING_COUNT; ++i)
                {
                    if ((mappings[i].msgId == mid.Value) && (mappings[i].flowDirection & CfsToTrick))
                    {
                        if (mappings[i].commandCode != cmdCode)
                        {
                            continue;
                        }

                        char **cmdBuffer = mappings[i].packedCommandBuffer;

                        mappings[i].pack((void **)cmdBuffer, (void *)trickCmdMsgPtr);

                        for (int j = 0; j < mappings[i].memberCount; ++j) {
                            if(SendTvsMessage(mappings[i].connectionIndex, cmdBuffer[j]) < 0)
                            {
                                break;
                            }
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
