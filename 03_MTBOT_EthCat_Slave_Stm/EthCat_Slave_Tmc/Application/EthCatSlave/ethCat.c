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

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

_Objects Obj;

/* Contains the data received from master. */
static tEthCatRxData ethCat_data_receives;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void cb_get_inputs (void)
{
   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.rx0 != Obj.DataRxs.DataRx0)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.rx0 = (Obj.DataRxs.DataRx0);

      /* Update new data to TX memory to send to master. */
      Obj.DataTxs.DataTx0 = ethCat_data_receives.rx0 + 1U;
   }

   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.rx1 != Obj.DataRxs.DataRx1)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.rx1 = (Obj.DataRxs.DataRx1);

      /* Update new data to TX memory to send to master. */
      Obj.DataTxs.DataTx1 = ethCat_data_receives.rx1 + 1U;
   }

   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.rx2 != Obj.DataRxs.DataRx2)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.rx2 = (Obj.DataRxs.DataRx2);

      /* Update new data to TX memory to send to master. */
      Obj.DataTxs.DataTx2 = ethCat_data_receives.rx2 + 1U;
   }

   /* Device receive new data from host. Determine which data member changed. */
   if (ethCat_data_receives.rx3 != Obj.DataRxs.DataRx3)
   {
      /* Save the new data in local memory. */
      ethCat_data_receives.rx3 = (Obj.DataRxs.DataRx3);

      /* Update new data to TX memory to send to master. */
      Obj.DataTxs.DataTx3 = ethCat_data_receives.rx3 + 1U;
   }
}

void cb_set_outputs (void)
{

}

uint16_t ethCat_check_dc_handler (void)
{

}

void ethCat_hw_eep_handler (void)
{

}

void ethCat_Init (void)
{
   /* Configuration the etherCat slave. */
   static esc_cfg_t ethCat_cfg = 
   {
      .user_arg = "EtheCat Slave MTBOT",
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
   osDelay(1000); // Delay 1000 msec
   HAL_GPIO_WritePin (TMC_NRESET_GPIO_Port, TMC_NRESET_Pin, GPIO_PIN_SET);
   osDelay(1); // Delay 1ms

   /* Initialize the EtherCat module. */
   ecat_slv_init (&ethCat_cfg);
}

void ethCat_Task (void const * argument)
{
   /* Initialize the modules which used by this application. */
   ethCat_Init();

   /* Sit in main loop of this task. */
   while (true)
   {
      ecat_slv();
   }
}
