/**
 * @file md80_registers.h
 *
 * @details Contains all the register available of this device.
 */

#ifndef MD80_REGISTERS_H_
#define MD80_REGISTERS_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define MD80_REG_canId  0x001
#define MD80_REG_canBaudrate  0x002
#define MD80_REG_canWatchdog  0x003
#define MD80_REG_canTermination  0x004
#define MD80_REG_motorName  0x010
#define MD80_REG_motorPolePairs  0x011
#define MD80_REG_motorKt  0x012
#define MD80_REG_motorKt_a  0x013
#define MD80_REG_motorKt_b  0x014
#define MD80_REG_motorKt_c  0x015
#define MD80_REG_motorIMax  0x016
#define MD80_REG_motorGearRatio  0x017
#define MD80_REG_motorTorgueBandwidth  0x018
#define MD80_REG_motorFriction  0x019
#define MD80_REG_motorStiction  0x01A
#define MD80_REG_motorResistance  0x01B
#define MD80_REG_motorInductance  0x01C
#define MD80_REG_motorKV  0x01D
#define MD80_REG_motorCalibrationMode  0x01E
#define MD80_REG_motorThermistorType  0x01F
#define MD80_REG_outputEncoder  0x020
#define MD80_REG_outputEncoderDir  0x021
#define MD80_REG_outputEncoderDefaultBaud  0x022
#define MD80_REG_outputEncoderVelocity  0x023
#define MD80_REG_outputEncoderPosition  0x024
#define MD80_REG_outputEncoderMode  0x025
#define MD80_REG_outputEncoderCalibrationMode  0x026

#define MD80_REG_motorPosPidKp  0x030
#define MD80_REG_motorPosPidKi  0x031
#define MD80_REG_motorPosPidKd  0x032
#define MD80_REG_motorPosPidOutMax  0x033
#define MD80_REG_motorPosPidWindup  0x034

#define MD80_REG_motorVelPidKp  0x040
#define MD80_REG_motorVelPidKi  0x041
#define MD80_REG_motorVelPidKd  0x042
#define MD80_REG_motorVelPidOutMax  0x043
#define MD80_REG_motorVelPidWindup  0x044

#define MD80_REG_motorImpPidKp  0x050
#define MD80_REG_motorImpPidKd  0x051
#define MD80_REG_motorImpPidOutMax  0x052

#define MD80_REG_mainEncoderVelocity  0x062
#define MD80_REG_mainEncoderPosition  0x063

#define MD80_REG_runSaveCmd  0x080
#define MD80_REG_runTestMainEncoderCmd  0x081
#define MD80_REG_runTestOutputEncoderCmd  0x082
#define MD80_REG_runCalibrateCmd  0x083
#define MD80_REG_runCalibrateOutpuEncoderCmd  0x084
#define MD80_REG_runCalibratePiGains  0x085

#define MD80_REG_calOutputEncoderStdDev  0x100
#define MD80_REG_calOutputEncoderMinE  0x101
#define MD80_REG_calOutputEncoderMaxE  0x102
#define MD80_REG_calMainEncoderStdDev  0x103
#define MD80_REG_calMainEncoderMinE  0x104
#define MD80_REG_calMainEncoderMaxE  0x105

#define MD80_REG_shuntResistance  0x700

#define MD80_REG_buildDate  0x800
#define MD80_REG_commitHash  0x801
#define MD80_REG_firmwareVersion  0x802
#define MD80_REG_hardwareVersion  0x803
#define MD80_REG_bridgeType  0x804
#define MD80_REG_errorVector  0x805
#define MD80_REG_mosfetTemperature  0x806
#define MD80_REG_motorTemperature  0x807
#define MD80_REG_motorShutdownTemp  0x808
#define MD80_REG_mainEncoderErrors  0x809
#define MD80_REG_outputEncoderErrors  0x80A
#define MD80_REG_calibrationErrors  0x80B
#define MD80_REG_bridgeErrors  0x80C
#define MD80_REG_hardwareErrors  0x80D
#define MD80_REG_communicationErrors  0x80E

#define MD80_REG_TOTAL  (70U)

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

/* Read only registers. */
typedef struct
{
   uint32_t firmwareVersion;
   uint8_t hardwareVersion;
   uint32_t buildDate;
   char commitHash[8];
   uint8_t bridgeType;
   float resistance;
   float inductance;
   uint16_t errorVector;
   float mosfetTemperature;
   float motorTemperature;
   float mainEncoderVelocity;
   float mainEncoderPosition;
   float outputEncoderVelocity;
   float outputEncoderPosition;
   float calOutputEncoderStdDev;
   float calOutputEncoderMinE;
   float calOutputEncoderMaxE;
   float calMainEncoderStdDev;
   float calMainEncoderMinE;
   float calMainEncoderMaxE;
   uint32_t mainEncoderErrors;
   uint32_t outputEncoderErrors;
   uint32_t calibrationErrors;
   uint32_t bridgeErrors;
   uint32_t hardwareErrors;
   uint32_t communicationErrors;
   float shuntResistance;
} tMd80_RegistersRo;

/* Read write registers. */
typedef struct
{
   char motorName[24];
   uint32_t canId;
   uint32_t canBaudrate;
   uint16_t canWatchdog;
   uint8_t canTermination;
   uint32_t polePairs;
   uint16_t motorKV;
   uint8_t motorCalibrationMode;
   uint8_t motorThermistorType;
   float motorKt;
   float motorKt_a;
   float motorKt_b;
   float motorKt_c;
   float iMax;
   float gearRatio;
   uint8_t outputEncoder;
   uint8_t outputEncoderMode;
   uint8_t outputEncoderCalibrationMode;
   float outputEncoderDir;
   uint16_t torqueBandwidth;
   uint32_t outputEncoderDefaultBaud;
   float friction;
   float stiction;
   uint8_t motorShutdownTemp;
   // ImpedanceControllerGains_t impedancePdGains;
   // PidControllerGains_t velocityPidGains;
   // PidControllerGains_t positionPidGains;
   uint8_t runSaveCmd;
   uint8_t runTestOutputEncoderCmd;
   uint8_t runTestMainEncoderCmd;
   uint8_t runCalibrateCmd;
   uint8_t runCalibrateOutpuEncoderCmd;
   uint8_t runCalibratePiGains;
} tMd80_RegistersRw;

typedef struct
{
   tMd80_RegistersRo Ro;
   tMd80_RegistersRw Rw;
} tMd80_regRead;

typedef struct
{
   tMd80_RegistersRw Rw;
} tMd80_regWrite;

typedef enum 
{
   MD80_REG_TYPE_UNKOWN = 0,
   MD80_REG_TYPE_U8 = 1,
   MD80_REG_TYPE_I8 = 2,
   MD80_REG_TYPE_U16 = 3,
   MD80_REG_TYPE_I16 = 4,
   MD80_REG_TYPE_U32 = 5,
   MD80_REG_TYPE_I32 = 6,
   MD80_REG_TYPE_F32 = 7,
   MD80_REG_TYPE_STR = 8,
} tMd80_type;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

#endif /* MD80_REGISTERS_H_ */
