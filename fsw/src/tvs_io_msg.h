/*=======================================================================================
** File Name:  tvs_io_msg.h
**
** Title:  Message Definition Header File for TVS_IO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  To define TVS_IO's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TVS_IO_MSG_H_
#define _TVS_IO_MSG_H_

/*
** Pragmas
*/

/*
** Include Files
*/



/*
** Local Defines
*/

/*
** TVS_IO command codes
*/
#define TVS_IO_NOOP_CC                 0
#define TVS_IO_RESET_CC                1

/*
** Local Structure Declarations
*/
typedef struct
{
    CFE_MSG_TelemetryHeader_t TlmHeader;
    uint8                     usCmdCnt;
    uint8                     usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} TVS_IO_HkTlm_t;


#endif /* _TVS_IO_MSG_H_ */

/*=======================================================================================
** End of file tvs_io_msg.h
**=====================================================================================*/
    
