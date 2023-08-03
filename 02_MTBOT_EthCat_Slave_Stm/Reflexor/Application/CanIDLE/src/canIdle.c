/**
 * @file canIdle.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include <string.h>
#include "can_iso_tp.h"
#include "canIdle_types.h"
#include "canIdle_Config.h"
#include "canIdle.h"
#include "fdcan.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define CANIDLE_DLC_OFF(x) (x << 16U)

#define CANIDLE_TX_SIZE (64U)       /*< Size of tx is 4Kb. */
#define CANIDLE_RX_SIZE (64U)       /*< Size of Rx is 64 bytes. */
#define CANIDLE_MSG_BUFFER (256U)   /*< Size of msg. */
#define CANIDLE_TASK_DELAY_MS (1)   /*< Delay 5 msec. */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static char canIdle_msg[CANIDLE_MSG_BUFFER] = {0};

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION PROTOTYPES
 *-------------------------------------------------------------------------------------------------------------------*/

static uint32_t canIdle_getCurrentTimeInMillis(void);
static void canIdle_debugLog(const char *msg);
static int canIdle_FDCanSend(can_iso_tp_link_t_p link, const struct CAN_msg *msg);
static int canIdle_N_USData_indication(can_iso_tp_link_t_p link,
                                       const uint8_t *payload,
                                       uint32_t size,
                                       CAN_ISO_TP_RESAULT error);
static uint8_t canIdle_getDeviceId(tCanIdle_DeviceId id);

static void canIdle_pollEvent(tCanIdle_Module *const module);

static tCanIdle_State canIdle_WaitingEvent(tCanIdle_Module *const module);
static tCanIdle_State canIdle_ProcessEntry(tCanIdle_Module *const module);

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

static void canIdle_pollEvent(tCanIdle_Module *const module)
{
   can_iso_tp_link_t_p link = NULL;
   uint32_t currTimeMs = 0u;
   uint8_t idx = 0u;

   /* Determine the identify of can device. */
   idx = canIdle_getDeviceId(module->input.id);

   if (CAN_DEV_ID_INVALID != idx)
   {
      /* Get current time of system. */
      currTimeMs = canIdle_getCurrentTimeInMillis();

      /* Get the link and config of device. */
      link = (can_iso_tp_link_t_p)&canIdle_Devices[idx].local;

      /* Polling event of Can TP. */
      iso_can_tp_poll(link, currTimeMs);
   }
}

// Function to get the current time in milliseconds
static uint32_t canIdle_getCurrentTimeInMillis(void)
{
   uint32_t milliseconds = (uint32_t)(xTaskGetTickCount()/portTICK_PERIOD_MS);

   return milliseconds;
}

static uint8_t canIdle_getDeviceId(tCanIdle_DeviceId id)
{
   uint8_t iter = 0u;
   uint8_t deviceId = CAN_DEV_ID_INVALID;

   for (iter = 0u; iter < CAN_DEV_TOTAL; iter++)
   {
      if ((id == canIdle_Devices[iter].deviceId) && (CAN_DEV_ID_INVALID != canIdle_Devices[iter].deviceId))
      {
         deviceId = iter;
         break;
      }
   }

   return (deviceId);
}

static void canIdle_debugLog(const char *msg)
{
   /* Currently, the log message debug of this module does not print out. */
   (void *)msg;
}

static int canIdle_FDCanSend(can_iso_tp_link_t_p link, const struct CAN_msg *msg)
{
   FDCAN_TxHeaderTypeDef txHeader = {0};
   int ret = OP_NOK;

   txHeader.Identifier = msg->id.id;
   txHeader.IdType = (true == msg->id.isExt) ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
   txHeader.DataLength = (FDCAN_DLC_BYTES_0 | ((uint32_t)CANIDLE_DLC_OFF(msg->dlc)));
   txHeader.FDFormat = (true == msg->id.isCANFD) ? FDCAN_FD_CAN : FDCAN_CLASSIC_CAN;
   txHeader.BitRateSwitch = FDCAN_BRS_OFF;
   txHeader.TxFrameType = (true == msg->id.isRemote) ? FDCAN_REMOTE_FRAME : FDCAN_DATA_FRAME;
   txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;

   if (HAL_OK == HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &txHeader, (uint8_t *)msg->data))
   {
      sprintf(canIdle_msg, "can msg with id 0x%x dlc 0x%d: ", msg->id.id, msg->dlc);

      if (NULL != link->init_info.print_debug)
      {
         link->init_info.print_debug(canIdle_msg);
      }

      ret = OP_OK;
   }

   return (ret);
}

