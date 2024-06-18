/*=======================================================================================
** File Name:  tvs_io_platform_cfg.h
**
** Title:  Platform Configuration Header File for TVS_IO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  This header file contains declartions and definitions of all TVS_IO's 
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TVS_IO_PLATFORM_CFG_H_
#define _TVS_IO_PLATFORM_CFG_H_

/*
** tvs_io Platform Configuration Parameter Definitions
*/
#define TVS_IO_SCH_PIPE_DEPTH  2
#define TVS_IO_CMD_PIPE_DEPTH  10
#define TVS_IO_TLM_PIPE_DEPTH  10
#define TVS_IO_TRICK_PIPE_DEPTH 256

#define TVS_IO_RCV_THREAD_STACK_SIZE 16384

#define TVS_IO_TIMEOUT_MSEC    1000

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

#define TVS_SET_BINARY_NO_NAMES "trick.var_binary_nonames()\n"

#define TVS_SET_COPY_MODE_CMD "trick.var_set_copy_mode(0)\n"

#define TVS_SET_WRITE_MODE_CMD "trick.var_set_write_mode(0)\n"

#define TVS_IO_FRAME_DATA_BUFFER_SIZE 81920 // 80 kB default - can be tuned up or down based on data rate per frame

#endif /* _TVS_IO_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file tvs_io_platform_cfg.h
**=====================================================================================*/
    
