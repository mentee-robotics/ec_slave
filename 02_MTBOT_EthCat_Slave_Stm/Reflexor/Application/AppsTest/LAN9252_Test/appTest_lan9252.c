/**
 * @file appTest_LAN9252.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "appTest_lan9252.h"
#include "lan9252.h"
#include "main.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static tAppTest_lan9252 lan9252Test = {
   .testCase = APPTEST_LAN9252_NO_TEST,
};

static uint8_t data_Tx[APPTEST_LAN9252_DATA_TX_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};
static uint8_t data_Rx[APPTEST_LAN9252_DATA_RX_SIZE] = {0};

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void AppTest_Lan9252_Init ()
{
   lan9252_Init();

   HAL_GPIO_WritePin(LAN_Reset_CMD_GPIO_Port, LAN_Reset_CMD_Pin, GPIO_PIN_SET);
}

void AppTest_Lan9252_MainFunction ()
{
   uint32_t value = 0u;
   
   switch (lan9252Test.testCase)
   {
   case APPTEST_LAN9252_NO_TEST:
      /** Do nothing here. */
      break;

   case APPTEST_LAN9252_ESC_INFORMATION:
      value = lan9252_read_32 (LAN9252_ESC_CMD_ID_REV);

      /* Get the information for EtherCat slave connected. */
      lan9252Test.info.ethCatIC = (value >> 16) & 0xFFFF;
      
      /* Get the revision of this ic. */
      lan9252Test.info.revision = (value & 0xFFFF);

      /* Get the build of this ic. */
      lan9252Test.info.byteTest = lan9252_read_32(LAN9252_ESC_CMD_BYTE_TEST);

      break;

   default:
      break;
   }
}
