/**
 * @file canIdle.h
 *
 * TODO: Comment.
 */
#ifndef CANIDLE_H_
#define CANIDLE_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "canIdle_Config.h"
#include "canIdle_types.h"

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
bool canIdle_ReadAddr (uint32_t * const id, const uint8_t *payload, uint8_t *size);
bool canIdle_IsDevWorking (tCanIdle_DeviceId id);
void canIdle_SetAddrTxRxDev (tCanIdle_DeviceId id, uint16_t addrTx, uint16_t addrRx);
bool canIdle_Send (tCanIdle_DeviceId id, const uint8_t * payload, uint8_t size);
bool canIdle_SendToAddr (uint32_t addrTx, const uint8_t * payload, uint8_t size);
bool canIdle_Read (tCanIdle_DeviceId * const id, const uint8_t *payload, uint8_t *size);
bool canIdle_SetFunctionCallbackIsr ( tCanIdle_DeviceId id,
                                      callbackFunction func);
void canIdle_GetAddrTxRxDev(tCanIdle_DeviceId id, uint16_t * addrTx, uint16_t * addrRx);
void canIdle_Init (tCanIdle_Module * const module);
void canIdle_MainFunction(tCanIdle_Module * const module);

#endif /* CANIDLE_H_ */
