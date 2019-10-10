#include <string.h>

#include "tvs_io_generated.h"
#include "tvs_io_utils.h"

char *TVS_PDU1_Init_Msgs[TVS_PDU1_MEMBER_COUNT] = 
{
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.id\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_cmd[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_cmd[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_cmd[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_feedback[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_feedback[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_feedback[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.invalid_command_error\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_mismatch[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_mismatch[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_switch_mismatch[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_posNeg_mismatch[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_posNeg_mismatch[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_posNeg_mismatch[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[3]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[4]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[5]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[6]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_feedback[7]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[3]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[4]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[5]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[6]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_trip_state[7]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[3]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[4]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[5]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[6]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_cmd[7]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[3]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[4]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[5]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[6]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_switch_mismatch[7]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_current[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_current[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.internal_current\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_voltage[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.input_voltage[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.internal_voltage\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.housekeeping_voltage\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[0]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[1]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[2]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[3]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[4]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[5]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[6]\") \n",
	"trick.var_add(\"hab_amps_cfs.pdu1.tlm.output_current[7]\") \n",
};

void TVS_IO_InitGeneratedCode(TVS_IO_Mapping *mappings)
{
	mappings[0].memberCount = TVS_PDU1_MEMBER_COUNT;
	mappings[0].msgId = 0x7E6A;
	mappings[0].commandCode = 0;
	mappings[0].packetType = 1;
	mappings[0].flowDirection = 1;
	mappings[0].initMessages = TVS_PDU1_Init_Msgs;
	mappings[0].unpackedDataBuffer = (char*)malloc(sizeof(PDU1));
	CFE_SB_InitMsg(mappings[0].unpackedDataBuffer,
					0x7E6A, sizeof(PDU1), TRUE);

	CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)mappings[0].unpackedDataBuffer, 0);
	mappings[0].unpack = TVS_Unpack_0x7E6A;

}

void TVS_Unpack_0x7E6A(void *mystruct, void *buffer)
{
	PDU1 *mystructptr = (PDU1*)mystruct;
	char *data = (char*)buffer;

	uint32 byteOffset = 0;
	int32 currentMemberLength = -1;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->PDU_UNITID = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->STATUS_INPUTBUS1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->STATUS_INPUTBUS2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->STATUS_INPUTPAD = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->FEEDBACK_INPUTRELAY1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->FEEDBACK_INPUTRELAY2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->FEEDBACK_INPUT_PAD = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->PDU_INVALIDCOMMANDERROR = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->FEEDBACKSTATEMISMATCHERROR_INPUTBUS1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->FEEDBACKSTATEMISMATCHERROR_INPUTBUS2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->FEEDBACKSTATEMISMATCHERROR_PAD = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->POSNEGMISMATCHERROR_INPUT1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->POSNEGMISMATCHERROR_INPUT2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->POSNEGMISMATCHERROR_PAD = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC3 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC4 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC5 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC6 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC7 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->CHANNELSTATUS_RPC8 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC3 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC4 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC5 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC6 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC7 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->TRIPSTATUS_RPC8 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC3 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC4 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC5 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC6 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC7 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->OUTSTATUS_RPC8 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC1 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC2 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC3 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC4 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC5 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC6 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC7 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	currentMemberLength = *((int32*) &data[byteOffset + 4] );
	mystructptr->MISMATCHERROR_RPC8 = TVS_UnpackUnsignedInteger( &data[byteOffset + 8], currentMemberLength );
	byteOffset += 8 + currentMemberLength;

	mystructptr->PDU_CURRENTBUS_1 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTBUS_2 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTINTERNALBUS = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_VOLTAGEBUS_1 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_VOLTAGEBUS_2 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_VOLTAGEINTERNAL = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_VOLTAGEHKPG = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_1 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_2 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_3 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_4 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_5 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_6 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_7 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

	mystructptr->PDU_CURRENTRPC_8 = TVS_UnpackDouble( &data[byteOffset + 8] );
	byteOffset += 16;

}

