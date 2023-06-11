/**
 * @file canIdle_Config.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include "canIdle_types.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

tCanIdle_Devices canIdle_Devices[CAN_DEV_TOTAL] =
{
   /* MD80 Device 0. */
   {
      .deviceId = MD80_CAN_0,
      .isActive = true,
      .funIrq = NULL,
      .config = {
         .rx_id = {
            70,
            false, 
            true, 
            false
            },
         .tx_id = {
            70,
            false, 
            true, 
            false
            },
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
      .deviceId = MD80_CAN_1,
      .isActive = true,
      .funIrq = NULL,
      .config = {
         .rx_id = {
            71,
            false, 
            true, 
            false
            },
         .tx_id = {
            71,
            false, 
            true, 
            false
            },
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
      .deviceId = MD80_CAN_2,
      .isActive = true,
      .funIrq = NULL,
      .config = {
         .rx_id = {
            102,
            false, 
            true, 
            false},
         .tx_id = {
            102,
            false, 
            true, 
            false
            },
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
      .deviceId = MD80_CAN_3,
      .isActive = true,
      .funIrq = NULL,
      .config = {
         .rx_id = {
            103,
            false, 
            true, 
            false},
         .tx_id = {
            103,
            false, 
            true, 
            false
         },
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

tCanIdle_Module canIdle_Module = {
      .dev = canIdle_Devices,
};

