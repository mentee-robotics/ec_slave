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

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef void (*callbackFunction)(const uint8_t * payload, uint8_t size);

typedef enum
{
   CANIDLE_DEV0 = 0,
   CANIDLE_DEV1,
   CANIDLE_DEV2,
   CANIDLE_DEV3,
   CANIDLE_DEV_TOTAL,
   CANIDLE_ID_INVALID,
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
   uint8_t devReqId;
   struct CAN_msg msgRes;
} tCanIdle_Local;

typedef struct
{
   struct CAN_msg msgRes;
   bool isNewMsg;
   tCanIdle_DeviceId id;
} tCanIdle_Ouput;

typedef struct
{
   uint8_t devReqId;
   uint32_t size;
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

extern tCanIdle_Devices canIdle_Devices[CANIDLE_DEV_TOTAL];
extern tCanIdle_Module canIdle_Module;

#endif /* CANIDLE_TYPES_H_ */
