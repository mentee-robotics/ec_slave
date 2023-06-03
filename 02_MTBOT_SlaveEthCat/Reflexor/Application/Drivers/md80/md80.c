/**
 * @file md80.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include "md80.h"
#include "canIdle.h"
#include "cmsis_os.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

static uint8_t md80_getSize(uint16_t regId)
{
   switch (regId)
   {
   case MD80_REG_motorThermistorType:
   case MD80_REG_motorCalibrationMode:
   case MD80_REG_outputEncoderCalibrationMode:
   case MD80_REG_outputEncoderMode:
   case MD80_REG_bridgeType:
   case MD80_REG_outputEncoder:
   case MD80_REG_hardwareVersion:
   case MD80_REG_canTermination:
   case MD80_REG_motorShutdownTemp:
   case MD80_REG_runCalibrateCmd:
   case MD80_REG_runCalibrateOutpuEncoderCmd:
   case MD80_REG_runCalibratePiGains:
   case MD80_REG_runTestOutputEncoderCmd:
   case MD80_REG_runTestMainEncoderCmd:
   case MD80_REG_runSaveCmd:
      return (1);
   case MD80_REG_motorTorgueBandwidth:
   case MD80_REG_canWatchdog:
   case MD80_REG_errorVector:
   case MD80_REG_motorKV:
      return (2);
   case MD80_REG_shuntResistance:
   case MD80_REG_mainEncoderVelocity:
   case MD80_REG_mainEncoderPosition:
   case MD80_REG_mosfetTemperature:
   case MD80_REG_motorTemperature:
   case MD80_REG_motorInductance:
   case MD80_REG_motorResistance:
   case MD80_REG_firmwareVersion:
   case MD80_REG_buildDate:
   case MD80_REG_motorImpPidKp:
   case MD80_REG_motorImpPidKd:
   case MD80_REG_motorImpPidOutMax:
   case MD80_REG_motorPosPidKp:
   case MD80_REG_motorPosPidKi:
   case MD80_REG_motorPosPidKd:
   case MD80_REG_motorPosPidOutMax:
   case MD80_REG_motorPosPidWindup:
   case MD80_REG_motorVelPidKp:
   case MD80_REG_motorVelPidKi:
   case MD80_REG_motorVelPidKd:
   case MD80_REG_motorVelPidOutMax:
   case MD80_REG_motorVelPidWindup:
   case MD80_REG_motorFriction:
   case MD80_REG_motorStiction:
   case MD80_REG_outputEncoderDir:
   case MD80_REG_outputEncoderDefaultBaud:
   case MD80_REG_outputEncoderVelocity:
   case MD80_REG_outputEncoderPosition:
   case MD80_REG_canBaudrate:
   case MD80_REG_motorGearRatio:
   case MD80_REG_motorPolePairs:
   case MD80_REG_motorKt:
   case MD80_REG_motorKt_a:
   case MD80_REG_motorKt_b:
   case MD80_REG_motorKt_c:
   case MD80_REG_motorIMax:
   case MD80_REG_mainEncoderErrors:
   case MD80_REG_outputEncoderErrors:
   case MD80_REG_calibrationErrors:
   case MD80_REG_bridgeErrors:
   case MD80_REG_hardwareErrors:
   case MD80_REG_communicationErrors:
   case MD80_REG_calOutputEncoderStdDev:
   case MD80_REG_calOutputEncoderMinE:
   case MD80_REG_calOutputEncoderMaxE:
   case MD80_REG_calMainEncoderStdDev:
   case MD80_REG_calMainEncoderMinE:
   case MD80_REG_calMainEncoderMaxE:
   case MD80_REG_canId:
      return (4);
   case MD80_REG_commitHash:
      return (8);
   case MD80_REG_motorName:
      return (24);
   default:
      return (0);
   }
}

void md80_Init(tMd80_Device *const me)
{
}

void md80_Deinit(tMd80_Device *const me)
{
}

bool md80_TransmitReceive(tMd80_Device *const me,
                          uint32_t timeout)
{
   bool ret = false;

   if (true == canIdle_Send(me->config.canId, me->local.command.toMd80.data, me->local.command.toMd80.length))
   {
      if (0 < timeout)
      {
         osDelay(timeout);
         ret = canIdle_Read(me->config.canId, me->local.response.fromMd80.data, (uint8_t *)&me->local.response.fromMd80.length);
      }
   }

   return ret;
}

static void md80_UpdateCommandFrame (tMd80_Device *const me)
{
   switch (me->local.mode)
   {
   case MD80_IDLE:
      me->local.command.toMd80.length = 2u;
      me->local.command.toMd80.data[0] = MD80_FRAME_GET_INFO;
      me->local.command.toMd80.data[1] = 0x00;
      break;

   case MD80_IMPEDANCE:
      if (true == me->config.isRegularsAdjust)
      {
         /* Pack ImpedanceFrame */
         md80_PackImpedanceFrame(me);

         /* Reset flag. */
         me->config.isRegularsAdjust = false;
      }
      else
      {
         /* Pack Motion Target Frame */
         md80_PackMotionTargetsFrame(me);
      }
      break;

   case MD80_POSITION_PID:
      if (true == me->config.isRegularsAdjust)
      {
         /* Pack PositionFrame */
         md80_PackPositionFrame(me);

         /* Reset flag. */
         me->config.isRegularsAdjust = false;
      }
      else if (true == me->config.isVelocityRegulatorAdjust)
      {
         /* Pack velocityRegulator */
         md80_PackVelocityFrame(me);

         /* Reset flag. */
         me->config.isVelocityRegulatorAdjust = false;
      }
      else
      {
         /* packMotionTargetsFrame */
         md80_PackMotionTargetsFrame(me);
      }
      break;

   case MD80_VELOCITY_PID:
      if (true == me->config.isVelocityRegulatorAdjust)
      {
         /* packVelocityFrame */
         md80_PackVelocityFrame(me);

         /* Reset flag. */
         me->config.isVelocityRegulatorAdjust = false;
      }
      else
      {
         /* packMotionTargetsFrame */
         md80_PackMotionTargetsFrame(me);
      }
      break;

   default:
      break;
   }
}

