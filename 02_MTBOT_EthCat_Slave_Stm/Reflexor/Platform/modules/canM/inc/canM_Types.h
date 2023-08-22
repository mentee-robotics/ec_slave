/**
 * @file canM_types.h
 *
 * TODO: Comment.
 */
#ifndef CANM_TYPES_H_
#define CANM_TYPES_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "fdcan.h"
#include "can_iso_tp.h"
#include "can_iso_tp_private.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define CANM_TX_SIZE (64U)       /*< Size of TX is 64 bytes. */
#define CANM_RX_SIZE (64U)       /*< Size of Rx is 64 bytes. */

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef void (*callbackFunction)(const uint8_t * payload, uint8_t size);

typedef struct
{
	struct {
		uint16_t prescaler;
		uint16_t syncJumpWidth;
		uint16_t timeSeq1;
		uint16_t timeSeq2;
	} nominal;
	struct {
		uint16_t prescaler;
		uint16_t syncJumpWidth;
		uint16_t timeSeq1;
		uint16_t timeSeq2;
	} data;
} tCanM_BusTimingParams;

typedef enum
{
   DEV_CAN_0 = 0,
   DEV_CAN_1,
   DEV_CAN_2,
   DEV_CAN_3,
   DEV_CAN_4,
   DEV_CAN_5,
   DEV_CAN_6,
   DEV_CAN_7,
   CAN_DEV_TOTAL,
   CAN_ALL_DEV,
   CAN_DEV_ID_INVALID,
} tCanM_DeviceId;

typedef enum
{
   CANM_IDLE = 0,
   CANM_PROCESS_ENTRY,
   CANM_PROCESS,
   CANM_PROCESS_POST
} tCanM_State;

typedef struct
{
   tCanM_DeviceId deviceId;
   bool isActive;
   struct can_iso_tp_init_t config;
   struct can_iso_tp_link_t local;
   callbackFunction funIrq;
} tCanM_Devices;

typedef struct
{
   FDCAN_HandleTypeDef * phfdcan;
} tCanM_Config;

typedef struct
{
   tCanM_State state;
   bool isNewReq;
   bool isRecMsg;
   bool isFunction;
   uint8_t idx;
   uint8_t dataResp[CANM_RX_SIZE];
   struct CAN_msg canResp;
} tCanM_Local;

typedef struct
{
   uint8_t dataResp[CANM_RX_SIZE];
   uint8_t size;
   tCanM_DeviceId id;
   bool isNewResp;
} tCanM_Ouput;

typedef struct
{
   uint8_t dataReq[CANM_TX_SIZE];
   uint8_t size;
   tCanM_DeviceId id;
} tCanM_Input;

typedef struct
{
   tCanM_Config config;
   tCanM_Input input;
   tCanM_Local local;
   tCanM_Ouput output;
   tCanM_Devices * dev;
} tCanM_Module;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

extern tCanM_Devices canM_Devices[CAN_DEV_TOTAL];
extern tCanM_Module canM_Module;

#endif /* CANM_TYPES_H_ */
