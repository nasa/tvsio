/*=======================================================================================
** File Name:  tvs_io_private_types.h
**
** Title:  Type Header File for TVS_IO Application
**
** $Author:    Nexsys
** $Revision: 1.1 $
** $Date:      2018-03-08
**
** Purpose:  This header file contains declarations and definitions of all TVS_IO's private
**           data structures and data types.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-08 | Nexsys | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _TVS_IO_PRIVATE_TYPES_H_
#define _TVS_IO_PRIVATE_TYPES_H_

/*
** Pragmas
*/

/*
** Include Files
*/
#include <stdint.h>
#include <netinet/in.h>

#include "cfe.h"

/*
** Local Defines
*/

typedef void (*TVS_PackStructFunction)(void **buffer, void *mystruct);
typedef uint32_t (*TVS_UnpackStructFunction)(void *mystruct, void *buffer);

/*
** Local Structure Declarations
*/

enum FlowDirection { TrickToCfs = 1, CfsToTrick = 2, BiDirectional = 3 };

typedef struct
{
    int32 memberCount;
    char **initMessages; // will be NULL for command types

    uint8 packetType; // 0 for tlm, 1 for cmd
    uint32 commandCode;

    uint8 flowDirection;
    uint32 msgId;
    int32 unpackedSize;

    // TODO: make sure these are initialized on the heap in your generated code
    char *unpackedDataBuffer; // include header (filled out on init) for performance reasons in this buffer...
    char **packedCommandBuffer;

    TVS_PackStructFunction pack;
    TVS_UnpackStructFunction unpack;
    
} TVS_IO_Mapping;

typedef struct
{
    uint8  ucCmdHeader[CFE_SB_CMD_HDR_SIZE];
} TVS_IO_NoArgCmd_t;

typedef struct
{
    uint32  counter;

    /* TODO:  Add input data to this application here, such as raw data read from I/O
    **        devices or data subscribed from other apps' output data.
    */

} TVS_IO_InData_t;

typedef struct
{
    uint8   ucTlmHeader[CFE_SB_TLM_HDR_SIZE];
    uint32  uiCounter;
} TVS_IO_OutData_t;


typedef struct
{
    int32 socket;
    struct sockaddr_in serv_addr;

} TVS_IO_TrickServer_t;

/* TODO:  Add more private structure definitions here, if necessary. */

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
** Local Function Prototypes
*/

#endif /* _TVS_IO_PRIVATE_TYPES_H_ */

/*=======================================================================================
** End of file tvs_io_private_types.h
**=====================================================================================*/
    
