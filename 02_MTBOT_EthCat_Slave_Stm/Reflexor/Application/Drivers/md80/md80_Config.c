/**
 * @file md80_Config.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "md80.h"
#include "canIdle.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

tMd80_Device md80_Dev[MD80_NUM_DEV] = 
{
   /* MD80 Device 0. */
   {
      .config = {
         MD80_CAN_0,                /**< Id of Md80. */
         CAN_BAUD_1M,               /**< Baudrate of motor. */
         10.0f,                     /**< Torque max. */
         20.0f,                     /**< Velocity max. */
         2.0f,                      /**< Current max. */
         .velocityController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .positionController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .impedanceController = {
            .kp = 20.0f,
            .kd = 0.2f,
            .torque_ff = 0.0f,
         },
         .isMd80Detected = false,
      },
   },
   /* MD80 Device 1. */
   {
      .config = {
         MD80_CAN_1,                /**< Id of Md80. */
         CAN_BAUD_1M,               /**< Baudrate of motor. */
         10.0f,                     /**< Torque max. */
         20.0f,                     /**< Velocity max. */
         2.0f,                      /**< Current max. */
         .velocityController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .positionController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .impedanceController = {
            .kp = 20.0f,
            .kd = 0.2f,
            .torque_ff = 0.0f,
         },
         .isMd80Detected = false,
      },
   },
   /* MD80 Device 2. */
   {
      .config = {
         MD80_CAN_2,                /**< Id of Md80. */
         CAN_BAUD_1M,               /**< Baudrate of motor. */
         10.0f,                     /**< Torque max. */
         20.0f,                     /**< Velocity max. */
         2.0f,                      /**< Current max. */
         .velocityController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .positionController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .impedanceController = {
            .kp = 20.0f,
            .kd = 0.2f,
            .torque_ff = 0.0f,
         },
         .isMd80Detected = false,
      },
   },
   /* MD80 Device 3. */
   {
      .config = {
         MD80_CAN_3,                /**< Id of Md80. */
         CAN_BAUD_1M,               /**< Baudrate of motor. */
         10.0f,                     /**< Torque max. */
         20.0f,                     /**< Velocity max. */
         2.0f,                      /**< Current max. */
         .velocityController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .positionController = {
            .kp = 20.0f,
            .ki = 10.0f,
            .kd = 1.0f,
            .i_windup = 1.0f
         },
         .impedanceController = {
            .kp = 20.0f,
            .kd = 0.2f,
            .torque_ff = 0.0f,
         },
         .isMd80Detected = false,
      },
   },
   /* MD80 Device 4. */
   {
      0
   },
   /* MD80 Device 5. */
   {
      0
   }
};
