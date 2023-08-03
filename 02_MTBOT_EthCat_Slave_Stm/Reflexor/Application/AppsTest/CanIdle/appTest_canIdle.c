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
#include "cmsis_os.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static tAppTest_CanIdle_Case testCase = APPTEST_CANIDLE_NO;
static float angleRad = 0.0f;
static float dt = 0.01f;
static bool isCanIdle1Enabled = false;
static float maxAngleRad = 2 * M_PI;
static bool rotationCycle = true;

static tMd80_Device md80_dev1;

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
    	 canIdle_SetAddrTxRxDev(MD80_CAN_0, 70, 70);
    	 //md80_Restart(&md80_dev1);


      	 osDelay(1000);

         /* Reset encoder at current position */
         md80_ControlMd80Mode (&md80_dev1, MD80_IMPEDANCE);
         osDelay(1000);

         md80_ControlMd80Enable (&md80_dev1, true);
         osDelay(1000);

         md80_SetImpedanceControllerParams(&md80_dev1, 10.0f, 1.0f);
         osDelay(1000);

         md80_SetEncoderZero(&md80_dev1);
         osDelay(1000);

         md80_SetMaxTorque (&md80_dev1, 10.0);
         md80_SetMaxVelocity (&md80_dev1, 5.0);
         osDelay(1000);

         /* Set mode to position PID */

         md80_SetCurrentLimit(&md80_dev1, 1.0f);
         osDelay(500);

         isCanIdle1Enabled = true;
      }

      md80_SetTargetPosition (&md80_dev1 , angleRad);
//      md80_SetTorque(&md80_dev1, 10);
//      md80_SetTargetVelocity(&md80_dev1, 1.0);

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
      osDelay(10);

      break;
   
   default:
      break;
   }
}