void md80_UpdateRespondeData(tMd80_Device *const me, tMd80_ResponseFrame *const respFrame)
{
   if ((me->config.canId != respFrame->canId) || (respFrame->fromMd80.data[0] != MD80_RESPONSE_DEFAULT))
   {
      return;
   }

   me->local.errorVector = *(uint16_t *)&respFrame->fromMd80.data[1];
   me->local.temperature = respFrame->fromMd80.data[3];
   me->local.position = *(float *)&respFrame->fromMd80.data[4];
   me->local.velocity = *(float *)&respFrame->fromMd80.data[8];
   me->local.torque = *(float *)&respFrame->fromMd80.data[12];
   me->local.encoderPosition = *(float *)&respFrame->fromMd80.data[16];
   me->local.encoderVelocity = *(float *)&respFrame->fromMd80.data[20];
}

bool md80_ConfigBlink(tMd80_Device *const me)
{
   bool ret = false;
   me->local.command.toMd80.data[0] = MD80_FRAME_FLASH_LED;
   me->local.command.toMd80.data[1] = 0x00;
   me->local.command.toMd80.length = 2u;

   if (true == md80_TransmitReceive(me, 500))
   {
      ret = true;
   }

   return ret;
}

bool md80_SetEncoderZero(tMd80_Device *const me)
{
   bool ret = false;

   me->local.command.toMd80.data[0] = MD80_FRAME_ZERO_ENCODER;
   me->local.command.toMd80.data[1] = 0x00;
   me->local.command.toMd80.length = 2u;

   if (true == md80_TransmitReceive(me, 50))
   {
      /* Set target position to 0.0f to avoid jerk at startup */
      md80_SendMotionCommand(me, 0.0f, 0.0f, 0.0f);

      /* Set target position. */
      md80_SetTargetPosition(me, 0.0f);

      ret = true;
   }

   return (ret);
}

