/**
 * @file appTest_canIdle.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <math.h>
#include "md80.h"
#include "canM_Types.h"
#include "canM.h"
#include "canIdle.h"
#include "appTest_canIdle.h"
#include "cmsis_os.h"
#include "ethCat_types.h"
#include "canIdle.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static tAppTest_CanIdle appTest = {
   .testCase = APPTEST_CANIDLE_NO,
   .angleRad = 0.0f,
   .dt = 0.01f,
   .maxAngleRad = 2 * M_PI,
   .rotationCycle = true,
   .isCanIdleEnabled = false,
   .md80id = 0,
};

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void AppTest_CanIdle_Init ()
{

}

void AppTest_CanIdle_MainFunction ()
{
   uint8_t cmd = 0;
   uint8_t length = 0;
   uint8_t dataCmd[ETHCAT_LENGTH_DATAMAX] = {0};
   //cdc_printf("[appTest main]@%u:\t %d\n",GetCycleCount(), appTest.testCase);
   switch (appTest.testCase)
   {
   case APPTEST_CANIDLE_NO:

      break;

   case APPTEST_CANIDLE_BLINK:
      md80_ConfigBlink (&appTest.dev);
      break;

   case APPTEST_CANIDLE_SCAN:

      break;
   
   case APPTEST_CANIDLE_EXAMPLE8:
      if (false == appTest.isCanIdleEnabled)
      {
    	   canM_SetAddrTxRxDev(DEV_CAN_0, appTest.md80id, appTest.md80id);

      	osDelay(1000);

         /* Reset encoder at current position */
         md80_ControlMd80Mode (&appTest.dev, MD80_IMPEDANCE);
         osDelay(1000);

         md80_ControlMd80Enable (&appTest.dev, true);
         osDelay(1000);

         md80_SetImpedanceControllerParams(&appTest.dev, 10.0f, 1.0f);
         osDelay(1000);

         md80_SetEncoderZero(&appTest.dev);
         osDelay(1000);

         md80_SetMaxTorque (&appTest.dev, 10.0);
         md80_SetMaxVelocity (&appTest.dev, 5.0);
         osDelay(1000);

         /* Set mode to position PID */

         md80_SetCurrentLimit(&appTest.dev, 1.0f);
         osDelay(500);

         appTest.isCanIdleEnabled = true;
      }

      md80_SetTargetPosition (&appTest.dev , appTest.angleRad);

      if (appTest.rotationCycle)
      {
         appTest.angleRad += appTest.dt;
      }
      else
      {
         appTest.angleRad -= appTest.dt;
      }

      if (appTest.angleRad >= appTest.maxAngleRad)
      {
    	   appTest.rotationCycle = false;
      }
      else if (appTest.angleRad <= 0)
      {
    	   appTest.rotationCycle = true;
      }

      md80_MainFunction (&appTest.dev);
      osDelay(10);

      break;
   
   case APPTEST_CANIDLE_MD80_SET_BAUDRATE:
      md80_ConfigMd80Can(&appTest.dev, 
                         appTest.md80BaudrateSet, 
                         appTest.newTimeoutMs, 
                         appTest.isCanTermination);
      appTest.testCase = APPTEST_CANIDLE_NO;
      break;
   
   case APPTEST_CANIDLE_CONFIGURE_BAUDRATE:
      canIdle_UpdateCmd(appTest.md80id, BUS_FRAME_CANDLE_CONFIG_BAUDRATE, 1, (uint8_t *)&appTest.md80BaudrateSet);
      appTest.testCase = APPTEST_CANIDLE_NO;
      break;

   case APPTEST_CANIDLE_MD80_SAVE:
      md80_ConfigMd80Save(&appTest.dev);
      appTest.testCase = APPTEST_CANIDLE_NO;
      break;

   default:
      break;
   }
}
