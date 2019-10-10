#ifndef __TVS_IO_GENERATED_H__
#define __TVS_IO_GENERATED_H__

#include "cfe_sb.h"

#include "tvs_io_private_types.h"

#include "CannonStructDef.h"
#include "CannonStructDef.h"

#define TVS_IO_MAPPING_COUNT 2
#define TVS_IO_MAX_COMMAND_STRLEN 1024

void TVS_IO_InitGeneratedCode(TVS_IO_Mapping *mappings);

#define TVS_STRUCT_CANNON_MEMBER_COUNT 9

extern char *TVS_Struct_Cannon_Init_Msgs[TVS_STRUCT_CANNON_MEMBER_COUNT];

void TVS_Unpack_0x01BA(void *mystruct, void *buffer);
#define TVS_STRUCT_CANNON_SETVELCMD_MEMBER_COUNT 3

void TVS_Pack_0x19BA(void **buffer, void *mystruct);

#endif // __TVS_IO_GENERATED_H__