void md80_SendMotionCommand(tMd80_Device *const me, float pos, float vel, float torque)
{
   me->local.command.toMd80.length = 16U;
   me->local.command.toMd80.data[0] = MD80_FRAME_SET_MOTION_TARGETS;
   me->local.command.toMd80.data[1] = 0x00;
   *(float *)&me->local.command.toMd80.data[2] = vel;
   *(float *)&me->local.command.toMd80.data[6] = pos;
   *(float *)&me->local.command.toMd80.data[10] = torque;

   if (true == md80_TransmitReceive(me, 100))
   {
      md80_UpdateRespondeData(me, (tMd80_ResponseFrame *)&me->local.response);
   }
}

bool md80_SetCurrentLimit(tMd80_Device *const me, float currentLimit)
{
   bool ret = false;

   if (MD80_DRIVER_MAXCURRENT < currentLimit)
   {
      currentLimit = MD80_DRIVER_MAXCURRENT;
   }
   else if (MD80_DRIVER_MINCURRENT > currentLimit)
   {
      currentLimit = MD80_DRIVER_MINCURRENT;
   }

   me->local.command.toMd80.data[0] = MD80_FRAME_BASE_CONFIG;
   me->local.command.toMd80.data[1] = 0x00;
   me->local.command.toMd80.length = 6u;
   *(float *)&me->local.command.toMd80.data[2] = currentLimit;

   if (true == md80_TransmitReceive(me, 50))
   {
      if (MD80_FRAME_BASE_CONFIG == me->local.response.fromMd80.data[0])
      {
         me->config.currentMax = currentLimit;
         ret = true;
      }
   }

   return (ret);
}

bool md80_ControlMd80Mode(tMd80_Device *const me, tMd80_Mode mode)
{
   bool ret = false;

   me->local.command.toMd80.data[0] = MD80_FRAME_CONTROL_SELECT;
   me->local.command.toMd80.data[1] = 0x00;
   me->local.command.toMd80.data[2] = (uint8_t)mode;
   me->local.command.toMd80.length = 3u;

   if (true == md80_TransmitReceive(me, 50))
   {
      me->local.mode = mode;
      ret = true;
   }

   return ret;
}

bool md80_ControlMd80Enable(tMd80_Device *const me, bool enable)
{
   bool ret = false;

   me->local.command.toMd80.data[0] = MD80_FRAME_MOTOR_ENABLE;
   me->local.command.toMd80.data[1] = 0x00;
   me->local.command.toMd80.data[2] = (uint8_t)enable;
   me->local.command.toMd80.length = 3u;

   if (true == md80_TransmitReceive(me, 50))
   {
	   ret = true;
   }

   return ret;
}

void md80_MainFunction (tMd80_Device *const me)
{
   md80_UpdateCommandFrame (me);

   if (true == md80_TransmitReceive(me, 50))
   {
      md80_UpdateRespondeData(me, (tMd80_ResponseFrame *)&me->local.response);
   }
}

void md80_SetPositionControllerParams(tMd80_Device *const me,
                                      float kp,
                                      float ki,
                                      float kd,
                                      float iWindup)
{
   me->config.isRegularsAdjust = true;
   me->config.positionController.kp = kp;
   me->config.positionController.ki = ki;
   me->config.positionController.kd = kd;
   me->config.positionController.i_windup = iWindup;
}

void md80_SetVelocityControllerParams(tMd80_Device *const me,
                                      float kp,
                                      float ki,
                                      float kd,
                                      float iWindup)
{
   me->config.velocityController.kp = kp;
   me->config.velocityController.ki = ki;
   me->config.velocityController.kd = kd;
   me->config.velocityController.i_windup = iWindup;
   me->config.isVelocityRegulatorAdjust = true;
}

void md80_SetImpedanceControllerParams(tMd80_Device *const me, float kp, float kd)
{
   me->config.impedanceController.kp = kp;
   me->config.impedanceController.kd = kd;
   me->config.isRegularsAdjust = true;
}

void md80_SetMaxTorque(tMd80_Device *const me, float maxTorque)
{
   /* Send request */
   me->config.torqueMax = maxTorque;
   me->config.isTorqueMaxAdjust = true;
}

void md80_SetMaxVelocity(tMd80_Device *const me, float maxVelocity)
{
   me->config.velocityMax = maxVelocity;
   me->config.isVelocityAdjust = true;
}

