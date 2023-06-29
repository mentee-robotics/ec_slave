/**
 * @file md80_types.h
 *
 * @details Contains all the types data structure used with this device.
 */

#ifndef MD80_TYPES_H_
#define MD80_TYPES_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef enum
{
   CAN_BAUD_1M = 1,  /*< FDCAN Baudrate of 1Mbps*/
   CAN_BAUD_2M = 2,  /*< FDCAN Baudrate of 2Mbps*/
   CAN_BAUD_5M = 5,  /*< FDCAN Baudrate of 5Mbps*/
   CAN_BAUD_8M = 8,  /*< FDCAN Baudrate of 8Mbps*/
} tMd80_Baudrate;


typedef enum
{
   BUS_FRAME_NONE = 0,
   BUS_FRAME_PING_START = 1,
   BUS_FRAME_CANDLE_CONFIG_BAUDRATE = 2,
   BUS_FRAME_MD80_ADD = 3,
   BUS_FRAME_MD80_GENERIC_FRAME = 4,
   BUS_FRAME_MD80_CONFIG_CAN = 5,
   BUS_FRAME_BEGIN = 6,
   BUS_FRAME_END = 7,
   BUS_FRAME_UPDATE = 8,
   BUS_FRAME_RESET = 9,
} tMd80_BusFrameId;

/**
 * @brief Impedance regulator parameters
 *
 * Impedance regulator output is computed as: torque = kp * position_error + kd * velocity_error + torque_ff;
 */
typedef struct
{
   float kp;
   float kd;
   float torque_ff;
} tMd80_Impedance;

/**
 * @brief PID regulator parameters. This is used to setup either Position PID regulator or Velocity PID regulator.
 * @note i_windup is an anti-windup parameter. This limits the maximum output of the integral (i) part of the regulator.
 */
typedef struct
{
   float kp;
   float ki;
   float kd;
   float i_windup;
} tMd80_Pid;

/**
 * @brief Md80 Control Mode
 * @note Position PID is a cascade regulator, output of the Position PID (target velocity) is passed as an input
 *  of Velocity PID. Velocity PID output (torque) is then passed directly to internal current/torque controller.
 */
typedef enum
{
   MD80_IDLE = 0u,                  /*!< Motor phases are shorted to gnd, the motor is braking */
   MD80_POSITION_PID = 1u,          /*!< Position PID mode (cascade regulators) */
   MD80_VELOCITY_PID = 2u,          /*!< Velocity PID mode */
   MD80_DEPRECATED = 3u,            /*!< This mode is deprecated and left for compatibility reasons. Do not use it */
   MD80_IMPEDANCE = 4u,             /*!< Impedance mode, uses Impedance controller similar to spring-damper system */
} tMd80_Mode;

/**
 * @brief FDCAN frame ids supported by Md80
 */
typedef enum
{
   MD80_FRAME_FLASH_LED = 0x00,
   MD80_FRAME_MOTOR_ENABLE = 0x01,
   MD80_FRAME_CONTROL_SELECT = 0x02,
   MD80_FRAME_ZERO_ENCODER = 0x03,
   MD80_FRAME_BASE_CONFIG = 0x04,
   MD80_FRAME_GET_INFO = 0x05,
   MD80_FRAME_SET_BANDWIDTH = 0x06,
   MD80_FRAME_POS_CONTROL = 0x10,
   MD80_FRAME_VEL_CONTROL = 0x11,
   MD80_FRAME_IMP_CONTROL = 0x12,
   MD80_FRAME_RESTART = 0x13,
   MD80_FRAME_SET_MOTION_TARGETS = 0x14,
   MD80_FRAME_CAN_CONFIG = 0x20,
   MD80_FRAME_CAN_SAVE = 0x21,
   MD80_FRAME_WRITE_REGISTER = 0x40,
   MD80_FRAME_READ_REGISTER = 0x41,
   MD80_FRAME_DIAGNOSTIC = 0x69,
   MD80_FRAME_CALIBRATION = 0x70,
   MD80_FRAME_CALIBRATION_OUTPUT = 0x71,
   MD80_RESPONSE_DEFAULT = 0xA0
} tMd80_FrameId;

typedef struct
{
   uint8_t length;
   uint8_t data[32];
} tMd80_CanFrame;

typedef struct
{
   uint8_t canId;
   tMd80_CanFrame toMd80;
} tMd80_CommandFrame;

typedef struct 
{
   uint8_t canId;
   tMd80_CanFrame fromMd80;
} tMd80_ResponseFrame;

// ########################3

typedef struct
{
   float kp;
   float kd;
   float outMax;
} tMd80_ImpedanceControllerGains;

typedef struct
{
   float kp;
   float ki;
   float kd;
   float intWindup;
   float outMax;
} tMd80_PidControllerGains;

typedef union
{
   struct
   {
      char tag;
      uint8_t revision;
      uint8_t minor;
      uint8_t major;
   } B;
   uint32_t U;
} tMd80_version;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

#endif /* MD80_TYPES_H_ */
