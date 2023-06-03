/**
 * @file ethCat.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "ethCat.h"
#include "ecat_slv.h"
#include "utypes.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "canIdle.h"
#include "md80/md80.h"
#include "md80/md80_types.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define ETHCAT_REST_DURATION (1000) /*< Time reset the etherCat IC, this is make it reload the data structures
                                        from EEPROM. */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

_Objects Obj;

/* Contains the data received from master. */
static tEthCatRxData ethCat_data_receives;

/* MD80 devices. */
tMd80_Device md80_dev1 = {
   .config.canId = 0,
};

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void cb_get_inputs (void)
{
   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.Dev_0_DataControl.Mode != Obj.Dev_0_DataControl.Mode)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.Dev_0_DataControl.Mode = Obj.Dev_0_DataControl.Mode;

      /* Update new data to TX memory to send to master. */
      md80_ControlMd80Mode (&md80_dev1, ethCat_data_receives.Dev_0_DataControl.Mode);
   }

   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.Dev_0_DataControl.Position != Obj.Dev_0_DataControl.Position)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.Dev_0_DataControl.Position = Obj.Dev_0_DataControl.Position;

      /* Update new data to TX memory to send to master. */
      md80_SetTargetPosition (&md80_dev1, ((float)ethCat_data_receives.Dev_0_DataControl.Position / 100.0f));
   }

   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.Dev_0_DataControl.Velocity != Obj.Dev_0_DataControl.Velocity)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.Dev_0_DataControl.Velocity = Obj.Dev_0_DataControl.Velocity;

      /* Update new data to TX memory to send to master. */
      md80_SetTargetVelocity (&md80_dev1, (float)ethCat_data_receives.Dev_0_DataControl.Velocity);
   }

   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.Dev_0_DataControl.Torque != Obj.Dev_0_DataControl.Torque)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.Dev_0_DataControl.Torque = Obj.Dev_0_DataControl.Torque;

      /* Update new data to TX memory to send to master. */
      md80_SetTorque (&md80_dev1, (float)ethCat_data_receives.Dev_0_DataControl.Torque);
   }
}

void cb_set_outputs (void)
{

}

uint16_t ethCat_check_dc_handler (void)
{
   uint16_t ret = 0u;

   return ret;
}

void ethCat_hw_eep_handler (void)
{

}

void ethCat_Init (void)
{
   /* Configuration the etherCat slave. */
   static esc_cfg_t ethCat_cfg = 
   {
      .user_arg = "EtherCat Slave MTBOT",
      .use_interrupt = 0,
      .watchdog_cnt = 150,
      .set_defaults_hook = NULL,
      .pre_state_change_hook = NULL,
      .post_state_change_hook = NULL,
      .application_hook = NULL,
      .safeoutput_override = NULL,
      .pre_object_download_hook = NULL,
      .post_object_download_hook = NULL,
      .rxpdo_override = NULL,
      .txpdo_override = NULL,
      .esc_hw_interrupt_enable = NULL,
      .esc_hw_interrupt_disable = NULL,
      .esc_hw_eep_handler = ethCat_hw_eep_handler,
      .esc_check_dc_handler = ethCat_check_dc_handler,
   };

   /* Reset slave first. */
   HAL_GPIO_WritePin (TMC_NRESET_GPIO_Port, TMC_NRESET_Pin, GPIO_PIN_RESET);

   /* Delay */
   osDelay(ETHCAT_REST_DURATION);

   /* Turn on the etherCat IC. */
   HAL_GPIO_WritePin (TMC_NRESET_GPIO_Port, TMC_NRESET_Pin, GPIO_PIN_SET);

   /* Delay */
   osDelay(1);

   /* Initialize the EtherCat module. */
   ecat_slv_init (&ethCat_cfg);

   /* Reset encoder at current position */
   md80_SetEncoderZero (&md80_dev1);

   /* Set mode to position PID */
   md80_ControlMd80Mode (&md80_dev1, MD80_IMPEDANCE);

   /* Enable the drive */
   md80_ControlMd80Enable (&md80_dev1, true);

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

void ethCat_Task (void const * argument)
{
   /* Initialize the modules which used by this application. */
   ethCat_Init();

   /* Sit in main loop of this task. */
   while (true)
   {
      ecat_slv();

      md80_MainFunction (&md80_dev1);

      Obj.Dev_0_DataReturn.Mode = md80_dev1.local.mode;
      Obj.Dev_0_DataReturn.Position = (uint32_t)(md80_dev1.local.position * 100.0f);
      Obj.Dev_0_DataReturn.Velocity = md80_dev1.local.velocity;
      Obj.Dev_0_DataReturn.Torque = md80_dev1.local.torque;
   }
}
