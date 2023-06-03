/**
 * @file md80.h
 *
 * @details Contains the inteface use with this device.
 */

#ifndef MD80_H_
#define MD80_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>
#include "md80_registers.h"
#include "md80_types.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define MD80_DRIVER_MAXCURRENT (40.0F)
#define MD80_DRIVER_MINCURRENT (1.0F)
#define MD80_DRIVER_MINBANDWIDTH (50U)
#define MD80_DRIVER_MAXBANDWIDTH (2500U)

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

/* TODO: Comment */
typedef struct
{
   uint8_t canId;
   tMd80_Baudrate canBaudrate;
   float torqueMax;
   float velocityMax;
   float currentMax;
   tMd80_Pid velocityController;
   tMd80_Pid positionController;
   tMd80_Impedance impedanceController;
   bool isTorqueMaxAdjust;
   bool isVelocityAdjust;
   bool isRegularsAdjust;
   bool isVelocityRegulatorAdjust;
} tMd80_config;

/* TODO: Comment */
typedef struct
{
   float position;
   float velocity;
   float torque;
} tMd80_input;

/* TODO: Comment */
typedef struct
{
   
} tMd80_output;

/* TODO: Comment */
typedef struct
{
   uint16_t frameId;
   float position;
   float velocity;
   float torque;
   float encoderPosition;
   float encoderVelocity;
   uint8_t temperature;
   tMd80_regRead regRead;
   tMd80_regWrite regWrite;
   tMd80_CommandFrame command;
   tMd80_ResponseFrame response;
   uint16_t errorVector;
   tMd80_Mode mode;
} tMd80_local;

/* TODO: Comment */
typedef struct 
{
   tMd80_config config;
   tMd80_input input;
   tMd80_output output;
   tMd80_local local;
} tMd80_Device;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/**
 * Function used to initialize this module.
 *
 * @param me      - TODO: Comment
 *
 * @return void.
 * 
 * @details       - TODO: Comment
 */
void md80_Init (tMd80_Device * const me);

/**
 * Function used to set the position PID Regulator parameters.
 *
 * @param me      - TODO: Comment
 * @param kp      Proportional gain
 * @param ki      integral gain
 * @param kd      derivative gain
 * @param iWindup anti-windup - maximal output of the integral (i) part of the regulator.
 *
 * @return void.
 * 
 * @details Regulator output is target velocity in rad/s. The output is then passed as input to Velocity PID regulator.
 */
void md80_SetPositionControllerParams (tMd80_Device * const me,
                                       float kp,
                                       float ki,
                                       float kd,
                                       float iWindup);

/**
 * Function use to set the Velocity PID Regulator parameters.
 * 
 * @param kp proportional gain
 * @param ki integral gain
 * @param kd derivative gain
 * @param iWindup anti-windup - maximal output of the integral (i) part of the regulator
 * 
 * @return void.
 * 
 * @details Regulator output is Torque in Nm. The output is then passed directly to internal current/torque regulator.
 */
void md80_SetVelocityControllerParams (tMd80_Device * const me,
                                       float kp, 
                                       float ki, 
                                       float kd, 
                                       float iWindup);

/**
 * Function use to set the Impedance Regulator parameters.
 * 
 * @param kp Displacement gain ( analogic to 'k' parameter of the spring-damper equation)
 * @param kd Damping coefficient (analogic to 'b' parameter of the spring-damper equation)
 * 
 * @return void.
 */
void md80_SetImpedanceControllerParams (tMd80_Device * const me, float kp, float kd);

/**
 * Function use to set the Max Torque object 
 * 
 * @param maxTorque Torque limit for PID/Impedance regulators
 * 
 * @note This can be overriden by current limiter set by 'Candle.configMd80CurrentLimit'. Current/torque
 * will be limited to whichever limit has a lower value.
 * @note This is only applied with CUSTOM Impedance/Velocity PID controller settings.
 */
void md80_SetMaxTorque (tMd80_Device * const me, float maxTorque);

/**
 * @brief Set the Max Velocity for Position PID and Velocity PID modes.
 * @note This is only applied with CUSTOM Position PID and Velocity PID controller settings.
 * @note Has no effect in Torque or Impedance mode.
 * @param maxVelocity
 */
void md80_SetMaxVelocity (tMd80_Device * const me, float maxVelocity);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
void md80_SetTargetPosition (tMd80_Device * const me, float pos);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
void md80_SetTargetVelocity (tMd80_Device * const me, float vel);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
void md80_SetTorque (tMd80_Device * const me, float torque);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
uint16_t md80_GetErrorVector (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
float md80_GetPosition (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
float md80_GetVelocity (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
float md80_GetTorque (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
float md80_GetOutputEncoderPosition (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
float md80_GetOutputEncoderVelocity (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
float md80_GetTemperature (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
bool md80_ConfigBlink (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
bool md80_SetEncoderZero (tMd80_Device * const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
void md80_SendMotionCommand (tMd80_Device * const me, float pos, float vel, float torque);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
bool md80_SetCurrentLimit (tMd80_Device * const me, float currentLimit);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
bool md80_ControlMd80Mode (tMd80_Device * const me, tMd80_Mode mode);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
bool md80_ControlMd80Enable (tMd80_Device * const me, bool enable);

void md80_MainFunction (tMd80_Device *const me);

/**
 * TODO - Comment
 * 
 * @param         - TODO: Comment
 * 
 * @details       - TODO: Comment
 */
bool md80_TransmitReceive (tMd80_Device * const me, 
                           uint32_t timeout);

#endif /* MD80_H_ */
