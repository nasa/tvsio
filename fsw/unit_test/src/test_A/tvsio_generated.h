#ifndef __TVSIO_GENERATED_H__
#define __TVSIO_GENERATED_H__

#include "cfe_sb.h"

#include "tvsio_private_types.h"

#include "CannonStructDef.h"
#include "CannonStructDef.h"

#define TVSIO_MAPPING_COUNT 2
#define TVSIO_MAX_COMMAND_STRLEN 1024

void TVSIO_InitGeneratedCode(TVSIO_Mapping *mappings);

#define TVS_STRUCT_CANNON_MEMBER_COUNT 9

extern char *TVS_Struct_Cannon_Init_Msgs[TVS_STRUCT_CANNON_MEMBER_COUNT];

void TVS_Unpack_0x01BA(void *mystruct, void *buffer);
#define TVS_STRUCT_CANNON_SETVELCMD_MEMBER_COUNT 3

void TVS_Pack_0x19BA(void **buffer, void *mystruct);

#endif // __TVSIO_GENERATED_H__