void md80_SetTargetPosition(tMd80_Device *const me, float pos)
{
   me->input.position = pos;
}

void md80_SetTargetVelocity(tMd80_Device *const me, float vel)
{
   me->input.velocity = vel;
}

void md80_SetTorque(tMd80_Device *const me, float torque)
{
   me->input.torque = torque;
}

void md80_Ping (tMd80_Device *const me, tMd80_Baudrate canBaudrate)
{
   uint8_t data[2u] = {0u};

   data[0] = BUS_FRAME_PING_START;
   data[1] = 0x00;
}

void md80_ConfigCanBaudrate(tMd80_Device *const me, tMd80_Baudrate canBaudrate)
{

}

void md80_PackImpedanceFrame(tMd80_Device *const me)
{
   me->local.command.toMd80.length = 32u;
   me->local.command.toMd80.data[0] = MD80_FRAME_IMP_CONTROL;
   me->local.command.toMd80.data[1] = 0x00;
   *(float *)&me->local.command.toMd80.data[2] = me->config.impedanceController.kp;
   *(float *)&me->local.command.toMd80.data[6] = me->config.impedanceController.kd;
   *(float *)&me->local.command.toMd80.data[10] = me->input.position;
   *(float *)&me->local.command.toMd80.data[14] = me->input.velocity;
   *(float *)&me->local.command.toMd80.data[18] = me->input.torque;
   *(float *)&me->local.command.toMd80.data[22] = me->config.torqueMax;
}

void md80_PackPositionFrame(tMd80_Device *const me)
{
   me->local.command.toMd80.length = 32;
   me->local.command.toMd80.data[0] = MD80_FRAME_POS_CONTROL;
   me->local.command.toMd80.data[1] = 0x00;
   *(float *)&me->local.command.toMd80.data[2] = me->config.positionController.kp;
   *(float *)&me->local.command.toMd80.data[6] = me->config.positionController.ki;
   *(float *)&me->local.command.toMd80.data[10] = me->config.positionController.kd;
   *(float *)&me->local.command.toMd80.data[14] = me->config.positionController.i_windup;
   *(float *)&me->local.command.toMd80.data[18] = me->config.velocityMax;
   *(float *)&me->local.command.toMd80.data[22] = me->input.position;
}

void md80_PackVelocityFrame (tMd80_Device *const me)
{
   me->local.command.toMd80.length = 32;
   me->local.command.toMd80.data[0] = MD80_FRAME_VEL_CONTROL;
   me->local.command.toMd80.data[1] = 0x00;
   *(float *)&me->local.command.toMd80.data[2] =  me->config.velocityController.kp;
   *(float *)&me->local.command.toMd80.data[6] =  me->config.velocityController.ki;
   *(float *)&me->local.command.toMd80.data[10] = me->config.velocityController.kd;
   *(float *)&me->local.command.toMd80.data[14] = me->config.velocityController.i_windup;
   *(float *)&me->local.command.toMd80.data[18] = me->config.torqueMax;
   *(float *)&me->local.command.toMd80.data[22] = me->input.velocity;
}
void md80_PackMotionTargetsFrame (tMd80_Device *const me)
{
   me->local.command.toMd80.length = 24;
   me->local.command.toMd80.data[0] = MD80_FRAME_SET_MOTION_TARGETS;
   me->local.command.toMd80.data[1] = 0x00;
   memcpy((uint8_t *)&me->local.command.toMd80.data[2], (uint8_t *)&me->input.velocity, sizeof(float));
   memcpy((uint8_t *)&me->local.command.toMd80.data[6], (uint8_t *)&me->input.position, sizeof(float));
   memcpy((uint8_t *)&me->local.command.toMd80.data[10], (uint8_t *)&me->input.torque, sizeof(float));
   memcpy((uint8_t *)&me->local.command.toMd80.data[14], (uint8_t *)&me->config.torqueMax, sizeof(float));
   memcpy((uint8_t *)&me->local.command.toMd80.data[18], (uint8_t *)&me->config.velocityMax, sizeof(float));
}
