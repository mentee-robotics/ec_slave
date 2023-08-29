/**
 * @file canM_Cfg.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include "canM_Types.h"
#include "canM_Cfg.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

const tCanM_BusTimingParams canM_BusTimingParams[CANM_TOTAL_BAUDRATE_SUPPORT] =
{
   /* Baudrate: 1Mpbs. */
   {
      .nominal = {4, 8, 31, 8},
      .data = {4, 8, 31, 8},
   },
   /* Baudrate: 2Mpbs. */
   {
      .nominal = {4, 8, 31, 8},
      .data = {2, 10, 29, 10},
   },
   /* Baudrate: 3Mpbs. */
   {
      /* Not support. */
      .nominal = {0},
      .data = {0},
   },
   /* Baudrate: 4Mpbs. */
   {
      .nominal = {4, 8, 31, 8},
      .data = {1, 10, 29, 10},
   },
   /* Baudrate: 5Mpbs. */
   {
      .nominal = {4, 8, 31, 8},
      .data = {1, 8, 23, 8},
   },
   /* Baudrate: 6Mpbs. */
   {
      /* Not support. */
      .nominal = {0},
      .data = {0},
   },
   /* Baudrate: 7Mpbs. */
   {
      /* Not support. */
      .nominal = {0},
      .data = {0},
   },
   /* Baudrate: 8Mpbs. */
   {
      .nominal = {4, 8, 31, 8},
      .data = {1, 9, 10, 9},
   }
};

tCanM_Devices canM_Devices[CAN_DEV_TOTAL] =
{
   /* MD80 Device 0. */
   {
      .deviceId = DEV_CAN_0,
      .isActive = true,
      .funIrq = NULL,
      .config = {
         .rx_id = {0x6B, false, true, false},
         .tx_id = {0x6B, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   },
   /* MD80 Device 1. */
   {
      .deviceId = DEV_CAN_1, 
      .isActive = true, 
      .funIrq = NULL, 
      .config = {
         .rx_id = {71, false, true, false},
         .tx_id = {71, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   },
   /* MD80 Device 2. */
   {
      .deviceId = DEV_CAN_2, 
      .isActive = true, 
      .funIrq = NULL, 
      .config = {
         .rx_id = {102, false, true, false},
         .tx_id = {102, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   },
   /* MD80 Device 3. */
   {
      .deviceId = DEV_CAN_3, 
      .isActive = true, 
      .funIrq = NULL, 
      .config = {
         .rx_id = {103, false, true, false},
         .tx_id = {103, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   },
   /* MD80 Device 4. */
   {
      .deviceId = DEV_CAN_4, 
      .isActive = true, 
      .funIrq = NULL, 
      .config = {
         .rx_id = {103, false, true, false},
         .tx_id = {103, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   },
   /* MD80 Device 5. */
   {
      .deviceId = DEV_CAN_5, 
      .isActive = true, 
      .funIrq = NULL, 
      .config = {
         .rx_id = {103, false, true, false},
         .tx_id = {103, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   },
   /* MD80 Device 6. */
   {
      .deviceId = DEV_CAN_6, 
      .isActive = true, 
      .funIrq = NULL, 
      .config = {
         .rx_id = {103, false, true, false},
         .tx_id = {103, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   },
   /* MD80 Device 7. */
   {
      .deviceId = DEV_CAN_7, 
      .isActive = true, 
      .funIrq = NULL, 
      .config = {
         .rx_id = {103, false, true, false},
         .tx_id = {103, false, true, false},
         .N_Ar = 1000u,
         .N_As = 1000u,
         .N_Bs = 1000u,
         .N_Cr = 1000u,
         .N_WFTmax = 1u,
         .TX_DLC = 15u,
         .frame_pad = 0x77u,
         .FC_BS = 0u,
         .STmin = 0u,
      }
   }
};

tCanM_Module canM_Module = {
    .config.phfdcan = (FDCAN_HandleTypeDef *)&hfdcan1,
    .dev = canM_Devices,
};
