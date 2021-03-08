
#include "tvs_io_utils.h"
#include <string.h>

double TVS_UnpackDouble(void *buffer)
{
    double value;

    // this avoids alignment issues
    CFE_PSP_MemCpy((void *)&value, buffer, 8);
    return value;
}

float TVS_UnpackFloat(void *buffer)
{
    float value;

    // this avoids alignment issues
    CFE_PSP_MemCpy((void *)&value, buffer, 4);
    return value;
}

int64 TVS_UnpackSignedInteger(void *buffer, int32 length)
{
    if (length == 1)
    {
        int8 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 1);

        return value;
    }
    else if (length == 2)
    {
        int16 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 2);

        return value;
    }
    else if (length == 4)
    {
        int32 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 4);

        return value;
    }
    else if (length == 8)
    {
        int64 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 8);

        return value;
    }

    return -1;
}

uint64 TVS_UnpackUnsignedInteger(void *buffer, int32 length)
{
    if (length == 1)
    {
        uint8 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 1);

        return value;
    }
    else if (length == 2)
    {
        uint16 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 2);

        return value;
    }
    else if (length == 4)
    {
        uint32 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 4);

        return value;
    }
    else if (length == 8)
    {
        uint64 value;
        CFE_PSP_MemCpy((void *)&value, buffer, 8);

        return value;
    }

    return -1;
}
