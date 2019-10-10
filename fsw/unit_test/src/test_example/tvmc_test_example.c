#include "tvmc_test.h"

void TVMC_Function1_Test_Case1(void)
{
    int x = 1;

    UtAssert_True(42 == (x + 41), "42 == x + 41");
}

void TVMC_Test_A_AddTestCases()
{
    UtTest_Add(TVMC_Function1_Test_Case1, TVMC_Test_Setup, TVMC_Test_TearDown, "TVMC_Function1_Test_Case1");
}
