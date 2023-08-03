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
#include "canIdle.h"
#include "app_md80.h"
#include "uartDma/uartDma.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define ETHCAT_REST_DURATION (1000) /*< Time reset the etherCat IC, this is make it reload the data structures
                                        from EEPROM. */

#define ETHCAT_TASK_DELAY_MS (1U)   /*< Delay of this task is 1msec. */
#define ETHCAT_MD80_COMMAND (0x2002)

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
   uint8_t md80id = Obj.md80_Command.md80_dev_no;
   uint8_t cmd = Obj.md80_Command.command;
   uint8_t length = Obj.md80_Command.size;
   uint8_t dataCmd[24U] = {0u};

   /* Determine whether new command from host. */
   if (cmd != 0)
   {
      /* Copy command.*/
      memcpy(&dataCmd[0], (uint8_t *)&Obj.md80_Command.dataSet0, sizeof(Obj.md80_Command.dataSet0));
      memcpy(&dataCmd[4], (uint8_t *)&Obj.md80_Command.dataSet1, sizeof(Obj.md80_Command.dataSet1));
      memcpy(&dataCmd[8], (uint8_t *)&Obj.md80_Command.dataSet2, sizeof(Obj.md80_Command.dataSet2));
      memcpy(&dataCmd[12], (uint8_t *)&Obj.md80_Command.dataSet3, sizeof(Obj.md80_Command.dataSet3));
      memcpy(&dataCmd[16], (uint8_t *)&Obj.md80_Command.dataSet4, sizeof(Obj.md80_Command.dataSet4));

      if (cmd == 3)
      {
    	  printf ("Stop here");
      }

      /* Received new command. */
      Obj.md80_Last_Command_Received.command = cmd;
      Obj.md80_Last_Command_Received.counter = Obj.md80_Command.counter;
      Obj.md80_Last_Command_Received.timestamp = (uint32_t)(xTaskGetTickCount()/portTICK_PERIOD_MS);

      /* Print log via serial. */
      printf ("New Command: %d\n", cmd);
      printf ("Md80 id: %d\n", md80id);
      printf ("counter: %d", (int)Obj.md80_Command.counter);
      printf (" at Time: %d\n", (int)Obj.md80_Last_Command_Received.timestamp);
      printf ("DataCmd received: \n");
      for (int i = 0; i < sizeof(dataCmd); i++)
      {
    	  printf("%02X", dataCmd[i]);
      }
      printf ("\n");

      /* Clear the Command. */
      memset((void *)&Obj.md80_Command, 0x00, sizeof(Obj.md80_Command));

      /* Determine if Host send new command to md80s. */
      App_Md80_UpdateCmd(md80id, cmd, length, dataCmd);
   }
}

void cb_set_outputs (void)
{

}

uint32_t ethCat_object_download_hook ( uint16_t index,
                                       uint8_t subindex,
                                       uint16_t flags)
{
   return (0);
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
   osDelay(ETHCAT_REST_DURATION);

   /* Turn on etherCat. */
   HAL_GPIO_WritePin (LAN_Reset_CMD_GPIO_Port, LAN_Reset_CMD_Pin, GPIO_PIN_SET);

   /* Initialize the EtherCat module. */
   ecat_slv_init (&ethCat_cfg);
}

void timerCounterCb(void const * argument)
{
#ifdef ETHCAT_DUMMYDATA_500US == 1
   /* Update the counter dummy for md80: 1ms */
   Obj.md80_0_DataReturn.counter++;
   Obj.md80_0_DataReturn.timestamp = (uint32_t)(xTaskGetTickCount()/portTICK_PERIOD_MS);
#endif
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
   }
}
