/*=======================================================================================
** File Name:  tvsio_app.h
**
** Title:  Header File for TVSIO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  To define TVSIO's internal macros, data types, global variables and
**           function prototypes
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TVSIO_APP_H_
#define _TVSIO_APP_H_

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
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include "tvsio_platform_cfg.h"
#include "tvsio_private_ids.h"
#include "tvsio_private_types.h"
#include "tvsio_perfids.h"
#include "tvsio_msgids.h"
#include "tvsio_msg.h"

#include "tvsio_generated.h"

/*
** Local Defines
*/

/* Return codes */
#define TVSIO_CONN_SUCCESS (1)
#define TVSIO_CONN_FAILURE (-1)

typedef struct
{
   long long rt_frame_tics;
   int time_tic_value;

} TVSIO_SimTime_t;

/*
** Local Structure Declarations
*/
typedef struct
{
    /* CFE Event table */
    CFE_EVS_BinFilter_t  EventTbl[TVSIO_EVT_CNT];

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
       Data structure should be defined in tvsio/fsw/src/tvsio_private_types.h */
    TVSIO_InData_t   InData;

    /* Output data - to be published at the end of a Wakeup cycle.
       Data structure should be defined in tvsio/fsw/src/tvsio_private_types.h */
    TVSIO_OutData_t  OutData;

    /* Housekeeping telemetry - for downlink only.
       Data structure should be defined in tvsio/fsw/src/tvsio_msg.h */
    TVSIO_HkTlm_t  HkTlm;

    /* TODO:  Add declarations for additional private data here */

    TVSIO_TrickServer_t servers[TVS_NUM_SIM_CONN];
    long long currVirtualTimeInTics;

    CFE_ES_TaskId_t receiveTaskId;

    TVSIO_Mapping mappings[TVSIO_MAPPING_COUNT];

    // this buffer is designed to hold an entire frame's worth of data for processing
    TVSIO_FrameDataBuffer_t frameDataBuffers[TVS_NUM_SIM_CONN];

} TVSIO_AppData_t;

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

int32 InitConnectionInfo(void);
int32 ConnectToTrickVariableServerByConn(int32 connIdx);
void CheckVariableExistence(int32 connIdx, const char *commandString);
void SendInitMessagesByConn(int32 connIdx);
int32 TryReadMessageByConn(int32 connIdx);
int32 SendTvsMessage(int conn, const char *commandString);  //TODO should this be using CFE_SB_Msg_t (as it was before we changed the header to match the definition)

int32 TVSIO_InitApp(void);
int32 TVSIO_InitEvent(void);
int32 TVSIO_InitData(void);
int32 TVSIO_InitPipe(void);

void  TVSIO_AppMain(void);

int32 TVSIO_RcvMsg(int32 iBlocking);

void  TVSIO_ProcessNewData(void);
void  TVSIO_ProcessNewCmds(void);
void  TVSIO_ProcessNewAppCmds(CFE_SB_Buffer_t*);

void  TVSIO_ReportHousekeeping(void);
void  TVSIO_SendOutData(void);

bool  TVSIO_VerifyCmdLength(CFE_SB_Buffer_t*, uint16);

#endif /* _TVSIO_APP_H_ */

/*=======================================================================================
** End of file tvsio_app.h
**=====================================================================================*/
    
