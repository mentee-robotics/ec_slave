/**
 * @file ethCat.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "ethCat.h"
#include "ecat_slv.h"
#include "utypes.h"
#include "gpio.h"
#include "canIdle.h"
#include "cmsis_os.h"
#include "ethCat_types.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define ETHCAT_DELAY_MS (1000U)     /*< Time reset the etherCat IC, this is make it reload the data structures
                                        from EEPROM. */

#define ETHCAT_DUMMYDATA_500US (1)

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/* TODO: Consider move this data structure of etherCat to linker script. */
_Objects Obj __attribute__ ((section(".etherCat_module_space")));

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void cb_get_inputs (void)
{
   /* Do nothing */
}

void cb_set_outputs (void)
{
   /* Do nothing */
}

uint32_t ethCat_object_download_hook ( uint16_t index,
                                       uint8_t subindex,
                                       uint16_t flags)
{
   (void)flags;
   
   uint32_t ret = 0u;
   
   uint8_t md80id = Obj.md80_Command.md80_dev_no;
   uint8_t cmd = Obj.md80_Command.command;
   uint8_t length = Obj.md80_Command.size;
   uint8_t dataCmd[ETHCAT_LENGTH_DATAMAX] = {0u};
   
   switch (index)
   {
   case ETHCAT_MD80_COMMAND:
      /* Determine whether new command from host. */
      if ((cmd != BUS_FRAME_NONE) && (cmd < BUS_FRAME_INVALID) && (subindex == 9))
      {
         /* Copy command.*/
         memcpy(&dataCmd[0],  (uint8_t *)&Obj.md80_Command.dataSet0, sizeof(Obj.md80_Command.dataSet0));
         memcpy(&dataCmd[4],  (uint8_t *)&Obj.md80_Command.dataSet1, sizeof(Obj.md80_Command.dataSet1));
         memcpy(&dataCmd[8],  (uint8_t *)&Obj.md80_Command.dataSet2, sizeof(Obj.md80_Command.dataSet2));
         memcpy(&dataCmd[12], (uint8_t *)&Obj.md80_Command.dataSet3, sizeof(Obj.md80_Command.dataSet3));
         memcpy(&dataCmd[16], (uint8_t *)&Obj.md80_Command.dataSet4, sizeof(Obj.md80_Command.dataSet4));

         /* Received new command. */
         Obj.md80_Last_Command_Received.command = cmd;
         Obj.md80_Last_Command_Received.counter = Obj.md80_Command.counter;
         Obj.md80_Last_Command_Received.timestamp = (uint32_t)(xTaskGetTickCount() / portTICK_PERIOD_MS);

         /* Reset command. */
         Obj.md80_Command.command = BUS_FRAME_NONE;

         /* Print log via serial.
         cdc_printf("New Command: %d\n", cmd);
         cdc_printf("Md80 id: %d\n", md80id);
         cdc_printf("counter: %d", (int)Obj.md80_Command.counter);
         cdc_printf(" at Time: %d\n", (int)Obj.md80_Last_Command_Received.timestamp);
         cdc_printf("DataCmd received: \n");
         for (int i = 0; i < sizeof(dataCmd); i++)
         {
            printf("%02X", dataCmd[i]);
         }
         printf("\n");

         /* Determine if Host send new command to md80s. */
         canIdle_UpdateCmd(md80id, cmd, length, dataCmd);
      }
      break;
   
   default:
      break;
   }

   return ret;
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
      .post_object_download_hook = ethCat_object_download_hook,
      .rxpdo_override = NULL,
      .txpdo_override = NULL,
      .esc_hw_interrupt_enable = NULL,
      .esc_hw_interrupt_disable = NULL,
      .esc_hw_eep_handler = ethCat_hw_eep_handler,
      .esc_check_dc_handler = ethCat_check_dc_handler,
   };

   /* Turn off etherCat. */
   HAL_GPIO_WritePin (LAN_Reset_CMD_GPIO_Port, LAN_Reset_CMD_Pin, GPIO_PIN_RESET);

   /* Delay */
   osDelay(ETHCAT_DELAY_MS);

   /* Turn on etherCat. */
   HAL_GPIO_WritePin (LAN_Reset_CMD_GPIO_Port, LAN_Reset_CMD_Pin, GPIO_PIN_SET);

   /* Initialize the EtherCat module. */
   ecat_slv_init (&ethCat_cfg);
}

void ethCat_MainFunction (void)
{
	static uint32_t start=0;
	static uint32_t end =0;

	//start = DWT->CYCCNT;//GetCycleCount();
	//start = xTaskGetTickCount()/portTICK_PERIOD_MS;
	ecat_slv();
	//end= DWT->CYCCNT;//GetCycleCount();
	//end= xTaskGetTickCount()/portTICK_PERIOD_MS;
	//sendMessage(ETHCat,2,666, (end-start));

}

void timerCounterCb(void const * argument)
{
#if ETHCAT_DUMMYDATA_500US == 1
   /* Update the counter dummy for md80: 1ms */
   Obj.md80_0_DataReturn.counter++;

   Obj.md80_0_DataReturn.timestamp = (uint32_t)(DWT->CYCCNT/160);
   //Obj.md80_0_DataReturn.timestamp = (uint32_t)(xTaskGetTickCount()/portTICK_PERIOD_MS);
#endif
}
