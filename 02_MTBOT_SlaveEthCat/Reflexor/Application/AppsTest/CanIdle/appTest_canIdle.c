/**
 * @file appTest_canIdle.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "canIdle.h"
#include "md80/md80.h"
#include "math.h"
#include "appTest_canIdle.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

tMd80_Device md80_dev1 = {
   .config.canId = CANIDLE_DEV0,
};

static tAppTest_CanIdle_Case testCase = APPTEST_CANIDLE_NO;
static float angleRad = 0.0f;
static float dt = 0.01f;
static bool isCanIdle1Enabled = false;
static float maxAngleRad = 2 * M_PI;
static bool rotationCycle = true;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void AppTest_CanIdle_Init ()
{

}

void AppTest_CanIdle_MainFunction ()
{
   switch (testCase)
   {
   case APPTEST_CANIDLE_NO:

      break;

   case APPTEST_CANIDLE_BLINK:
      md80_ConfigBlink (&md80_dev1);
      break;
   
   case APPTEST_CANIDLE_EXAMPLE8:
      if (false == isCanIdle1Enabled)
      {
         /* Reset encoder at current position */
         md80_SetEncoderZero (&md80_dev1);

         /* Set mode to position PID */
         md80_ControlMd80Mode (&md80_dev1, MD80_IMPEDANCE);

         /* Enable the drive */
         md80_ControlMd80Enable (&md80_dev1, true);

         isCanIdle1Enabled = true;

         md80_SetPositionControllerParams(&md80_dev1, 20.0f, 0.2f, 0.0f, 15.0f);
         md80_SetVelocityControllerParams (&md80_dev1, 0.5f, 0.1f, 0.0f, 1.5f);
         md80_SetImpedanceControllerParams(&md80_dev1, 20.0f, 0.2f);

         /* Set max torque: 50 Nm. */
         md80_SetMaxTorque (&md80_dev1, 10);
         md80_SetTorque(&md80_dev1, 1);

         /* Set velocity: 5 rad/s */
         md80_SetMaxVelocity (&md80_dev1, 20.0f);
         md80_SetTargetVelocity(&md80_dev1, 5.0f);

         md80_SetCurrentLimit (&md80_dev1, 1.0f);
      }


      md80_SetTargetPosition (&md80_dev1 , angleRad);

      if (rotationCycle)
      {
         angleRad += dt;
      }
      else
      {
         angleRad -= dt;
      }

      if (angleRad >= maxAngleRad)
      {
    	   rotationCycle = false;
      }
      else if (angleRad <= 0)
      {
    	   rotationCycle = true;
      }

      md80_MainFunction (&md80_dev1);

      break;
   
   default:
      break;
   }
}
