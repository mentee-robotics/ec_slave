/**
 * @file canM.h
 *
 * TODO: Comment.
 */
#ifndef CANM_H_
#define CANM_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "canM_Cfg.h"
#include "canM_Types.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

extern bool canM_SetNewBaudrate (uint8_t newBaudrateMbps);
extern bool canM_IsDevWorking (tCanM_DeviceId id);
extern bool canM_ReadAddr (uint32_t * const id, 
                           const uint8_t *payload, 
                           uint8_t *size);
extern bool canM_SendToAddr (uint32_t addrTx, 
                             const uint8_t * payload, 
                             uint8_t size);
extern bool canM_Send (tCanM_DeviceId id, 
                       const uint8_t * payload, 
                       uint8_t size);
extern bool canM_Read (tCanM_DeviceId * const id, 
                       const uint8_t *payload, 
                       uint8_t *size);
extern bool canM_SetFunctionCallbackIsr ( tCanM_DeviceId id,
                                          callbackFunction func);
extern void canM_SetAddrTxRxDev ( tCanM_DeviceId id,
                                  uint16_t addrTx,
                                  uint16_t addrRx);
extern void canM_GetAddrTxRxDev (tCanM_DeviceId id,
                                 uint16_t * addrTx,
                                 uint16_t * addrRx);
extern void canM_Init (tCanM_Module * const module);
extern bool canM_Start (void);
extern bool canM_Stop (void);
extern void canM_MainFunction (tCanM_Module * const module);

#endif /* CANM_H_ */
