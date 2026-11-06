#ifndef __TVSIO_GENERATED_H__
#define __TVSIO_GENERATED_H__

#include "cfe_sb.h"

#include "tvsio_private_types.h"

#include "AMPS_types.h"

#define TVSIO_MAPPING_COUNT 1
#define TVSIO_MAX_COMMAND_STRLEN 1024

void TVSIO_InitGeneratedCode(TVSIO_Mapping *mappings);

#define TVS_PDU1_MEMBER_COUNT 61

extern char *TVS_PDU1_Init_Msgs[TVS_PDU1_MEMBER_COUNT];

void TVS_Unpack_0x7E6A(void *mystruct, void *buffer);
#endif // __TVSIO_GENERATED_H__