/*=======================================================================================
** File Name:  tvsio_platform_cfg.h
**
** Title:  Platform Configuration Header File for TVSIO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  This header file contains declartions and definitions of all TVSIO's 
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TVSIO_PLATFORM_CFG_H_
#define _TVSIO_PLATFORM_CFG_H_

/*
** tvsio Platform Configuration Parameter Definitions
*/
/* Depth of the pipe used to receive the app's scheduler ("wakeup") messages */
#define TVSIO_SCH_PIPE_DEPTH  2
/* Depth of the pipe used to receive ground/flight commands addressed to TVSIO */
#define TVSIO_CMD_PIPE_DEPTH  10
/* Depth of the pipe used to receive telemetry data to be forwarded to Trick */
#define TVSIO_TLM_PIPE_DEPTH  10
/* Depth of the queue used to pass data received from apps to push to the Trick servers */
#define TVSIO_TRICK_PIPE_DEPTH OS_QUEUE_MAX_DEPTH

/* Milliseconds to wait on CFE_ES_WaitForStartupSync() during app initialization */
#define TVSIO_TIMEOUT_MSEC    1000

/* Number of simultaneous Trick simulation connections supported; sizes the servers/frameDataBuffers arrays */
#ifndef TVS_NUM_SIM_CONN
    #define TVS_NUM_SIM_CONN 2
#endif

/* IP addresses of the Trick variable servers, one per connection (indexed in parallel with TVS_SERVER_PORTS) */
#ifndef TVS_SERVER_IPS
    #define TVS_SERVER_IPS ((char const*[]) { "127.0.0.1", "127.0.0.1" })
#endif

/* TCP ports of the Trick variable servers, one per connection (indexed in parallel with TVS_SERVER_IPS) */
#ifndef TVS_SERVER_PORTS
    #define TVS_SERVER_PORTS (int[]) {17000, 17001}
#endif

/* Trick variable server command to pause the variable server's data cycling */
#define TVS_PAUSE_CMD "trick.var_pause()\n"

/* Trick variable server command to resume the variable server's data cycling */
#define TVS_UNPAUSE_CMD "trick.var_unpause()\n"

/* Trick variable server command sent at connection setup to briefly run the cycle rate fast (0.005s) before TVS_DEFAULT_RATE or the user-provided rate takes over */
#define TVS_GO_FAST_CMD "trick.var_cycle(0.005)\n"

/* Default data cycle rate (in seconds) requested from the Trick variable server once connected if no rate specified */
#define TVS_DEFAULT_RATE 0.1

/* Seconds to wait between successive reconnection attempts after a dropped/failed connection */
#define TVS_CONNECTION_RETRY_RATE 5

/* Seconds between periodic "still trying to connect" status event messages while awaiting an initial connection */
#define TVS_CONNECTION_ATTEMPT_RATE 60

/* Trick variable server command to disable sending of variable names with binary data (reduces data volume) */
#define TVS_SET_BINARY_NO_NAMES "trick.var_binary_nonames()\n"

/* Trick variable server command selecting the copy mode (0 = async, 1 = scheduled, 2 = copy at top of frame) used when gathering variable data */
#define TVS_SET_COPY_MODE_CMD "trick.var_set_copy_mode(0)\n"

/* Trick variable server command selecting the write mode (0 = async, 1 = write when copied) used when sending variable data */
#define TVS_SET_WRITE_MODE_CMD "trick.var_set_write_mode(0)\n"

/* Trick variable server command to send the sim time and frame-tic variables at connection setup, used to establish timing */
#define TVS_RT_FRAME_SEND_ONCE_CMD "trick.var_send_once(\"trick_sys.sched.time_tic_value,trick_sys.sched.software_frame_tics\", 2)\n"

/* Size (in bytes) of the per-connection buffer used to hold one frame's worth of data received from a Trick variable server */
#define TVSIO_FRAME_DATA_BUFFER_SIZE 81920 // 80 kB default - can be tuned up or down based on data rate per frame

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* Caution: The byte swap implementation below is expermental!       */
/* This will only byte swap data coming FROM trick (flowDirection 1) */
/* Compile-time flag enabling the experimental byte-swap-on-receive code path; set to true to enable, use at your own risk */
#define TVS_BYTE_SWAP false // Set to true to byte swap, use at your own risk
/* Trick variable server command enabling byte swapping of data sent by the variable server (flowDirection 1), only sent when TVS_BYTE_SWAP is true */
#define TVS_SET_BYTE_SWAP_CMD "trick.var_byteswap(1)\n"
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#endif /* _TVSIO_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file tvsio_platform_cfg.h
**=====================================================================================*/
    
