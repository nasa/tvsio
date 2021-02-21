/*=======================================================================================
** File Name:  tvs_io_app.h
**
** Title:  Header File for TVS_IO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  To define TVS_IO's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TVS_IO_APP_H_
#define _TVS_IO_APP_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include "tvs_io_platform_cfg.h"
#include "tvs_io_mission_cfg.h"
#include "tvs_io_private_ids.h"
#include "tvs_io_private_types.h"
#include "tvs_io_perfids.h"
#include "tvs_io_msgids.h"
#include "tvs_io_msg.h"

#include "tvs_io_generated.h"

/*
** Local Defines
*/

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[TVS_IO_EVT_CNT];

    /* CFE scheduling pipe */
    CFE_SB_PipeId_t  SchPipeId; 
    uint16           usSchPipeDepth;
    char             cSchPipeName[OS_MAX_API_NAME];

    /* CFE command pipe */
    CFE_SB_PipeId_t  CmdPipeId;
    uint16           usCmdPipeDepth;
    char             cCmdPipeName[OS_MAX_API_NAME];
    
    /* CFE telemetry pipe */
    CFE_SB_PipeId_t  TlmPipeId;
    uint16           usTlmPipeDepth;
    char             cTlmPipeName[OS_MAX_API_NAME];

    /* Trick outbound msg pipe */
    CFE_SB_PipeId_t trickPipeId;
    uint16          trickPipeDepth;
    char            trickPipeName[OS_MAX_API_NAME];

    /* Task-related */
    uint32  uiRunStatus;
    
    /* Input data - from I/O devices or subscribed from other apps' output data.
       Data structure should be defined in tvs_io/fsw/src/tvs_io_private_types.h */
    TVS_IO_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in tvs_io/fsw/src/tvs_io_private_types.h */
    TVS_IO_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in tvs_io/fsw/src/tvs_io_msg.h */
    TVS_IO_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */

    //NOTE this can also be declared a static array at compile time since user should have set TVS_NUM_SIM_CONN, similarly to frameDataBuffers below
    //TVS_IO_TrickServer_t servers[TVS_NUM_SIM_CONN]; // TODO Decide which is the best approach -JWP
    TVS_IO_TrickServer_t * servers;

    uint32 receiveTaskId;

    TVS_IO_Mapping mappings[TVS_IO_MAPPING_COUNT];

    // this buffer is designed to hold an entire frame's worth of data for processing
    TVS_IO_FrameDataBuffer_t frameDataBuffers[TVS_NUM_SIM_CONN];

} TVS_IO_AppData_t;

/*
** External Global Variables
*/

/*
** Global Variables
*/

/*
** Local Variables
*/

/*
**  Local Function Prototypes
**
**  Putting here mainly as a brief list for reference...
**  Not all of these are meant to be used externally.
*/

int32 InitConnectionInfo();
int32 ConnectToTrickVariableServer();
int32 SendInitMessages();
int32 TryReadMessage();
int32 SendTvsMessage(int conn, char *commandString);  //TODO should this be using CFE_SB_Msg_t (as it was before we changed the header to match the definition)
void  ReceiveTaskRun();

int32 TVS_IO_InitApp(void);
int32 TVS_IO_InitEvent(void);
int32 TVS_IO_InitData(void);
int32 TVS_IO_InitPipe(void);

void  TVS_IO_AppMain(void);

void  TVS_IO_CleanupCallback(void);

int32 TVS_IO_RcvMsg(int32 iBlocking);

void  TVS_IO_ProcessNewData(void);
void  TVS_IO_ProcessNewCmds(void);
void  TVS_IO_ProcessNewAppCmds(CFE_SB_Msg_t*);

void  TVS_IO_ReportHousekeeping(void);
void  TVS_IO_SendOutData(void);

boolean  TVS_IO_VerifyCmdLength(CFE_SB_Msg_t*, uint16);

#endif /* _TVS_IO_APP_H_ */

/*=======================================================================================
** End of file tvs_io_app.h
**=====================================================================================*/
    