static int canIdle_N_USData_indication(can_iso_tp_link_t_p link,
                                       const uint8_t *payload,
                                       uint32_t size,
                                       CAN_ISO_TP_RESAULT error)
{
   int ret = 0;

   if (N_TIMEOUT_CR == error)
   {
      canIdle_Module.local.state = CANIDLE_IDLE;
   }

   return ret;
}

static int canIdle_N_USData_confirm(can_iso_tp_link_t_p link,
                                    const uint8_t *payload,
                                    uint32_t size,
                                    CAN_ISO_TP_RESAULT error)
{
   int ret = 0;

   if (N_TIMEOUT_A == error)
   {
      canIdle_Module.local.state = CANIDLE_IDLE;
   }

   return ret;
}

static tCanIdle_State canIdle_WaitingEvent(tCanIdle_Module *const module)
{
   tCanIdle_State nextState = CANIDLE_IDLE;
   uint8_t idx = CAN_DEV_ID_INVALID;

   if (true == module->local.isNewReq)
   {
      /* Reset flag. */
      module->local.isNewReq = false;
      module->local.isRecMsg = false;
      module->output.isNewResp = false;

      /* Verify the can id before module jump to next state. */
      idx = canIdle_getDeviceId (module->input.id);
      module->local.isFunction = false;

      if (CAN_DEV_ID_INVALID != idx)
      {
         module->local.idx = idx;
         /* Change mode to next state. */
         nextState = CANIDLE_PROCESS_ENTRY;
      }
   }

   return (nextState);
}

static tCanIdle_State canIdle_ProcessEntry (tCanIdle_Module *const module)
{
   tCanIdle_State nextState = CANIDLE_PROCESS_ENTRY;
   can_iso_tp_link_t_p link = NULL;
   uint8_t idx = module->local.idx;
   uint8_t isFunctionId = (uint8_t)module->local.isFunction;

   if (CAN_DEV_ID_INVALID != idx)
   {
      /* Get the link and config of device. */
      link = (can_iso_tp_link_t_p)&canIdle_Devices[idx].local;

      /* Send request.*/
      if (OP_OK != iso_can_tp_N_USData_request (link, isFunctionId, (uint8_t *)module->input.dataReq, module->input.size))
      {
         /* Go back to Idle state when send request data is failed. */
         nextState = CANIDLE_IDLE;
      }

      /* Next state. */
      nextState = CANIDLE_PROCESS;
   }

   return (nextState);
}

static tCanIdle_State canIdle_Process (tCanIdle_Module *const module)
{
   tCanIdle_State nextState = CANIDLE_PROCESS;
   uint8_t idx = module->local.idx;

   if (CAN_DEV_ID_INVALID != idx)
   {
      /* FIXME - Consider add the code to manage the state timeout, device can not send data and receive data from
      Device can on bus after command sent. */

      if (true == module->local.isRecMsg)
      {
         /* Update new state. */
         nextState = CANIDLE_PROCESS_POST;

         /* Reset flag. */
         module->local.isRecMsg = false;
      }
   }

   return (nextState);
}

static tCanIdle_State canIdle_ProcessPost(tCanIdle_Module *const module)
{
   tCanIdle_State nextState = CANIDLE_PROCESS_POST;
   can_iso_tp_link_t_p link = NULL;
   uint8_t idx = module->local.idx;

   /* Update data. */
   if (CAN_DEV_ID_INVALID != idx)
   {
      /* Get the link and config of device. */
      link = (can_iso_tp_link_t_p)&canIdle_Devices[idx].local;

      if (OP_OK == iso_can_tp_L_Data_indication(link, (struct CAN_msg *)&module->local.canResp))
      {
         iso_can_tp_L_Data_confirm(link, (struct CAN_msg *)&module->local.canResp, N_OK);

         /* Enter critical section. */
         taskENTER_CRITICAL();

         /* Update data output. */
         memcpy((uint8_t *)module->output.dataResp, (uint8_t *)module->local.canResp.data, dlc2len(module->local.canResp.dlc));
         
         module->output.size = dlc2len(module->local.canResp.dlc);
         module->output.id = canIdle_Devices[idx].deviceId;

         /* Set flag to identify new data updated. */
         module->output.isNewResp = true;

         /* Exit critical section. */
         taskEXIT_CRITICAL();

         /* Call back function. */
         if (NULL != module->dev[idx].funIrq)
         {
            module->dev[idx].funIrq(module->local.dataResp, module->output.size);
         }
      }
   }

   /* Back to IDLE state. */
   nextState = CANIDLE_IDLE;

   return (nextState);
}

