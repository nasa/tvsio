#ifndef __TVS_IO_GENERATED_H__
#define __TVS_IO_GENERATED_H__

#include "cfe_sb.h"

#include "tvs_io_private_types.h"

#include "AMPS_types.h"

#define TVS_IO_MAPPING_COUNT 1
#define TVS_IO_MAX_COMMAND_STRLEN 1024

void TVS_IO_InitGeneratedCode(TVS_IO_Mapping *mappings);

#define TVS_PDU1_MEMBER_COUNT 61

extern char *TVS_PDU1_Init_Msgs[TVS_PDU1_MEMBER_COUNT];

void TVS_Unpack_0x7E6A(void *mystruct, void *buffer);
#endif // __TVS_IO_GENERATED_H__