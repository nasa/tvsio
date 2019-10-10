#include <stdint.h>
#include <math.h>

#include "tvmc_test_amps.h"

const float fpTol = 1e-3;

// TODO: in your test structure definition file CannonStructDef.h, you should have an 
//          #include "cfe_sb.h" in it...

void TVMC_Pack_0x19BA_Test(void)
{
    #define OUTPUTS_COUNT 5

    // TODO: finish initialization of outputs
    const char *outputs[OUTPUTS_COUNT] = {
        "trick.var_set('hab_amps_cfs.pdu1.tlm.id', ) \n",
        "trick.var_set('', ) \n", 
        "trick.var_set('', ) \n", 
        "trick.var_set('', ) \n", 
        "trick.var_set('', ) \n"
    }

    PDU1 cmd;

    // TODO: set 'cmd' values

    // allocate the buffers for your serialized representations
	char **packedCommandBuffer = (char**)malloc(3*sizeof(char*));
	for (int i = 0; i < 3; ++i)
		packedCommandBuffer[i] = (char*)malloc(1024);

    TVS_Pack_0x19BA(packedCommandBuffer, &cmd);

    for (int i = 0; i < 3; ++i)
    {
        UtAssert_StrCmp(packedCommandBuffer[i], outputs[i], "strcmp test amps");
    }

    for (int i = 0; i < 3; ++i)
        free(packedCommandBuffer[i]);
    
    free(packedCommandBuffer);
    #undef OUTPUTS_COUNT
}

void TVMC_Unpack_0x01BA_Test(void)
{
    // populate your packed data first... watch your book-keeping here!
    char packedData[4096];

    int32_t intBuffer;
    uint32_t uintBuffer;
    float floatBuffer;
    double doubleBuffer;
    char byteBuffer[512] = "the quick brown fox jumps over the lazy dog!";

    // rf.delta_time
    doubleBuffer = 1234.56789101;
    memcpy(&packedData[8], &doubleBuffer, 8);

    // mystring's length
    intBuffer = 512;
    memcpy(&packedData[16], &intBuffer, 4);

    // mystring
    memcpy(&packedData[20], byteBuffer, 512);

    // impact length
    intBuffer = 4;
    memcpy(&packedData[536], &intBuffer, 4);

    // impact
    uintBuffer = 42;
    memcpy(&packedData[540], &uintBuffer, 4);

    // rf.lower_set
    floatBuffer = 3.3456;
    memcpy(&packedData[552], &floatBuffer, 4);

    // rf.upper_set
    doubleBuffer = 2.123456;
    memcpy(&packedData[564], &doubleBuffer, 8);

    // pos[0]
    doubleBuffer = 24.24567;
    memcpy(&packedData[580], &doubleBuffer, 8);

    // pos[1]
    doubleBuffer = 54321.54321;
    memcpy(&packedData[596], &doubleBuffer, 8);

    // vel[0]
    doubleBuffer = 987654.54321;
    memcpy(&packedData[612], &doubleBuffer, 8);

    // vel[1]
    doubleBuffer = 123456.789456;
    memcpy(&packedData[628], &doubleBuffer, 8);

    // now unpack it w/ the generated code...
    Struct_Cannon actualData = { 0 };

    TVS_Unpack_0x01BA(&actualData, packedData);

    // assert that the data was unpacked as expected...
    Struct_Cannon expectedData = {
    
        .rf.delta_time = 1234.56789101,
        .mystring = "the quick brown fox jumps over the lazy dog!",
        .impact = 42.f,
        .rf.lower_set = 3,
        .rf.upper_set = 2,
        .pos[0] = 24.24567,
        .pos[1] = 54321.54321,
        .vel[0] = 987654.54321,
        .vel[1] = 123456.789456
    };

    UtAssert_DoubleCmpRel(actualData.rf.delta_time, expectedData.rf.delta_time, fpTol, "actualData.rf.delta_time check");
    UtAssert_StrCmp(actualData.mystring, expectedData.mystring, "actualData.mystring check");
    UtAssert_DoubleCmpRel(actualData.impact, expectedData.impact, fpTol, "actualData.impact check");
    UtAssert_True(actualData.rf.lower_set == expectedData.rf.lower_set, "actualData.rf.lower_set check");
    UtAssert_True(actualData.rf.upper_set == expectedData.rf.upper_set, "actualData.rf.upper_set check");
    UtAssert_DoubleCmpRel(actualData.pos[0], expectedData.pos[0], fpTol, "actualData.pos[0] check");
    UtAssert_DoubleCmpRel(actualData.pos[1], expectedData.pos[1], fpTol, "actualData.pos[1] check");
    UtAssert_DoubleCmpRel(actualData.vel[0], expectedData.vel[0], fpTol, "actualData.vel[0] check");
    UtAssert_DoubleCmpRel(actualData.vel[1], expectedData.vel[1], fpTol, "actualData.vel[1] check");
}

void TVMC_Test_A_AddTestCases()
{
    UtTest_Add(TVMC_Pack_0x19BA_Test, TVMC_Test_Setup, TVMC_Test_TearDown, "TVMC_Pack_0x19BA_Test");
    UtTest_Add(TVMC_Unpack_0x01BA_Test, TVMC_Test_Setup, TVMC_Test_TearDown, "TVMC_Unpack_0x01BA_Test");
}
