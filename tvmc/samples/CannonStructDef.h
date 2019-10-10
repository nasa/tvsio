#ifndef __CANNON_STRUCT_DEF_H__
#define __CANNON_STRUCT_DEF_H__

#define STRUCT_CANNON_MID 0x01BA
#define STRUCT_CANNON_SET_VEL_CMD_MID 0x19BA

typedef struct
{
    char commandHeader[CFE_SB_CMD_HDR_SIZE];
    float vel[2];

    char mystring[512];

} Struct_Cannon_SetVelCmd;

typedef struct
{
    char primaryHeader[6];
    char secondaryHeader[6];
    
    int16 lower_set : 4;
    int16 upper_set : 3;
    double delta_time;

} Regula_Falsi;

typedef struct
{
    char primaryHeader[6];
    char secondaryHeader[6];

    Regula_Falsi rf;

    float impact;
    float pos[2];
    float vel[2];

    char mystring[512];

} Struct_Cannon;

#endif