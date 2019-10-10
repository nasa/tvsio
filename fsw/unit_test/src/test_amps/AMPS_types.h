/* Created : Tue Jun 05 16:52:10 CDT 2018
   User    : rhirsh
   Project : habitat
   Script  : /gfe/TOOLS/cfs_tools/cfs_cdd/scripts/HabTypesHeaderExtended.js
   Table(s): MBSU1, CCSDS_TLM_HDR, CCSDS_Primary, MBSU2, mPDU1, PDU1, PDU2, PDU3, PDU4 */

#ifndef _AMPS_TYPES_H_
#define _AMPS_TYPES_H_

#include "shared_types.h"
///* @file */


#ifdef __cplusplus
extern "C" {
#endif
/** Structure MBSU1 (188 bytes in size)  *
* AMPS MBSU1 telemetry message
*/
typedef struct
{
  CCSDS_TLM_HDR cfsHeader;                                   ///< [    0] (16 bytes)  Extended CFS SB message Header #CCSDS_TLM_HDR
  uint8_t MBSU_ID;                                           ///< [   16] (1 bytes)  unique ID of component
  uint8_t SOURCESELECT_STATUS_INPUT1:2;                /* */ ///< [   17]   Shows state of input 1 (was MBSU_INPUT_SOURCESELECT_STATUS)
  uint8_t SOURCESELECT_STATUS_INPUT2:2;                      ///< [   17]   Shows state of input 2 (was MBSU_INPUT_SOURCESELECT_STATUS)
  uint8_t SOURCESELECT_STATUS_INPUT3:2;                      ///< [   17]   Shows state of input 3 (was MBSU_INPUT_SOURCESELECT_STATUS)
  uint8_t SOURCESELECT_STATUS_PAD:2;                         ///< [   17]   
  uint8_t FEEDBACK_STATUS_INPUTRELAY1:2;               /* */ ///< [   18]   shows the actual relay state read from feedback contact1 (was MBSU_INPUTRELAY_FEEDBACK_STATUS)
  uint8_t FEEDBACK_STATUS_INPUTRELAY2:2;                     ///< [   18]   shows the actual relay state read from feedback contact2 (was MBSU_INPUTRELAY_FEEDBACK_STATUS)
  uint8_t FEEDBACK_STATUS_INPUTRELAY3:2;                     ///< [   18]   shows the actual relay state read from feedback contact3 (was MBSU_INPUTRELAY_FEEDBACK_STATUS)
  uint8_t FEEDBACK_STATUS_PAD:2;                             ///< [   18]   
  uint8_t MBSU_INPUT_INVALIDBATTERY_ERROR;                   ///< [   19] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting battery
  uint8_t MBSU_INPUT_INVALIDSOLARARRAY_ERROR;                ///< [   20] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting solar array
  uint8_t MBSU_INPUT_INVALIDAUXMBSUINPUT_ERROR;              ///< [   21] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting aux mbsu as input
  uint8_t MBSU_INPUT_INVALIDAUXMBSUOUTPUT_ERROR;             ///< [   22] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting aux mbsu as output
  uint8_t STATEMISMATCH_ERROR_INPUT_RELAY1:2;          /* */ ///< [   23]   0 = ok, 1 = error (was MBSU_INPUT_RELAYSTATEMISMATCH_ERROR)
  uint8_t STATEMISMATCH_ERROR_INPUT_RELAY2:2;                ///< [   23]   0 = ok, 1 = error (was MBSU_INPUT_RELAYSTATEMISMATCH_ERROR)
  uint8_t STATEMISMATCH_ERROR_INPUT_RELAY3:2;                ///< [   23]   0 = ok, 1 = error (was MBSU_INPUT_RELAYSTATEMISMATCH_ERROR)
  uint8_t STATEMISMATCH_ERROR_PAD:2;                         ///< [   23]   
  uint8_t POSNEGSTATEMISMATCH_ERROR_INPUT1:1;          /* */ ///< [   24]   0 = ok, 1 = indicates that pos and neg relay contact states are mismatched (was MBSU_INPUT_POSNEGSTATEMISMATCH_ERROR)
  uint8_t POSNEGSTATEMISMATCH_ERROR_INPUT2:1;                ///< [   24]   0 = ok, 1 = indicates that pos and neg relay contact states are mismatched (was MBSU_INPUT_POSNEGSTATEMISMATCH_ERROR)
  uint8_t POSNEGSTATEMISMATCH_ERROR_INPUT3:1;                ///< [   24]   0 = ok, 1 = indicates that pos and neg relay contact states are mismatched (was MBSU_INPUT_POSNEGSTATEMISMATCH_ERROR)
  uint8_t POSNEGSTATEMISMATCH_ERROR_PAD:5;                   ///< [   24]   
  uint8_t OUTSTATUS_BUS1:2;                            /* */ ///< [   25]   shows state of output register that drives relay1 (was MBSU_BUSOUTSTATUS)
  uint8_t OUTSTATUS_BUS2:2;                                  ///< [   25]   shows state of output register that drives relay2 (was MBSU_BUSOUTSTATUS)
  uint8_t OUTSTATUS_PAD:4;                                   ///< [   25]   
  uint8_t RELAYFEEDBACKSTATUS_BUSOUT1:2;               /* */ ///< [   26]   shows the actual relay state read from feedback contact 1 (was MBSU_BUSOUTERLAYFEEDBACKSTATUS)
  uint8_t RELAYFEEDBACKSTATUS_BUSOUT2:2;                     ///< [   26]   shows the actual relay state read from feedback contact 2 (was MBSU_BUSOUTERLAYFEEDBACKSTATUS)
  uint8_t RELAYFEEDBACK_PAD:4;                               ///< [   26]   
  uint8_t RELAYSTATEMISMATCHERROR_BUSOUT1:2;           /* */ ///< [   27]   0 = ok, 1 = error (was MBSU_BUSOUTRELAYSTATEMISMATCHERROR)
  uint8_t RELAYSTATEMISMATCHERROR_BUSOUT2:2;                 ///< [   27]   0 = ok, 1 = error (was MBSU_BUSOUTRELAYSTATEMISMATCHERROR)
  uint8_t RELAYSTATE_PAD:4;                                  ///< [   27]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN1:1;/* */ ///< [   28]   0 = ok 1 = error
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN2:1;      ///< [   28]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN3:1;      ///< [   28]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN4:1;      ///< [   28]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_PAD:4;        ///< [   28]   
  uint8_t TRIPSTATE_INPUTRELAY1:2;                     /* */ ///< [   29]   relay trip state of input relay1 (was MBSU_INPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_INPUTRELAY2:2;                           ///< [   29]   relay trip state of input relay2 (was MBSU_INPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_INPUTRELAY3:2;                           ///< [   29]   relay trip state of input relay3 (was MBSU_INPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_INPUTRELAY_PAD:2;                        ///< [   29]   
  uint8_t TRIPSTATE_OUTPUTRELAY1:1;                    /* */ ///< [   30]   relay trip state of output relay1 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_OUTPUTRELAY2:1;                          ///< [   30]   relay trip state of output relay2 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_OUTPUTRELAY3:1;                          ///< [   30]   relay trip state of output relay3 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_OUTPUTRELAY4:1;                          ///< [   30]   relay trip state of output relay4 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_PAD:4;                                   ///< [   30]   
  uint8_t MBSU_PAD[5];                                       ///< [   31] (5x1=5 bytes)  GBA - to align on even 8-byte boundary, counting from start of header, puts the first double at byte #32
  double MBSU_SOLAR_ARRAY_INPUT_CURRENT;                     ///< [   36] (8 bytes)  current frmo solar array
  double MBSU_BATTERY_BIDIRECTIONAL_CURRENT;                 ///< [   44] (8 bytes)  current to/from battery
  double MBSU_AUXMBSU_BIDIRECTIONAL_CURRENT;                 ///< [   52] (8 bytes)  current to/from aux port
  double MBSU_AUX_VOLTAGE;                                   ///< [   60] (8 bytes)  voltage of the aux mbsu port
  double MBSU_HKPS_VOLTAGE;                                  ///< [   68] (8 bytes)  voltage of the house keeping power supply
  double MBSU_OUTPUT_CURRENT_1;                              ///< [   76] (8 bytes)  mbsu output current 1
  double MBSU_OUTPUT_CURRENT_2;                              ///< [   84] (8 bytes)  mbsu output current 2
  double MBSU_OUTPUT_CURRENT_3;                              ///< [   92] (8 bytes)  mbsu output current 3
  double MBSU_OUTPUT_CURRENT_4;                              ///< [  100] (8 bytes)  mbsu output current 4
  double MBSU_SARVOLTAGE;                                    ///< [  108] (8 bytes)  solar array regulator voltage
  double MBSU_BATTVOLTAGE;                                   ///< [  116] (8 bytes)  battery input voltage
  double MBSU_INNERVOLTAGE;                                  ///< [  124] (8 bytes)  inner bus voltage
  double MBSU_SARCURRTRIPSP;                                 ///< [  132] (8 bytes)  trip setpoint for SAR input
  double MBSU_BATTCURRTRIPSP;                                ///< [  140] (8 bytes)  trip setpoint for battery
  double MBSU_AUXCURRTRIPSP;                                 ///< [  148] (8 bytes)  trip setpoint for aux input
  double MBSU_OUTPUTCURRTRIPSP_1;                            ///< [  156] (8 bytes)  output ch1 trip setpoint
  double MBSU_OUTPUTCURRTRIPSP_2;                            ///< [  164] (8 bytes)  output ch2 trip setpoint
  double MBSU_OUTPUTCURRTRIPSP_3;                            ///< [  172] (8 bytes)  output ch3 trip setpoint
  double MBSU_OUTPUTCURRTRIPSP_4;                            ///< [  180] (8 bytes)  output ch3 trip setpoint
} MBSU1;     ///<  Total size of 188 bytes

/** Structure MBSU2 (188 bytes in size)  *
* AMPS MBSU2 telemetry message
*/
typedef struct
{
  CCSDS_TLM_HDR cfsHeader;                                   ///< [    0] (16 bytes)  Extended CFS SB message Header #CCSDS_TLM_HDR
  uint8_t MBSU_ID;                                           ///< [   16] (1 bytes)  unique ID of component
  uint8_t SOURCESELECT_STATUS_INPUT1:2;                /* */ ///< [   17]   Shows state of input 1 (was MBSU_INPUT_SOURCESELECT_STATUS)
  uint8_t SOURCESELECT_STATUS_INPUT2:2;                      ///< [   17]   Shows state of input 2 (was MBSU_INPUT_SOURCESELECT_STATUS)
  uint8_t SOURCESELECT_STATUS_INPUT3:2;                      ///< [   17]   Shows state of input 3 (was MBSU_INPUT_SOURCESELECT_STATUS)
  uint8_t SOURCESELECT_STATUS_PAD:2;                         ///< [   17]   
  uint8_t FEEDBACK_STATUS_INPUTRELAY1:2;               /* */ ///< [   18]   shows the actual relay state read from feedback contact1 (was MBSU_INPUTRELAY_FEEDBACK_STATUS)
  uint8_t FEEDBACK_STATUS_INPUTRELAY2:2;                     ///< [   18]   shows the actual relay state read from feedback contact2 (was MBSU_INPUTRELAY_FEEDBACK_STATUS)
  uint8_t FEEDBACK_STATUS_INPUTRELAY3:2;                     ///< [   18]   shows the actual relay state read from feedback contact3 (was MBSU_INPUTRELAY_FEEDBACK_STATUS)
  uint8_t FEEDBACK_STATUS_PAD:2;                             ///< [   18]   
  uint8_t MBSU_INPUT_INVALIDBATTERY_ERROR;                   ///< [   19] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting battery
  uint8_t MBSU_INPUT_INVALIDSOLARARRAY_ERROR;                ///< [   20] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting solar array
  uint8_t MBSU_INPUT_INVALIDAUXMBSUINPUT_ERROR;              ///< [   21] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting aux mbsu as input
  uint8_t MBSU_INPUT_INVALIDAUXMBSUOUTPUT_ERROR;             ///< [   22] (1 bytes)  0 = OK, 1 = indicates hat the aux mbsu port is NOT in OFF state when selecting aux mbsu as output
  uint8_t STATEMISMATCH_ERROR_INPUT_RELAY1:2;          /* */ ///< [   23]   0 = ok, 1 = error (was MBSU_INPUT_RELAYSTATEMISMATCH_ERROR)
  uint8_t STATEMISMATCH_ERROR_INPUT_RELAY2:2;                ///< [   23]   0 = ok, 1 = error (was MBSU_INPUT_RELAYSTATEMISMATCH_ERROR)
  uint8_t STATEMISMATCH_ERROR_INPUT_RELAY3:2;                ///< [   23]   0 = ok, 1 = error (was MBSU_INPUT_RELAYSTATEMISMATCH_ERROR)
  uint8_t STATEMISMATCH_ERROR_PAD:2;                         ///< [   23]   
  uint8_t POSNEGSTATEMISMATCH_ERROR_INPUT1:1;          /* */ ///< [   24]   0 = ok, 1 = indicates that pos and neg relay contact states are mismatched (was MBSU_INPUT_POSNEGSTATEMISMATCH_ERROR)
  uint8_t POSNEGSTATEMISMATCH_ERROR_INPUT2:1;                ///< [   24]   0 = ok, 1 = indicates that pos and neg relay contact states are mismatched (was MBSU_INPUT_POSNEGSTATEMISMATCH_ERROR)
  uint8_t POSNEGSTATEMISMATCH_ERROR_INPUT3:1;                ///< [   24]   0 = ok, 1 = indicates that pos and neg relay contact states are mismatched (was MBSU_INPUT_POSNEGSTATEMISMATCH_ERROR)
  uint8_t POSNEGSTATEMISMATCH_ERROR_PAD:5;                   ///< [   24]   
  uint8_t OUTSTATUS_BUS1:2;                            /* */ ///< [   25]   shows state of output register that drives relay1 (was MBSU_BUSOUTSTATUS)
  uint8_t OUTSTATUS_BUS2:2;                                  ///< [   25]   shows state of output register that drives relay2 (was MBSU_BUSOUTSTATUS)
  uint8_t OUTSTATUS_PAD:4;                                   ///< [   25]   
  uint8_t RELAYFEEDBACKSTATUS_BUSOUT1:2;               /* */ ///< [   26]   shows the actual relay state read from feedback contact 1 (was MBSU_BUSOUTERLAYFEEDBACKSTATUS)
  uint8_t RELAYFEEDBACKSTATUS_BUSOUT2:2;                     ///< [   26]   shows the actual relay state read from feedback contact 2 (was MBSU_BUSOUTERLAYFEEDBACKSTATUS)
  uint8_t RELAYFEEDBACK_PAD:4;                               ///< [   26]   
  uint8_t RELAYSTATEMISMATCHERROR_BUSOUT1:2;           /* */ ///< [   27]   0 = ok, 1 = error (was MBSU_BUSOUTRELAYSTATEMISMATCHERROR)
  uint8_t RELAYSTATEMISMATCHERROR_BUSOUT2:2;                 ///< [   27]   0 = ok, 1 = error (was MBSU_BUSOUTRELAYSTATEMISMATCHERROR)
  uint8_t RELAYSTATE_PAD:4;                                  ///< [   27]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN1:1;/* */ ///< [   28]   0 = ok 1 = error
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN2:1;      ///< [   28]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN3:1;      ///< [   28]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_CHAN4:1;      ///< [   28]   
  uint8_t MBSU_BUSOUTPPOSNEGSTATEMISMATCHERROR_PAD:4;        ///< [   28]   
  uint8_t TRIPSTATE_INPUTRELAY1:2;                     /* */ ///< [   29]   relay trip state of input relay1 (was MBSU_INPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_INPUTRELAY2:2;                           ///< [   29]   relay trip state of input relay2 (was MBSU_INPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_INPUTRELAY3:2;                           ///< [   29]   relay trip state of input relay3 (was MBSU_INPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_INPUTRELAY_PAD:2;                        ///< [   29]   
  uint8_t TRIPSTATE_OUTPUTRELAY1:1;                    /* */ ///< [   30]   relay trip state of output relay1 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_OUTPUTRELAY2:1;                          ///< [   30]   relay trip state of output relay2 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_OUTPUTRELAY3:1;                          ///< [   30]   relay trip state of output relay3 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_OUTPUTRELAY4:1;                          ///< [   30]   relay trip state of output relay4 (was MBSU_OUTPUTRELAYTRIPSTATE)
  uint8_t TRIPSTATE_PAD:4;                                   ///< [   30]   
  uint8_t MBSU_PAD[5];                                       ///< [   31] (5x1=5 bytes)  GBA - to align on even 8-byte boundary, counting from start of header, puts the first double at byte #32
  double MBSU_SOLAR_ARRAY_INPUT_CURRENT;                     ///< [   36] (8 bytes)  current frmo solar array
  double MBSU_BATTERY_BIDIRECTIONAL_CURRENT;                 ///< [   44] (8 bytes)  current to/from battery
  double MBSU_AUXMBSU_BIDIRECTIONAL_CURRENT;                 ///< [   52] (8 bytes)  current to/from aux port
  double MBSU_AUX_VOLTAGE;                                   ///< [   60] (8 bytes)  voltage of the aux mbsu port
  double MBSU_HKPS_VOLTAGE;                                  ///< [   68] (8 bytes)  voltage of the house keeping power supply
  double MBSU_OUTPUT_CURRENT_1;                              ///< [   76] (8 bytes)  mbsu output current 1
  double MBSU_OUTPUT_CURRENT_2;                              ///< [   84] (8 bytes)  mbsu output current 2
  double MBSU_OUTPUT_CURRENT_3;                              ///< [   92] (8 bytes)  mbsu output current 3
  double MBSU_OUTPUT_CURRENT_4;                              ///< [  100] (8 bytes)  mbsu output current 4
  double MBSU_SARVOLTAGE;                                    ///< [  108] (8 bytes)  solar array regulator voltage
  double MBSU_BATTVOLTAGE;                                   ///< [  116] (8 bytes)  battery input voltage
  double MBSU_INNERVOLTAGE;                                  ///< [  124] (8 bytes)  inner bus voltage
  double MBSU_SARCURRTRIPSP;                                 ///< [  132] (8 bytes)  trip setpoint for SAR input
  double MBSU_BATTCURRTRIPSP;                                ///< [  140] (8 bytes)  trip setpoint for battery
  double MBSU_AUXCURRTRIPSP;                                 ///< [  148] (8 bytes)  trip setpoint for aux input
  double MBSU_OUTPUTCURRTRIPSP_1;                            ///< [  156] (8 bytes)  output ch1 trip setpoint
  double MBSU_OUTPUTCURRTRIPSP_2;                            ///< [  164] (8 bytes)  output ch2 trip setpoint
  double MBSU_OUTPUTCURRTRIPSP_3;                            ///< [  172] (8 bytes)  output ch3 trip setpoint
  double MBSU_OUTPUTCURRTRIPSP_4;                            ///< [  180] (8 bytes)  output ch3 trip setpoint
} MBSU2;     ///<  Total size of 188 bytes

/** Structure mPDU1 (69 bytes in size)  *
* AMPS modular PDU 1 telemetry message
*/
typedef struct
{
  CCSDS_TLM_HDR cfsHeader;                                   ///< [    0] (16 bytes)  Extended CFS SB message Header #CCSDS_TLM_HDR
  uint8_t UID;                                               ///< [   16] (1 bytes)  Globally unique identifier
  uint16_t TLM_VALID_FLAGS;                                  ///< [   17] (2 bytes)  15 bit-to-slot-mapped telemetry valid flags
  uint8_t INPUT1_STATUS;                                     ///< [   19] (1 bytes)  Slot #5: Bus Switchgear Module #1 Status
  int8_t INPUT1_CURRENT_HIGH;                                ///< [   20] (1 bytes)  Slot #5: Bus Switchgear Module #1 High-side Current
  int8_t INPUT1_CURRENT_LOW;                                 ///< [   21] (1 bytes)  Slot #5: Bus Switchgear Module #1 Low-side Current
  uint8_t INPUT1_VOLTAGE_EXTERNAL;                           ///< [   22] (1 bytes)  Slot #5: Bus Switchgear Module #1 External Bus Voltage
  uint8_t INPUT1_VOLTAGE_INTERNAL;                           ///< [   23] (1 bytes)  Slot #5: Bus Switchgear Module #1 Internal Bus Voltage
  uint8_t INPUT2_STATUS;                                     ///< [   24] (1 bytes)  Slot #6: Bus Switchgear Module #2 Status
  int8_t INPUT2_CURRENT_HIGH;                                ///< [   25] (1 bytes)  Slot #6: Bus Switchgear Module #2 High-side Current
  int8_t INPUT2_CURRENT_LOW;                                 ///< [   26] (1 bytes)  Slot #6: Bus Switchgear Module #2 Low-side Current
  uint8_t INPUT2_VOLTAGE_EXTERNAL;                           ///< [   27] (1 bytes)  Slot #6: Bus Switchgear Module #2 External Bus Voltage
  uint8_t INPUT2_VOLTAGE_INTERNAL;                           ///< [   28] (1 bytes)  Slot #6: Bus Switchgear Module #2 Internal Bus Voltage
  uint8_t OUTPUT1_STATUS;                                    ///< [   29] (1 bytes)  Slot #8: Load Switchgear Module #1 Status
  uint8_t OUTPUT1_CURRENT_A;                                 ///< [   30] (1 bytes)  Slot #8: Load Switchgear Module #1 Channel A Current
  uint8_t OUTPUT1_CURRENT_B;                                 ///< [   31] (1 bytes)  Slot #8: Load Switchgear Module #1 Channel B Current
  uint8_t OUTPUT1_CURRENT_C;                                 ///< [   32] (1 bytes)  Slot #8: Load Switchgear Module #1 Channel C Current
  uint8_t OUTPUT1_CURRENT_D;                                 ///< [   33] (1 bytes)  Slot #8: Load Switchgear Module #1 Channel D Current
  uint8_t OUTPUT2_STATUS;                                    ///< [   34] (1 bytes)  Slot #9: Load Switchgear Module #1 Status
  uint8_t OUTPUT2_CURRENT_A;                                 ///< [   35] (1 bytes)  Slot #9: Load Switchgear Module #2 Channel A Current
  uint8_t OUTPUT2_CURRENT_B;                                 ///< [   36] (1 bytes)  Slot #9: Load Switchgear Module #2 Channel B Current
  uint8_t OUTPUT2_CURRENT_C;                                 ///< [   37] (1 bytes)  Slot #9: Load Switchgear Module #2 Channel C Current
  uint8_t OUTPUT2_CURRENT_D;                                 ///< [   38] (1 bytes)  Slot #9: Load Switchgear Module #2 Channel D Current
  uint8_t OUTPUT3_STATUS;                                    ///< [   39] (1 bytes)  Slot #9: Load Switchgear Module #3 Status
  uint8_t OUTPUT3_CURRENT_A;                                 ///< [   40] (1 bytes)  Slot #10: Load Switchgear Module #3 Channel A Current
  uint8_t OUTPUT3_CURRENT_B;                                 ///< [   41] (1 bytes)  Slot #10: Load Switchgear Module #3 Channel B Current
  uint8_t OUTPUT3_CURRENT_C;                                 ///< [   42] (1 bytes)  Slot #10: Load Switchgear Module #3 Channel C Current
  uint8_t OUTPUT3_CURRENT_D;                                 ///< [   43] (1 bytes)  Slot #10: Load Switchgear Module #3 Channel D Current
  uint8_t OUTPUT4_STATUS;                                    ///< [   44] (1 bytes)  Slot #11: Load Switchgear Module #4 Status
  uint8_t OUTPUT4_CURRENT_A;                                 ///< [   45] (1 bytes)  Slot #11: Load Switchgear Module #4 Channel A Current
  uint8_t OUTPUT4_CURRENT_B;                                 ///< [   46] (1 bytes)  Slot #11: Load Switchgear Module #4 Channel B Current
  uint8_t OUTPUT4_CURRENT_C;                                 ///< [   47] (1 bytes)  Slot #11: Load Switchgear Module #4 Channel C Current
  uint8_t OUTPUT4_CURRENT_D;                                 ///< [   48] (1 bytes)  Slot #11: Load Switchgear Module #4 Channel D Current
  uint8_t OUTPUT5_STATUS;                                    ///< [   49] (1 bytes)  Slot #12: Load Switchgear Module #5 Status
  uint8_t OUTPUT5_CURRENT_A;                                 ///< [   50] (1 bytes)  Slot #12: Load Switchgear Module #5 Channel A Current
  uint8_t OUTPUT5_CURRENT_B;                                 ///< [   51] (1 bytes)  Slot #12: Load Switchgear Module #5 Channel B Current
  uint8_t OUTPUT5_CURRENT_C;                                 ///< [   52] (1 bytes)  Slot #12: Load Switchgear Module #5 Channel C Current
  uint8_t OUTPUT5_CURRENT_D;                                 ///< [   53] (1 bytes)  Slot #12: Load Switchgear Module #5 Channel D Current
  uint8_t OUTPUT6_STATUS;                                    ///< [   54] (1 bytes)  Slot #13: Load Switchgear Module #6 Status
  uint8_t OUTPUT6_CURRENT_A;                                 ///< [   55] (1 bytes)  Slot #13: Load Switchgear Module #6 Channel A Current
  uint8_t OUTPUT6_CURRENT_B;                                 ///< [   56] (1 bytes)  Slot #13: Load Switchgear Module #6 Channel B Current
  uint8_t OUTPUT6_CURRENT_C;                                 ///< [   57] (1 bytes)  Slot #13: Load Switchgear Module #6 Channel C Current
  uint8_t OUTPUT6_CURRENT_D;                                 ///< [   58] (1 bytes)  Slot #13: Load Switchgear Module #6 Channel D Current
  uint8_t OUTPUT7_STATUS;                                    ///< [   59] (1 bytes)  Slot #14: Load Switchgear Module #7 Status
  uint8_t OUTPUT7_CURRENT_A;                                 ///< [   60] (1 bytes)  Slot #14: Load Switchgear Module #7 Channel A Current
  uint8_t OUTPUT7_CURRENT_B;                                 ///< [   61] (1 bytes)  Slot #14: Load Switchgear Module #7 Channel B Current
  uint8_t OUTPUT7_CURRENT_C;                                 ///< [   62] (1 bytes)  Slot #14: Load Switchgear Module #7 Channel C Current
  uint8_t OUTPUT7_CURRENT_D;                                 ///< [   63] (1 bytes)  Slot #14: Load Switchgear Module #7 Channel D Current
  uint8_t OUTPUT8_STATUS;                                    ///< [   64] (1 bytes)  Slot #15: Load Switchgear Module #8 Status
  uint8_t OUTPUT8_CURRENT_A;                                 ///< [   65] (1 bytes)  Slot #15: Load Switchgear Module #8 Channel A Current
  uint8_t OUTPUT8_CURRENT_B;                                 ///< [   66] (1 bytes)  Slot #15: Load Switchgear Module #8 Channel B Current
  uint8_t OUTPUT8_CURRENT_C;                                 ///< [   67] (1 bytes)  Slot #15: Load Switchgear Module #8 Channel C Current
  uint8_t OUTPUT8_CURRENT_D;                                 ///< [   68] (1 bytes)  Slot #15: Load Switchgear Module #8 Channel D Current
} mPDU1;     ///<  Total size of 69 bytes

/** Structure PDU1 (148 bytes in size)  *
* AMPS PDU 1 telemetry message
*/
typedef struct
{
  CCSDS_TLM_HDR cfsHeader;                                   ///< [    0] (16 bytes)  Extended CFS SB message Header #CCSDS_TLM_HDR
  uint8_t PDU_UNITID;                                        ///< [   16] (1 bytes)  unique ID of the component
  uint8_t STATUS_INPUTBUS1:2;                          /* */ ///< [   17]   shows state of output register that drives relays - input 1 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTBUS2:2;                                ///< [   17]   shows state of output register that drives relays - input 2 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTPAD:4;                                 ///< [   17]   
  uint8_t FEEDBACK_INPUTRELAY1:2;                      /* */ ///< [   18]   shows the actual relay state read from feedback contacts - input 1 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUTRELAY2:2;                            ///< [   18]   shows the actual relay state read from feedback contacts - input 2 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUT_PAD:4;                              ///< [   18]   
  uint8_t PDU_INVALIDCOMMANDERROR;                           ///< [   19] (1 bytes)  0= OK, 1 = Error (indicates that more than one input has been selected)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS1:2;      /* */ ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS2:2;            ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_PAD:4;                  ///< [   20]   
  uint8_t POSNEGMISMATCHERROR_INPUT1:1;                /* */ ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_INPUT2:1;                      ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_PAD:6;                         ///< [   21]   
  uint8_t CHANNELSTATUS_RPC1:1;                        /* */ ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 1 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC2:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 2 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC3:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 3 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC4:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 4 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC5:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 5 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC6:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 6 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC7:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 7 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC8:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 8 (was PDU_RPCCHANNELSTATUS)
  uint8_t TRIPSTATUS_RPC1:1;                           /* */ ///< [   23]   0= OK 1= TRIPPED Indicates that channel 1 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC2:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 2 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC3:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 3 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC4:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 4 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC5:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 5 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC6:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 6 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC7:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 7 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC8:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 8 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t OUTSTATUS_RPC1:1;                            /* */ ///< [   24]   the last commanded state of the RPC1 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC2:1;                                  ///< [   24]   the last commanded state of the RPC2 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC3:1;                                  ///< [   24]   the last commanded state of the RPC3 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC4:1;                                  ///< [   24]   the last commanded state of the RPC4 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC5:1;                                  ///< [   24]   the last commanded state of the RPC5 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC6:1;                                  ///< [   24]   the last commanded state of the RPC6 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC7:1;                                  ///< [   24]   the last commanded state of the RPC7 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC8:1;                                  ///< [   24]   the last commanded state of the RPC8 (was PDU_RPCOUTSTATUS)
  uint8_t MISMATCHERROR_RPC1:1;                        /* */ ///< [   25]   mismatch of last commanded state and measured state of RPC1 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC2:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC2 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC3:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC3 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC4:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC4 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC5:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC5 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC6:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC6 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC7:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC7 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC8:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC8 (was PDU_RPCMISMATCHERROR)
  uint8_t PDU_PAD[2];                                        ///< [   26] (2x1=2 bytes)  GBA - padding to 8-byte boundary for first double at byte 24 from start of header
  double PDU_CURRENTBUS_1;                                   ///< [   28] (8 bytes)  CurrentBus1: Main Bus 1 input currents
  double PDU_CURRENTBUS_2;                                   ///< [   36] (8 bytes)  CurrentBus2: Main Bus 2 input currents
  double PDU_CURRENTINTERNALBUS;                             ///< [   44] (8 bytes)  CurrentInternalBus: Current inside the PDU upstream of RPCs
  double PDU_VOLTAGEBUS_1;                                   ///< [   52] (8 bytes)  VoltageBus1: Main bus voltage at input 1
  double PDU_VOLTAGEBUS_2;                                   ///< [   60] (8 bytes)  VoltageBus2: Main bus voltage at input 2
  double PDU_VOLTAGEINTERNAL;                                ///< [   68] (8 bytes)  VoltageInternal: Voltage inside the PDU upstream of RPCs
  double PDU_VOLTAGEHKPG;                                    ///< [   76] (8 bytes)  VoltageHKPG: Voltage of the power supply for the internal controller
  double PDU_CURRENTRPC_1;                                   ///< [   84] (8 bytes)  CurrentRPC1: Indicates current of each RPC channel
  double PDU_CURRENTRPC_2;                                   ///< [   92] (8 bytes)  CurrentRPC2: Indicates current of each RPC channel
  double PDU_CURRENTRPC_3;                                   ///< [  100] (8 bytes)  CurrentRPC3: Indicates current of each RPC channel
  double PDU_CURRENTRPC_4;                                   ///< [  108] (8 bytes)  CurrentRPC4: Indicates current of each RPC channel
  double PDU_CURRENTRPC_5;                                   ///< [  116] (8 bytes)  CurrentRPC5: Indicates current of each RPC channel
  double PDU_CURRENTRPC_6;                                   ///< [  124] (8 bytes)  CurrentRPC6: Indicates current of each RPC channel
  double PDU_CURRENTRPC_7;                                   ///< [  132] (8 bytes)  CurrentRPC7: Indicates current of each RPC channel
  double PDU_CURRENTRPC_8;                                   ///< [  140] (8 bytes)  CurrentRPC8: Indicates current of each RPC channel
} PDU1;     ///<  Total size of 148 bytes

/** Structure PDU2 (148 bytes in size)  *
* AMPS PDU 2 telemetry message
*/
typedef struct
{
  CCSDS_TLM_HDR cfsHeader;                                   ///< [    0] (16 bytes)  Extended CFS SB message Header #CCSDS_TLM_HDR
  uint8_t PDU_UNITID;                                        ///< [   16] (1 bytes)  unique ID of the component
  uint8_t STATUS_INPUTBUS1:2;                          /* */ ///< [   17]   shows state of output register that drives relays - input 1 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTBUS2:2;                                ///< [   17]   shows state of output register that drives relays - input 2 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTPAD:4;                                 ///< [   17]   
  uint8_t FEEDBACK_INPUTRELAY1:2;                      /* */ ///< [   18]   shows the actual relay state read from feedback contacts - input 1 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUTRELAY2:2;                            ///< [   18]   shows the actual relay state read from feedback contacts - input 2 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUT_PAD:4;                              ///< [   18]   
  uint8_t PDU_INVALIDCOMMANDERROR;                           ///< [   19] (1 bytes)  0= OK, 1 = Error (indicates that more than one input has been selected)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS1:2;      /* */ ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS2:2;            ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_PAD:4;                  ///< [   20]   
  uint8_t POSNEGMISMATCHERROR_INPUT1:1;                /* */ ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_INPUT2:1;                      ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_PAD:6;                         ///< [   21]   
  uint8_t CHANNELSTATUS_RPC1:1;                        /* */ ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 1 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC2:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 2 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC3:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 3 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC4:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 4 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC5:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 5 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC6:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 6 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC7:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 7 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC8:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 8 (was PDU_RPCCHANNELSTATUS)
  uint8_t TRIPSTATUS_RPC1:1;                           /* */ ///< [   23]   0= OK 1= TRIPPED Indicates that channel 1 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC2:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 2 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC3:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 3 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC4:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 4 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC5:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 5 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC6:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 6 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC7:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 7 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC8:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 8 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t OUTSTATUS_RPC1:1;                            /* */ ///< [   24]   the last commanded state of the RPC1 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC2:1;                                  ///< [   24]   the last commanded state of the RPC2 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC3:1;                                  ///< [   24]   the last commanded state of the RPC3 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC4:1;                                  ///< [   24]   the last commanded state of the RPC4 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC5:1;                                  ///< [   24]   the last commanded state of the RPC5 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC6:1;                                  ///< [   24]   the last commanded state of the RPC6 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC7:1;                                  ///< [   24]   the last commanded state of the RPC7 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC8:1;                                  ///< [   24]   the last commanded state of the RPC8 (was PDU_RPCOUTSTATUS)
  uint8_t MISMATCHERROR_RPC1:1;                        /* */ ///< [   25]   mismatch of last commanded state and measured state of RPC1 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC2:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC2 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC3:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC3 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC4:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC4 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC5:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC5 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC6:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC6 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC7:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC7 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC8:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC8 (was PDU_RPCMISMATCHERROR)
  uint8_t PDU_PAD[2];                                        ///< [   26] (2x1=2 bytes)  GBA - padding to 8-byte boundary for first double at byte 24 from start of header
  double PDU_CURRENTBUS_1;                                   ///< [   28] (8 bytes)  CurrentBus1: Main Bus 1 input currents
  double PDU_CURRENTBUS_2;                                   ///< [   36] (8 bytes)  CurrentBus2: Main Bus 2 input currents
  double PDU_CURRENTINTERNALBUS;                             ///< [   44] (8 bytes)  CurrentInternalBus: Current inside the PDU upstream of RPCs
  double PDU_VOLTAGEBUS_1;                                   ///< [   52] (8 bytes)  VoltageBus1: Main bus voltage at input 1
  double PDU_VOLTAGEBUS_2;                                   ///< [   60] (8 bytes)  VoltageBus2: Main bus voltage at input 2
  double PDU_VOLTAGEINTERNAL;                                ///< [   68] (8 bytes)  VoltageInternal: Voltage inside the PDU upstream of RPCs
  double PDU_VOLTAGEHKPG;                                    ///< [   76] (8 bytes)  VoltageHKPG: Voltage of the power supply for the internal controller
  double PDU_CURRENTRPC_1;                                   ///< [   84] (8 bytes)  CurrentRPC1: Indicates current of each RPC channel
  double PDU_CURRENTRPC_2;                                   ///< [   92] (8 bytes)  CurrentRPC2: Indicates current of each RPC channel
  double PDU_CURRENTRPC_3;                                   ///< [  100] (8 bytes)  CurrentRPC3: Indicates current of each RPC channel
  double PDU_CURRENTRPC_4;                                   ///< [  108] (8 bytes)  CurrentRPC4: Indicates current of each RPC channel
  double PDU_CURRENTRPC_5;                                   ///< [  116] (8 bytes)  CurrentRPC5: Indicates current of each RPC channel
  double PDU_CURRENTRPC_6;                                   ///< [  124] (8 bytes)  CurrentRPC6: Indicates current of each RPC channel
  double PDU_CURRENTRPC_7;                                   ///< [  132] (8 bytes)  CurrentRPC7: Indicates current of each RPC channel
  double PDU_CURRENTRPC_8;                                   ///< [  140] (8 bytes)  CurrentRPC8: Indicates current of each RPC channel
} PDU2;     ///<  Total size of 148 bytes

/** Structure PDU3 (148 bytes in size)  *
* AMPS PDU 3 telemetry message
*/
typedef struct
{
  CCSDS_TLM_HDR cfsHeader;                                   ///< [    0] (16 bytes)  Extended CFS SB message Header #CCSDS_TLM_HDR
  uint8_t PDU_UNITID;                                        ///< [   16] (1 bytes)  unique ID of the component
  uint8_t STATUS_INPUTBUS1:2;                          /* */ ///< [   17]   shows state of output register that drives relays - input 1 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTBUS2:2;                                ///< [   17]   shows state of output register that drives relays - input 2 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTPAD:4;                                 ///< [   17]   
  uint8_t FEEDBACK_INPUTRELAY1:2;                      /* */ ///< [   18]   shows the actual relay state read from feedback contacts - input 1 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUTRELAY2:2;                            ///< [   18]   shows the actual relay state read from feedback contacts - input 2 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUT_PAD:4;                              ///< [   18]   
  uint8_t PDU_INVALIDCOMMANDERROR;                           ///< [   19] (1 bytes)  0= OK, 1 = Error (indicates that more than one input has been selected)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS1:2;      /* */ ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS2:2;            ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_PAD:4;                  ///< [   20]   
  uint8_t POSNEGMISMATCHERROR_INPUT1:1;                /* */ ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_INPUT2:1;                      ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_PAD:6;                         ///< [   21]   
  uint8_t CHANNELSTATUS_RPC1:1;                        /* */ ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 1 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC2:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 2 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC3:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 3 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC4:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 4 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC5:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 5 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC6:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 6 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC7:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 7 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC8:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 8 (was PDU_RPCCHANNELSTATUS)
  uint8_t TRIPSTATUS_RPC1:1;                           /* */ ///< [   23]   0= OK 1= TRIPPED Indicates that channel 1 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC2:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 2 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC3:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 3 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC4:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 4 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC5:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 5 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC6:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 6 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC7:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 7 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC8:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 8 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t OUTSTATUS_RPC1:1;                            /* */ ///< [   24]   the last commanded state of the RPC1 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC2:1;                                  ///< [   24]   the last commanded state of the RPC2 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC3:1;                                  ///< [   24]   the last commanded state of the RPC3 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC4:1;                                  ///< [   24]   the last commanded state of the RPC4 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC5:1;                                  ///< [   24]   the last commanded state of the RPC5 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC6:1;                                  ///< [   24]   the last commanded state of the RPC6 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC7:1;                                  ///< [   24]   the last commanded state of the RPC7 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC8:1;                                  ///< [   24]   the last commanded state of the RPC8 (was PDU_RPCOUTSTATUS)
  uint8_t MISMATCHERROR_RPC1:1;                        /* */ ///< [   25]   mismatch of last commanded state and measured state of RPC1 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC2:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC2 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC3:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC3 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC4:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC4 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC5:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC5 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC6:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC6 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC7:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC7 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC8:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC8 (was PDU_RPCMISMATCHERROR)
  uint8_t PDU_PAD[2];                                        ///< [   26] (2x1=2 bytes)  GBA - padding to 8-byte boundary for first double at byte 24 from start of header
  double PDU_CURRENTBUS_1;                                   ///< [   28] (8 bytes)  CurrentBus1: Main Bus 1 input currents
  double PDU_CURRENTBUS_2;                                   ///< [   36] (8 bytes)  CurrentBus2: Main Bus 2 input currents
  double PDU_CURRENTINTERNALBUS;                             ///< [   44] (8 bytes)  CurrentInternalBus: Current inside the PDU upstream of RPCs
  double PDU_VOLTAGEBUS_1;                                   ///< [   52] (8 bytes)  VoltageBus1: Main bus voltage at input 1
  double PDU_VOLTAGEBUS_2;                                   ///< [   60] (8 bytes)  VoltageBus2: Main bus voltage at input 2
  double PDU_VOLTAGEINTERNAL;                                ///< [   68] (8 bytes)  VoltageInternal: Voltage inside the PDU upstream of RPCs
  double PDU_VOLTAGEHKPG;                                    ///< [   76] (8 bytes)  VoltageHKPG: Voltage of the power supply for the internal controller
  double PDU_CURRENTRPC_1;                                   ///< [   84] (8 bytes)  CurrentRPC1: Indicates current of each RPC channel
  double PDU_CURRENTRPC_2;                                   ///< [   92] (8 bytes)  CurrentRPC2: Indicates current of each RPC channel
  double PDU_CURRENTRPC_3;                                   ///< [  100] (8 bytes)  CurrentRPC3: Indicates current of each RPC channel
  double PDU_CURRENTRPC_4;                                   ///< [  108] (8 bytes)  CurrentRPC4: Indicates current of each RPC channel
  double PDU_CURRENTRPC_5;                                   ///< [  116] (8 bytes)  CurrentRPC5: Indicates current of each RPC channel
  double PDU_CURRENTRPC_6;                                   ///< [  124] (8 bytes)  CurrentRPC6: Indicates current of each RPC channel
  double PDU_CURRENTRPC_7;                                   ///< [  132] (8 bytes)  CurrentRPC7: Indicates current of each RPC channel
  double PDU_CURRENTRPC_8;                                   ///< [  140] (8 bytes)  CurrentRPC8: Indicates current of each RPC channel
} PDU3;     ///<  Total size of 148 bytes

/** Structure PDU4 (148 bytes in size)  *
* AMPS PDU 4 telemetry message
*/
typedef struct
{
  CCSDS_TLM_HDR cfsHeader;                                   ///< [    0] (16 bytes)  Extended CFS SB message Header #CCSDS_TLM_HDR
  uint8_t PDU_UNITID;                                        ///< [   16] (1 bytes)  unique ID of the component
  uint8_t STATUS_INPUTBUS1:2;                          /* */ ///< [   17]   shows state of output register that drives relays - input 1 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTBUS2:2;                                ///< [   17]   shows state of output register that drives relays - input 2 (was PDU_INPUTBUSSTATUS)
  uint8_t STATUS_INPUTPAD:4;                                 ///< [   17]   
  uint8_t FEEDBACK_INPUTRELAY1:2;                      /* */ ///< [   18]   shows the actual relay state read from feedback contacts - input 1 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUTRELAY2:2;                            ///< [   18]   shows the actual relay state read from feedback contacts - input 2 (was PDU_INPUTRELAYFEEDBACK)
  uint8_t FEEDBACK_INPUT_PAD:4;                              ///< [   18]   
  uint8_t PDU_INVALIDCOMMANDERROR;                           ///< [   19] (1 bytes)  0= OK, 1 = Error (indicates that more than one input has been selected)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS1:2;      /* */ ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_INPUTBUS2:2;            ///< [   20]   0=OK, 1 = Error (Indicates the On/Off patterns of status register and feedback contact do not agree (possible relay or relay driver fault) (was PDU_INPUTFEEDBACKSTATEMISMATCHERROR)
  uint8_t FEEDBACKSTATEMISMATCHERROR_PAD:4;                  ///< [   20]   
  uint8_t POSNEGMISMATCHERROR_INPUT1:1;                /* */ ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_INPUT2:1;                      ///< [   21]   0=OK, 1 = Error (Indicates that Positive and Negative relays are not in same state (PDU_POSNEGMISMATCHERROR)
  uint8_t POSNEGMISMATCHERROR_PAD:6;                         ///< [   21]   
  uint8_t CHANNELSTATUS_RPC1:1;                        /* */ ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 1 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC2:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 2 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC3:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 3 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC4:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 4 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC5:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 5 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC6:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 6 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC7:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 7 (was PDU_RPCCHANNELSTATUS)
  uint8_t CHANNELSTATUS_RPC8:1;                              ///< [   22]   0= OFF 1= ON Indicates state of RPC device for channel 8 (was PDU_RPCCHANNELSTATUS)
  uint8_t TRIPSTATUS_RPC1:1;                           /* */ ///< [   23]   0= OK 1= TRIPPED Indicates that channel 1 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC2:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 2 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC3:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 3 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC4:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 4 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC5:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 5 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC6:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 6 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC7:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 7 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t TRIPSTATUS_RPC8:1;                                 ///< [   23]   0= OK 1= TRIPPED Indicates that channel 8 has tripped (was PDU_RPCTRIPSTATUS)
  uint8_t OUTSTATUS_RPC1:1;                            /* */ ///< [   24]   the last commanded state of the RPC1 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC2:1;                                  ///< [   24]   the last commanded state of the RPC2 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC3:1;                                  ///< [   24]   the last commanded state of the RPC3 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC4:1;                                  ///< [   24]   the last commanded state of the RPC4 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC5:1;                                  ///< [   24]   the last commanded state of the RPC5 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC6:1;                                  ///< [   24]   the last commanded state of the RPC6 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC7:1;                                  ///< [   24]   the last commanded state of the RPC7 (was PDU_RPCOUTSTATUS)
  uint8_t OUTSTATUS_RPC8:1;                                  ///< [   24]   the last commanded state of the RPC8 (was PDU_RPCOUTSTATUS)
  uint8_t MISMATCHERROR_RPC1:1;                        /* */ ///< [   25]   mismatch of last commanded state and measured state of RPC1 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC2:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC2 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC3:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC3 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC4:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC4 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC5:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC5 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC6:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC6 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC7:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC7 (was PDU_RPCMISMATCHERROR)
  uint8_t MISMATCHERROR_RPC8:1;                              ///< [   25]   mismatch of last commanded state and measured state of RPC8 (was PDU_RPCMISMATCHERROR)
  uint8_t PDU_PAD[2];                                        ///< [   26] (2x1=2 bytes)  GBA - padding to 8-byte boundary for first double at byte 24 from start of header
  double PDU_CURRENTBUS_1;                                   ///< [   28] (8 bytes)  CurrentBus1: Main Bus 1 input currents
  double PDU_CURRENTBUS_2;                                   ///< [   36] (8 bytes)  CurrentBus2: Main Bus 2 input currents
  double PDU_CURRENTINTERNALBUS;                             ///< [   44] (8 bytes)  CurrentInternalBus: Current inside the PDU upstream of RPCs
  double PDU_VOLTAGEBUS_1;                                   ///< [   52] (8 bytes)  VoltageBus1: Main bus voltage at input 1
  double PDU_VOLTAGEBUS_2;                                   ///< [   60] (8 bytes)  VoltageBus2: Main bus voltage at input 2
  double PDU_VOLTAGEINTERNAL;                                ///< [   68] (8 bytes)  VoltageInternal: Voltage inside the PDU upstream of RPCs
  double PDU_VOLTAGEHKPG;                                    ///< [   76] (8 bytes)  VoltageHKPG: Voltage of the power supply for the internal controller
  double PDU_CURRENTRPC_1;                                   ///< [   84] (8 bytes)  CurrentRPC1: Indicates current of each RPC channel
  double PDU_CURRENTRPC_2;                                   ///< [   92] (8 bytes)  CurrentRPC2: Indicates current of each RPC channel
  double PDU_CURRENTRPC_3;                                   ///< [  100] (8 bytes)  CurrentRPC3: Indicates current of each RPC channel
  double PDU_CURRENTRPC_4;                                   ///< [  108] (8 bytes)  CurrentRPC4: Indicates current of each RPC channel
  double PDU_CURRENTRPC_5;                                   ///< [  116] (8 bytes)  CurrentRPC5: Indicates current of each RPC channel
  double PDU_CURRENTRPC_6;                                   ///< [  124] (8 bytes)  CurrentRPC6: Indicates current of each RPC channel
  double PDU_CURRENTRPC_7;                                   ///< [  132] (8 bytes)  CurrentRPC7: Indicates current of each RPC channel
  double PDU_CURRENTRPC_8;                                   ///< [  140] (8 bytes)  CurrentRPC8: Indicates current of each RPC channel
} PDU4;     ///<  Total size of 148 bytes

void byteswap_PDU4(PDU4 *inPtr, PDU4 *outPtr, int direction);
void bitswap_PDU4(PDU4 *inPtr, PDU4 *outPtr, int direction);
void byteswap_PDU3(PDU3 *inPtr, PDU3 *outPtr, int direction);
void bitswap_PDU3(PDU3 *inPtr, PDU3 *outPtr, int direction);
void byteswap_PDU2(PDU2 *inPtr, PDU2 *outPtr, int direction);
void bitswap_PDU2(PDU2 *inPtr, PDU2 *outPtr, int direction);
void byteswap_PDU1(PDU1 *inPtr, PDU1 *outPtr, int direction);
void bitswap_PDU1(PDU1 *inPtr, PDU1 *outPtr, int direction);
void byteswap_mPDU1(mPDU1 *inPtr, mPDU1 *outPtr, int direction);
void bitswap_mPDU1(mPDU1 *inPtr, mPDU1 *outPtr, int direction);
void byteswap_MBSU2(MBSU2 *inPtr, MBSU2 *outPtr, int direction);
void bitswap_MBSU2(MBSU2 *inPtr, MBSU2 *outPtr, int direction);
void byteswap_MBSU1(MBSU1 *inPtr, MBSU1 *outPtr, int direction);
void bitswap_MBSU1(MBSU1 *inPtr, MBSU1 *outPtr, int direction);
void byteswap_CCSDS_TLM_HDR(CCSDS_TLM_HDR *inPtr, CCSDS_TLM_HDR *outPtr, int direction);
void bitswap_CCSDS_TLM_HDR(CCSDS_TLM_HDR *inPtr, CCSDS_TLM_HDR *outPtr, int direction);
void byteswap_CCSDS_Primary(CCSDS_Primary *inPtr, CCSDS_Primary *outPtr, int direction);
void bitswap_CCSDS_Primary(CCSDS_Primary *inPtr, CCSDS_Primary *outPtr, int direction);
#ifdef __cplusplus
}
#endif
#endif /// #ifndef _AMPS_TYPES_H_
