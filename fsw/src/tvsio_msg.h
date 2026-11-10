/*=======================================================================================
** File Name:  tvsio_msg.h
**
** Title:  Message Definition Header File for TVSIO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  To define TVSIO's command and telemetry message defintions 
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TVSIO_MSG_H_
#define _TVSIO_MSG_H_

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
** TVSIO command codes
*/
#define TVSIO_NOOP_CC                 0
#define TVSIO_RESET_CC                1

/*
** Local Structure Declarations
*/
typedef struct
{
    CFE_MSG_TelemetryHeader_t   TlmHeader;
    uint8                       usCmdCnt;
    uint8                       usCmdErrCnt;

    /* TODO:  Add declarations for additional housekeeping data here */

} TVSIO_HkTlm_t;


#endif /* _TVSIO_MSG_H_ */

/*=======================================================================================
** End of file tvsio_msg.h
**=====================================================================================*/