void canIdle_SetAddrTxRxDev (tCanIdle_DeviceId id, uint16_t addrTx, uint16_t addrRx)
{
   uint8_t idx = id;

   if (CAN_DEV_ID_INVALID != idx)
   {
      canIdle_Devices[idx].local.init_info.tx_id.id = addrTx;
      canIdle_Devices[idx].local.init_info.rx_id.id = addrRx;
   }
}

void canIdle_GetAddrTxRxDev(tCanIdle_DeviceId id, uint16_t * addrTx, uint16_t * addrRx)
{
	uint8_t idx = id;

	if (CAN_DEV_ID_INVALID != idx)
	{
	  *addrTx = (uint16_t)canIdle_Devices[idx].local.init_info.tx_id.id;
	  *addrRx = (uint16_t)canIdle_Devices[idx].local.init_info.rx_id.id;
	}
}

bool canIdle_Send (tCanIdle_DeviceId id, const uint8_t *payload, uint8_t size)
{
   bool ret = false;

   if (CANIDLE_IDLE == canIdle_Module.local.state)
   {
      /* Enter critical section. */
      taskENTER_CRITICAL();

      /* Update the Device id send request. */
      canIdle_Module.input.id = id;
      canIdle_Module.input.size = size;

      /* Copy data to locally data of this module. */
      memcpy((void *)canIdle_Module.input.dataReq, (void *)payload, size);

      /* Exit critical section. */
      taskEXIT_CRITICAL();

      /* Setting flag which determine the new data update. */
      canIdle_Module.local.isNewReq = true;

      ret = true;
   }

   return (ret);
}

bool canIdle_SendToAddr (uint32_t addrTx, const uint8_t * payload, uint8_t size)
{
   bool ret = false;

   struct CAN_msg msg = {
      .id.id = addrTx,
      .id.isCANFD = true,
      .id.isExt = false,
      .id.isRemote = false,
      .dlc = lenToMinDlc(size),
   };

   memcpy((uint8_t *)msg.data, (uint8_t *)payload, size);

   if (OP_OK == canIdle_FDCanSend (NULL, (struct CAN_msg *)&msg))
   {
      ret = true;
   }

   return (ret);
}

bool canIdle_Read (tCanIdle_DeviceId * const id, const uint8_t *payload, uint8_t *size)
{
   bool ret = false;
   uint8_t lenDataRet = 0u;

   if (true == canIdle_Module.output.isNewResp)
   {
      lenDataRet = canIdle_Module.output.size;

      memcpy((uint8_t *)payload, (uint8_t *)canIdle_Module.output.dataResp, lenDataRet);
      
      *size = lenDataRet;

      canIdle_Module.output.isNewResp = false;
      
      *id = canIdle_Module.output.id;

      ret = true;
   }

   return ret;
}


bool canIdle_ReadAddr (uint32_t * const id, const uint8_t *payload, uint8_t *size)
{
   bool ret = false;
   uint8_t lenDataRet = 0u;

   if (true == canIdle_Module.local.isRecMsg)
   {
      lenDataRet = dlc2len(canIdle_Module.local.canResp.dlc);

      memcpy((uint8_t *)payload, (uint8_t *)canIdle_Module.local.canResp.data, lenDataRet);

      canIdle_Module.local.isRecMsg = false;

      *id = canIdle_Module.local.canResp.id.id;
      *size = lenDataRet;

      ret = true;
   }

   return ret;
}


