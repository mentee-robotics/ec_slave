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

#define ETHCAT_TASK_DELAY_MS (1U)   /*< Delay of this task is 1msec. */ 

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/* TODO: Consider move this data structure of etherCat to linker script. */
_Objects Obj;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void cb_get_inputs (void)
{

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
      .user_arg = "MTBOT: EthCat To CanFd",
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
   osDelay(ETHCAT_REST_DURATION);

   /* Initialize the EtherCat module. */
   ecat_slv_init (&ethCat_cfg);
}

void ethCat_Task (void const * argument)
{
   /* Initialize the modules which used by this application. */
   ethCat_Init();

   /* Sit in main loop of this task. */
   for (;;)
   {
      /* Run main function of etherCat. */
      ecat_slv();

      /* Delay tasks. */
      osDelay(ETHCAT_TASK_DELAY_MS);
   }
}
