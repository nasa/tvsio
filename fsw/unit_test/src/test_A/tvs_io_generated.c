#include <string.h>

#include "tvs_io_generated.h"
#include "tvs_io_utils.h"

char *TVS_Struct_Cannon_Init_Msgs[TVS_STRUCT_CANNON_MEMBER_COUNT] = 
{
	"trick.var_add(\"dyn.cannon.rf.delta_time\") \n",
	"trick.var_add(\"trick_zero_conf.zc.name\") \n",
	"trick.var_add(\"dyn.cannon.impact\") \n",
	"trick.var_add(\"dyn.cannon.rf.lower_set\") \n",
	"trick.var_add(\"dyn.cannon.rf.upper_set\") \n",
	"trick.var_add(\"dyn.cannon.pos[0]\") \n",
	"trick.var_add(\"dyn.cannon.pos[1]\") \n",
	"trick.var_add(\"dyn.cannon.vel[0]\") \n",
	"trick.var_add(\"dyn.cannon.vel[1]\") \n",
};

void TVS_IO_InitGeneratedCode(TVS_IO_Mapping *mappings)
{
	mappings[0].memberCount = TVS_STRUCT_CANNON_MEMBER_COUNT;
	mappings[0].msgId = 0x01BA;
	mappings[0].commandCode = 0;
	mappings[0].packetType = 0;
	mappings[0].flowDirection = 1;
	mappings[0].initMessages = TVS_Struct_Cannon_Init_Msgs;
	mappings[0].unpackedDataBuffer = (char*)malloc(sizeof(Struct_Cannon));
	CFE_SB_InitMsg(mappings[0].unpackedDataBuffer,
					0x01BA, sizeof(Struct_Cannon), TRUE);

	mappings[0].unpack = TVS_Unpack_0x01BA;

	mappings[1].memberCount = TVS_STRUCT_CANNON_SETVELCMD_MEMBER_COUNT;
	mappings[1].msgId = 0x19BA;
	mappings[1].commandCode = 23;
	mappings[1].packetType = 1;
	mappings[1].flowDirection = 2;
	mappings[1].packedCommandBuffer = (char**)malloc(3*sizeof(char*));
	for (int i = 0; i < 3; ++i)
		mappings[1].packedCommandBuffer[i] = (char*)malloc(1024);

	mappings[1].pack = TVS_Pack_0x19BA;

}

void TVS_Unpack_0x01BA(void *mystruct, void *buffer)
{
	Struct_Cannon *mystructptr = (Struct_Cannon*)mystruct;
	char *data = (char*)buffer;

	uint32 byteOffset = 0;
	int32 currentMemberLength = -1;

	mystructptr->rf.delta_time = TVS_UnpackFloat( &data[byteOffset + 8] );
	byteOffset += 12;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	memcpy(mystructptr->mystring, &data[byteOffset + 8], currentMemberLength);
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->impact = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	mystructptr->rf.lower_set = TVS_UnpackFloat( &data[byteOffset + 8] );
	byteOffset += 12;

	mystructptr->rf.upper_set = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->pos[0] = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->pos[1] = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->vel[0] = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->vel[1] = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

}

void TVS_Pack_0x19BA(void **buffer, void *mystruct)
{
	Struct_Cannon_SetVelCmd *mystructptr = (Struct_Cannon_SetVelCmd*)mystruct;
	char **data = (char**)buffer;

	snprintf(data[0], TVS_IO_MAX_COMMAND_STRLEN, "trick.var_set('dyn.cannon.vel[0]', %f) \n", mystructptr->vel[0]);
	snprintf(data[1], TVS_IO_MAX_COMMAND_STRLEN, "trick.var_set('dyn.cannon.vel[1]', %f) \n", mystructptr->vel[1]);
	snprintf(data[2], TVS_IO_MAX_COMMAND_STRLEN, "trick.var_set('trick_zero_conf.zc.name', '%s') \n", mystructptr->mystring);
}