bool canIdle_SetFunctionCallbackIsr(tCanIdle_DeviceId id, callbackFunction func)
{
   uint8_t idx = 0u;
   bool ret = false;

   idx = canIdle_getDeviceId(id);

   if (CAN_DEV_ID_INVALID != idx)
   {
      if (NULL == canIdle_Devices[idx].funIrq)
      {
         canIdle_Devices[idx].funIrq = func;

         ret = true;
      }
   }

   return ret;
}

/* FIXME: This function will supply a mechanism to help checking the state connection of device on can bus. */
bool canIdle_IsDevWorking(tCanIdle_DeviceId id)
{
   bool ret = false;

   return (ret);
}

void canIdle_Init (tCanIdle_Module *const module)
{
   uint8_t iter = 0u;

   /* Enable the CAN TP layer. */
   for (iter = 0u; iter < CAN_DEV_TOTAL; iter++)
   {
      if (false != canIdle_Module.dev[iter].isActive)
      {
         canIdle_Module.dev[iter].config.L_Data_request = canIdle_FDCanSend;
         canIdle_Module.dev[iter].config.N_USData_indication = (void *)canIdle_N_USData_indication;
         canIdle_Module.dev[iter].config.N_USData_confirm = (void *)canIdle_N_USData_confirm;
         canIdle_Module.dev[iter].config.rx_buff = module->local.dataResp;
         canIdle_Module.dev[iter].config.rx_buff_len = CANIDLE_RX_SIZE;
      }

      /* Link configuration of each device on bus can. */
      iso_can_tp_create(&canIdle_Module.dev[iter].local, &canIdle_Module.dev[iter].config);
   }

   /* Active notify when receive new msg. */
   HAL_FDCAN_ActivateNotification (&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);

   /* Start FDCAN core. */
   if (HAL_OK == HAL_FDCAN_Start (&hfdcan1))
   {
      /* Reset state of module to Idle. */
      module->local.isNewReq = false;
      module->local.state = CANIDLE_IDLE;
   }
}

void canIdle_MainFunction(tCanIdle_Module *const module)
{
   tCanIdle_State nextState = module->local.state;

   canIdle_pollEvent(module);

   switch (module->local.state)
   {
   case CANIDLE_IDLE:
      nextState = canIdle_WaitingEvent(module);
      break;

   case CANIDLE_PROCESS_ENTRY:
      nextState = canIdle_ProcessEntry(module);
      break;

   case CANIDLE_PROCESS:
      nextState = canIdle_Process(module);
      break;

   case CANIDLE_PROCESS_POST:
      nextState = canIdle_ProcessPost(module);
      break;

   default:
      break;
   }

   module->local.state = nextState;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
   FDCAN_RxHeaderTypeDef rxHeader = {0u};
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

   if (FDCAN_IT_RX_FIFO0_NEW_MESSAGE == (RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE))
   {
      /* Enter critical section. */
      taskENTER_CRITICAL_FROM_ISR();

      /* Get new message from FIFO. */
      if (HAL_OK == HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, (FDCAN_RxHeaderTypeDef *)&rxHeader, (uint8_t *)canIdle_Module.local.canResp.data))
      {
         /* Update new data. */
         canIdle_Module.local.canResp.id.id = rxHeader.Identifier;
         canIdle_Module.local.canResp.id.isCANFD = (FDCAN_FD_CAN == rxHeader.FDFormat) ? true : false;
         canIdle_Module.local.canResp.id.isExt = (FDCAN_EXTENDED_ID == rxHeader.IdType) ? true : false;
         canIdle_Module.local.canResp.id.isRemote = (FDCAN_REMOTE_FRAME == rxHeader.RxFrameType) ? true : false;
         canIdle_Module.local.canResp.dlc = (uint8_t)(rxHeader.DataLength >> 16U);

         /* Change the state of module. */
         canIdle_Module.local.isRecMsg = true;
      }

      /* Exit critical section. */
      taskEXIT_CRITICAL_FROM_ISR(xHigherPriorityTaskWoken);
   }
}

// Where are the scan devices?
void canIdleTask(void const *argument)
{
   /* USER CODE BEGIN canIdleTask */
   canIdle_Init(&canIdle_Module);

   /* Infinite loop */
   for (;;)
   {
      /* Run main function of canIdle. */
      canIdle_MainFunction(&canIdle_Module);
   }
   /* USER CODE END canIdleTask */
}
