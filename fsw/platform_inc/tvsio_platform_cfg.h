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
#define TVSIO_SCH_PIPE_DEPTH  2
#define TVSIO_CMD_PIPE_DEPTH  10
#define TVSIO_TLM_PIPE_DEPTH  10
#define TVSIO_TRICK_PIPE_DEPTH OS_QUEUE_MAX_DEPTH

#define TVSIO_RCV_THREAD_STACK_SIZE 16384

#define TVSIO_TIMEOUT_MSEC    1000

#ifndef TVS_NUM_SIM_CONN
    #define TVS_NUM_SIM_CONN 2
#endif

#ifndef TVS_SERVER_IPS
    #define TVS_SERVER_IPS ((char const*[]) { "127.0.0.1", "127.0.0.1" })
#endif

#ifndef TVS_SERVER_PORTS
    #define TVS_SERVER_PORTS (int[]) {17000, 17001}
#endif

#define TVS_PAUSE_CMD "trick.var_pause()\n"

#define TVS_UNPAUSE_CMD "trick.var_unpause()\n"

#define TVS_GO_FAST_CMD "trick.var_cycle(0.005)\n"

#define TVS_DEFAULT_RATE 0.1

#define TVS_CONNECTION_RETRY_RATE 5

#define TVS_CONNECTION_ATTEMPT_RATE 60

#define TVS_SET_BINARY_NO_NAMES "trick.var_binary_nonames()\n"

#define TVS_SET_COPY_MODE_CMD "trick.var_set_copy_mode(0)\n"

#define TVS_SET_WRITE_MODE_CMD "trick.var_set_write_mode(0)\n"

#define TVS_RT_FRAME_SEND_ONCE_CMD "trick.var_send_once(\"trick_sys.sched.time_tic_value,trick_sys.sched.software_frame_tics\", 2)\n"

#define TVSIO_FRAME_DATA_BUFFER_SIZE 81920 // 80 kB default - can be tuned up or down based on data rate per frame

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* Caution: The byte swap implementation below is expermental!       */
/* This will only byte swap data coming FROM trick (flowDirection 1) */
#define TVS_BYTE_SWAP false // Set to true to byte swap, use at your own risk
#define TVS_SET_BYTE_SWAP_CMD "trick.var_byteswap(1)\n"
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#endif /* _TVSIO_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file tvsio_platform_cfg.h
**=====================================================================================*/
    
