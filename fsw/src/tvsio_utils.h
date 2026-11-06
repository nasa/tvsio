#ifndef _TVSIO_UTILS_H_
#define _TVSIO_UTILS_H_

#include "cfe.h"

float TVS_UnpackFloat(void *buffer);
double TVS_UnpackDouble(void *buffer);
int64 TVS_UnpackSignedInteger(void *buffer, int32 length);
uint64 TVS_UnpackUnsignedInteger(void *buffer, int32 length);

#endif // _TVSIO_UTILS_H_