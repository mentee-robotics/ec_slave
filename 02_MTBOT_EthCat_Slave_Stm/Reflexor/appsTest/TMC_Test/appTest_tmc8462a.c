/**
 * @file appTest_tmc8462a.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "appTest_tmc8462a.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "TMC8462.h"
#include "TMC8462_Register.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static tAppTest_tmc8462A tmcTest = {
   .testCase = APPTEST_TMC8462A_NO_TEST,
};

static ConfigurationTypeDef tmc8462_config_esc;
static ConfigurationTypeDef tmc8462_config_mfc;
static TMC8462TypeDef tmc8462;

static uint8_t data_Tx[APPTEST_TMC8462_DATA_TX_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7};
static uint8_t data_Rx[APPTEST_TMC8462_DATA_RX_SIZE] = {0};

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void AppTest_TMC8462A_Init ()
{
  tmc8462_initConfig (&tmc8462, &tmc8462_config_esc, &tmc8462_config_mfc);
}

void AppTest_TMC8462A_MainFunction ()
{
   switch (tmcTest.testCase)
   {
   case APPTEST_TMC8462A_NO_TEST:
      /** Do nothing here. */
      break;

   case APPTEST_TMC8462A_ESC_INFORMATION:
      /* Get the information for EtherCat slave connected. */
      tmcTest.info.ethCatIC = tmc8462_esc_read_8 (&tmc8462, TMC8462_ESC_INFO_TYPE);
      /* Get the revision of this ic. */
      tmcTest.info.revision = tmc8462_esc_read_8 (&tmc8462, TMC8462_ESC_INFO_REV);
      /* Get the build of this ic. */
      tmcTest.info.build = tmc8462_esc_read_8 (&tmc8462, TMC8462_ESC_INFO_BUILD);

      tmcTest.info.noFmmuChn = tmc8462_esc_read_8 (&tmc8462, 0x0004);
      tmcTest.info.noSyncChn = tmc8462_esc_read_8 (&tmc8462, 0x0005);
      tmcTest.info.ramSize = tmc8462_esc_read_8 (&tmc8462, 0x0006);
      tmcTest.info.portDescrip = tmc8462_esc_read_8 (&tmc8462, 0x0007);
      tmcTest.info.featuresSupport = tmc8462_esc_read_16 (&tmc8462, 0x0008);

      break;

   case APPTEST_TMC8462A_SELFTEST_SPIDMA:
      tmc8462_esc_write_data (&tmc8462, data_Tx, TMC8462_ESC_PROCESS_DATA_RAM, APPTEST_TMC8462_DATA_TX_SIZE);

      vTaskDelay (500/portTICK_PERIOD_MS);

      tmc8462_esc_read_data (&tmc8462, data_Rx, TMC8462_ESC_PROCESS_DATA_RAM, APPTEST_TMC8462_DATA_TX_SIZE);
      break;

   case APPTEST_TMC8462A_GETDATA_FROM_SLAVE:

      break;
   default:
      break;
   }
}
