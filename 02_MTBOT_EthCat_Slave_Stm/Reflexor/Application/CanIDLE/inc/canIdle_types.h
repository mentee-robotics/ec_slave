/**
 * @file canIdle_types.h
 *
 * TODO: Comment.
 */
#ifndef CANIDLE_TYPES_H_
#define CANIDLE_TYPES_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "can_iso_tp.h"
#include "canIdle_Config.h"
#include "can_iso_tp_private.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define CANIDLE_TX_SIZE (64U)       /*< Size of TX is 64 bytes. */
#define CANIDLE_RX_SIZE (64U)       /*< Size of Rx is 64 bytes. */

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef void (*callbackFunction)(const uint8_t * payload, uint8_t size);

typedef enum
{
   MD80_CAN_0 = 0,
   MD80_CAN_1,
   MD80_CAN_2,
   MD80_CAN_3,
   CAN_DEV_TOTAL,
   CAN_ALL_DEV,
   CAN_DEV_ID_INVALID,
} tCanIdle_DeviceId;

typedef enum
{
   CANIDLE_IDLE = 0,
   CANIDLE_PROCESS_ENTRY,
   CANIDLE_PROCESS,
   CANIDLE_PROCESS_POST
} tCanIdle_State;

typedef struct
{
   tCanIdle_DeviceId deviceId;
   bool isActive;
   struct can_iso_tp_init_t config;
   struct can_iso_tp_link_t local;
   callbackFunction funIrq;
} tCanIdle_Devices;

typedef struct
{
} tCanIdle_Config;

typedef struct
{
   tCanIdle_State state;
   bool isNewReq;
   bool isRecMsg;
   bool isFunction;
   uint8_t idx;
   uint8_t dataResp[CANIDLE_RX_SIZE];
   struct CAN_msg canResp;
} tCanIdle_Local;

typedef struct
{
   uint8_t dataResp[CANIDLE_RX_SIZE];
   uint8_t size;
   tCanIdle_DeviceId id;
   bool isNewResp;
} tCanIdle_Ouput;

typedef struct
{
   uint8_t dataReq[CANIDLE_TX_SIZE];
   uint8_t size;
   tCanIdle_DeviceId id;
} tCanIdle_Input;

typedef struct
{
   tCanIdle_Config config;
   tCanIdle_Input input;
   tCanIdle_Local local;
   tCanIdle_Ouput output;
   tCanIdle_Devices * dev;
} tCanIdle_Module;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

extern tCanIdle_Devices canIdle_Devices[CAN_DEV_TOTAL];
extern tCanIdle_Module canIdle_Module;

#endif /* CANIDLE_TYPES_H_ */
