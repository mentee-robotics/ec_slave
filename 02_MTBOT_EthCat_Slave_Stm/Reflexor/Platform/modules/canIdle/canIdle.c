/**
 * @file canIdle.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>
#include "canIdle.h"
#include "ethCat_types.h"
#include "canM.h"
#include "cmsis_os.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define CANIDLE_ETH_TO_REAL(x) (x / 100.0f)
#define CANIDLE_REAL_TO_ETH(x) (x * 100.0f)
#define CANIDLE_START_ADDR (10U)
#define CANIDLE_END_ADDR (2000U)
#define CANIDLE_TIME_MS_SCAN (1U)
#define CANIDLE_TOTAL_DEV_SUPPORT (CAN_DEV_TOTAL)

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/
static void canIdle_UpdateResp(tCanIdle_Data *const app);

static void canIdle_ScanDevice(tCanIdle_Data *const app)
{
   uint32_t addr = 1u;
   uint8_t data[2u] = {0u};
   uint8_t timeout = CANIDLE_TIME_MS_SCAN;
   uint8_t dataResp[64u] = {0};
   uint8_t dataSize = 0;
   uint16_t md80Addr[CANIDLE_TOTAL_DEV_SUPPORT] = {0};
   uint8_t md80num = 0;

   /* Prepare the frame used to scan. */
   data[0] = MD80_FRAME_GET_INFO;
   data[1] = 0x00;

   /* Start scan md80 on bus. */
   for (addr = CANIDLE_START_ADDR; addr <= CANIDLE_END_ADDR; addr++)
   {
      if (true == canM_SendToAddr(addr, data, 2u))
      {
         while (timeout--)
         {
            if (true == canM_ReadAddr((uint32_t *)&md80Addr[md80num], (uint8_t *)dataResp, (uint8_t *)&dataSize))
            {
               md80num++;
               break;
            }

            if (md80num >= CANIDLE_TOTAL_DEV_SUPPORT)
            {
               break;
            }

            osDelay(1);
         }

         timeout = CANIDLE_TIME_MS_SCAN;
      }
   }

   /* Data return. */
   if (md80num > 0)
   {
      memcpy(app->rsp.data, md80Addr, md80num * 2u);

      /* Size of data return. */
      app->rsp.size = md80num * 2u;

      /* Update number of md80 detected on can bus. */
      app->numMd80Det = md80num;
   }
}

static void canIdle_Begin(tCanIdle_Data *const app)
{
   if (app->workState == CANIDLE_STOP)
   {
      app->workState = CANIDLE_RUN;
   }
}

static void canIdle_End (tCanIdle_Data *const app)
{
   if (app->workState == CANIDLE_RUN)
   {
      app->workState = CANIDLE_STOP;
   }
}

/**
 * @brief Clears all parameters set on the CANdle, returning it to the state same as after a powerup.
 * @return true if reset was successfully, false otherwise
*/
static void canIdle_Reset (tCanIdle_Data * const app)
{
   tMd80_Device * pMd80dev = NULL;
   uint8_t md80Idx = 0u;

   /* Set CanIdle to STOP mode. */
   canIdle_End(app);

   /* Enter critical section. */
   taskENTER_CRITICAL();

   if (app->numMd80Det > 0)
   {
      /* Reset the md80. */
      for (md80Idx = 0u; md80Idx < app->numMd80Det; md80Idx ++)
      {
         pMd80dev = (tMd80_Device *)&md80Dev[md80Idx];

         if (true == md80_IsEnabled(pMd80dev))
         {
            /* Disable md80. */
            md80_ControlMd80Enable (pMd80dev, false);

            /* Reset. */
            pMd80dev->config.isMd80Detected = false;
         }
         
      }

      /* Remove all address of md80 detected. */
      for (md80Idx = 0u; md80Idx < CANIDLE_TOTAL_DEV_SUPPORT; md80Idx ++)
      {
         Obj.md80_addrs[md80Idx] = 0x00;
      }
   }

   /* Reset all parameters set. */
   app->numMd80Det = 0;
   app->state = CANIDLE_IDLE;
   app->isNewCommand = false;

   /* Exit critical section. */
   taskEXIT_CRITICAL();
}

static void canIdle_GenericFrame(tCanIdle_Data *const app)
{
   uint8_t cmd;
   uint8_t md80Id = 0u;
   tMd80_Device *pMd80dev = NULL;
   uint8_t md80En = 0u;
   uint8_t md80Mode = MD80_IDLE;
   float currentLimit = 0.0f;
   uint16_t torqueBandwidth = 0u;
   float kp = 0.0f;
   float ki = 0.0f;
   float kd = 0.0f;
   float iWindup = 0.0f;
   tMd80_Baudrate newBaudrate;
   uint32_t newTimeout = 0u;;
   bool canTermination = false;

   /* Get command. */
   cmd = app->cmd.data[0];

   /* Get md80 id. */
   md80Id = app->cmd.motorId;

   /* Get the data structure of md80. */
   pMd80dev = (tMd80_Device *)&md80Dev[md80Id];

   /* Base on command. */
   switch (cmd)
   {
   case MD80_FRAME_ZERO_ENCODER:
      md80_SetEncoderZero(pMd80dev);
      break;

   case MD80_FRAME_FLASH_LED:
      md80_ConfigBlink(pMd80dev);
      break;

   case MD80_FRAME_MOTOR_ENABLE:
      md80En = app->cmd.data[1];
      md80_ControlMd80Enable(pMd80dev, md80En);
      break;

   case MD80_FRAME_CONTROL_SELECT:
      md80Mode = app->cmd.data[1];
      md80_ControlMd80Mode(pMd80dev, md80Mode);
      break;

   case MD80_FRAME_BASE_CONFIG:
      memcpy((void *)&currentLimit, (void *)&app->cmd.data[1], sizeof(float));
      md80_SetCurrentLimit(pMd80dev, currentLimit);
      break;

   case MD80_FRAME_SET_BANDWIDTH:
      memcpy((void *)&torqueBandwidth, (void *)&app->cmd.data[1], sizeof(torqueBandwidth));
      md80_SetMaxTorque(pMd80dev, torqueBandwidth);
      break;

   case MD80_FRAME_POS_CONTROL:
      memcpy((void *)&kp, (void *)&app->cmd.data[1], sizeof(kp));
      memcpy((void *)&ki, (void *)&app->cmd.data[5], sizeof(ki));
      memcpy((void *)&kd, (void *)&app->cmd.data[9], sizeof(kd));
      memcpy((void *)&iWindup, (void *)&app->cmd.data[13], sizeof(iWindup));

      /* Update parameters for control position. */
      md80_SetPositionControllerParams(pMd80dev, kp, ki, kd, iWindup);
      break;

   case MD80_FRAME_VEL_CONTROL:
      memcpy((void *)&kp, (void *)&app->cmd.data[1], sizeof(kp));
      memcpy((void *)&ki, (void *)&app->cmd.data[5], sizeof(ki));
      memcpy((void *)&kd, (void *)&app->cmd.data[9], sizeof(kd));
      memcpy((void *)&iWindup, (void *)&app->cmd.data[13], sizeof(iWindup));

      /* Update parameters for control Velocity mode. */
      md80_SetVelocityControllerParams(pMd80dev, kp, ki, kd, iWindup);
      break;

   case MD80_FRAME_IMP_CONTROL:
      memcpy((void *)&kp, (void *)&app->cmd.data[1], sizeof(kp));
      memcpy((void *)&kd, (void *)&app->cmd.data[5], sizeof(kd));

      /* Update parameter for control Impedance mode. */
      md80_SetImpedanceControllerParams(pMd80dev, kp, kd);
      break;

   case MD80_FRAME_CALIBRATION:
      md80_setupCalibration(pMd80dev);
      break;

   case MD80_FRAME_CALIBRATION_OUTPUT:
      md80_setupCalibrationOutput(pMd80dev);
      break;

   case MD80_FRAME_CAN_CONFIG:
      memcpy((void *)&newBaudrate, (void *)&app->cmd.data[2], sizeof(uint32_t));
      memcpy((void *)&newTimeout, (void *)&app->cmd.data[6], sizeof(uint16_t));
      memcpy((void *)&canTermination, (void *)&app->cmd.data[8], sizeof(uint8_t));

      md80_ConfigMd80Can(pMd80dev, newBaudrate, newTimeout, canTermination);
      break;
   
   case MD80_FRAME_CAN_SAVE:
      md80_ConfigMd80Save(pMd80dev);
      break;

   default:
      break;
   }
}

static void canIdle_AddMd80(tCanIdle_Data *const app)
{
   uint16_t canAddr = 0u;
   uint8_t iter = 0u;
   uint16_t canTxAddr = 0u;
   uint16_t canRxAddr = 0u;
   tMd80_Device *pMd80dev = NULL;
   uint8_t md80Id = 0u;

   if (app->cmd.size > 2u)
   {
      /* Get the data configuration of motor. */
      canAddr = (((uint16_t)app->cmd.data[1] << 8u) & 0xFF00U) | (((uint16_t)app->cmd.data[0]) & 0x00FFU);

      for (iter = 0u; iter < CANIDLE_TOTAL_DEV_SUPPORT; iter++)
      {
         /* Get the data structure of md80. */
         pMd80dev = (tMd80_Device *)&md80Dev[iter];
         
         if (false == pMd80dev->config.isMd80Detected)
         {
            /* Initialize md80. */
            md80_Init((tMd80_Device *)pMd80dev);

            /* Update the id. */
            pMd80dev->config.canId = iter;

            /* Update the can address TX and RX for motor. */
            canM_SetAddrTxRxDev(iter, canAddr, canAddr);

            /* Enable motor. */
            pMd80dev->config.isMd80Detected = true;

            break;
         }
         else
         {
            // This is solution workaround, TODO: fix it.
            canM_GetAddrTxRxDev(iter, &canTxAddr, &canRxAddr);
            if ((canAddr == canTxAddr) && (canAddr == canRxAddr))
               break;
         }
      }
   }
}

static void canIdle_RemoveMd80 (tCanIdle_Data *const app)
{
   /* FIXME: Update the code for this function.*/
}

static void canIdle_UpdateDataControl(void)
{
   uint8_t md80idx = 0u;
   tMd80_Device *pMd80dev = NULL;
   tEthCat_md80Control * pEcatMd80Ctl = NULL;
   tEthCat_md80Return * pEcatMd80Res = NULL;
   _Objects * const pEcatObj = (_Objects *)&Obj;

   /* Update the data for all motor detected. */
   for (md80idx = 0u; md80idx < canIdle_Module.numMd80Det; md80idx ++)
   {
      /* Get the pointer to data structure of md80. */
      pMd80dev = (tMd80_Device *)&md80Dev[md80idx];

      /* Determine whether this md80 was enabled successfully. */
      if (true == pMd80dev->local.isEnabled)
      {
         /* Update the pointer to data control corresponding with md80. */
         switch (md80idx)
         {
         case 0:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_0_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_0_DataReturn;
            break;

         case 1:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_1_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_1_DataReturn;
            break;

         case 2:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_2_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_2_DataReturn;
            break;

         case 3:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_3_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_3_DataReturn;
            break;
         
         case 4:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_4_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_4_DataReturn;
            break;
         
         case 5:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_5_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_5_DataReturn;
            break;
         
         case 6:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_6_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_6_DataReturn;
            break;
         
         case 7:
            pEcatMd80Ctl = (tEthCat_md80Control *)&pEcatObj->md80_7_DataControl;
            pEcatMd80Res = (tEthCat_md80Return *)&pEcatObj->md80_7_DataReturn;
            break;

         default:
            break;
         }
         
         /* Enter critical section. */
         taskENTER_CRITICAL();

         /* Update the data control for corresponding md80. */
         pMd80dev->input.velocity = CANIDLE_ETH_TO_REAL(pEcatMd80Ctl->Velocity);
         pMd80dev->input.position = CANIDLE_ETH_TO_REAL(pEcatMd80Ctl->Position);
         pMd80dev->input.torque = CANIDLE_ETH_TO_REAL(pEcatMd80Ctl->Torque);

         /* Update the data response from the corresponding md80. */
         pEcatMd80Res->enabled = true;
         pEcatMd80Res->Mode = pMd80dev->local.mode;
         pEcatMd80Res->Position = CANIDLE_REAL_TO_ETH(pMd80dev->output.position);
         pEcatMd80Res->Temperature = CANIDLE_REAL_TO_ETH(pMd80dev->output.temperature);
         pEcatMd80Res->Torque = CANIDLE_REAL_TO_ETH(pMd80dev->output.torque);
         pEcatMd80Res->Velocity = CANIDLE_REAL_TO_ETH(pMd80dev->output.velocity);
         pEcatMd80Res->encoderPos = CANIDLE_REAL_TO_ETH(pMd80dev->output.encoderPosition);
         pEcatMd80Res->encoderVel = CANIDLE_REAL_TO_ETH(pMd80dev->output.encoderVelocity);
         pEcatMd80Res->timestamp = (uint32_t)(xTaskGetTickCount() / portTICK_PERIOD_MS);
         pEcatMd80Res->counter++;

         /* Exit critical section. */
         taskEXIT_CRITICAL();
      }
   }
}

static tCanIdle_States canIdle_EventHandle(tCanIdle_Data *const app)
{
   tCanIdle_States nextState = CANIDLE_IDLE;

   /** Determine if there has new command. */
   if (true == app->isNewCommand)
   {
      /* Update next state to Command. */
      nextState = CANIDLE_COMMAND_ENTER;

      /* Clear the flag.*/
      app->isNewCommand = false;
   }
   else
   {
      /* Keep is in Control state. */
   }

   return (nextState);
}

static tCanIdle_States canIdle_Command_Enter(tCanIdle_Data *const app)
{
   tCanIdle_States nextState = CANIDLE_COMMAND_ENTER;

   /* Copy data to local. */
   app->cmd.frId = app->hostReq.frId;
   app->cmd.motorId = app->hostReq.motorId;
   app->cmd.size = app->hostReq.size;

   memcpy((uint8_t *)app->cmd.data, (uint8_t *)app->hostReq.data, app->cmd.size);

   /* Go to next state. */
   nextState = CANIDLE_COMMAND;

   /* Update next state. */
   return nextState;
}

static void canIdle_ConfigBaudrate (tCanIdle_Data * const app)
{
   uint8_t newBaudrate = 0u;
   uint8_t md80Id = 0u;

   /* Get baudrate. */
   newBaudrate = app->cmd.data[0];

   app->rsp.frId = app->cmd.frId;
   
   /* Configure new baudrate. */
   if (true == canM_SetNewBaudrate(newBaudrate))
   {
      app->rsp.data[0] = true;
   }
   else 
   {
      app->rsp.data[0] = false;
   }
}

static tCanIdle_States canIdle_Command(tCanIdle_Data *const app)
{
   tCanIdle_States nextState = CANIDLE_COMMAND;

   switch (app->hostReq.frId)
   {
   case BUS_FRAME_NONE:
      /* code */
      break;

   case BUS_FRAME_PING_START:
      /* Scan device on bus can. */
      canIdle_ScanDevice(app);
      break;

   case BUS_FRAME_MD80_ADD:
      /* Add md80 with id. */
      canIdle_AddMd80(app);
      break;

   case BUS_FRAME_MD80_GENERIC_FRAME:
      /* Send data configuration. */
      canIdle_GenericFrame(app);
      break;

   case BUS_FRAME_BEGIN:
      canIdle_Begin(app);
      break;

   case BUS_FRAME_END:
      canIdle_End(app);
      break;

   case BUS_FRAME_RESET:
      canIdle_Reset(app);
      break;

   case BUS_FRAME_CANDLE_CONFIG_BAUDRATE:
      canIdle_ConfigBaudrate(app);
      break;

   default:
      break;
   }

   /* Go to next state after finished handling the command. */
   nextState = CANIDLE_COMMAND_EXIT;

   /* Update state. */
   return nextState;
}

static tCanIdle_States canIdle_Command_Exit(tCanIdle_Data *const app)
{
   tCanIdle_States nextState = CANIDLE_COMMAND_EXIT;

   /* Enter critical section. */
   taskENTER_CRITICAL();

   /* Update the frame id and motor id. */
   app->rsp.frId = app->cmd.frId;
   app->rsp.motorId = app->cmd.motorId;
   app->rsp.size = app->cmd.size;

   /* Go back to Idle state. */
   nextState = CANIDLE_IDLE;

   /* Call function interrupt when it */
   canIdle_UpdateResp(app);

   /* Exit critical section. */
   taskEXIT_CRITICAL();

   /* Update state. */
   return nextState;
}

static void canIdle_Control(tCanIdle_Data * const app)
{
   uint8_t iter = 0u;
   tMd80_Device *pMd80dev = NULL;

   for (iter = 0u; iter < app->numMd80Det; iter++)
   {
      /* Get the data structure of md80. */
      pMd80dev = (tMd80_Device *)&md80Dev[iter];

      /* Run the main function. */
      md80_MainFunction(pMd80dev);
   }
}

static void canIdle_UpdateResp (tCanIdle_Data * const app)
{
   uint8_t iter = 0u;
   uint8_t md80Id = 0u;
   tMd80_Device *pMd80dev = NULL;

   if (app->rsp.frId == BUS_FRAME_PING_START)
   {
      /* Copy list address of md80. */
      if (app->numMd80Det > 0u)
      {
         for (iter = 0u; iter < app->numMd80Det; iter++)
         {
            Obj.md80_addrs[iter] = (uint32_t)app->rsp.data[iter * 2u] + ((uint32_t)app->rsp.data[(iter * 2u) + 1u] << 8);
         }

         for (;iter < CANIDLE_TOTAL_DEV_SUPPORT; iter++)
         {
            Obj.md80_addrs[iter] = 0U;
         }
      }
      else
      {
         for (iter = 0u; iter < CANIDLE_TOTAL_DEV_SUPPORT; iter++)
         {
            Obj.md80_addrs[iter] = 0U;
         }
      }
   }
   else if (app->rsp.frId == BUS_FRAME_CANDLE_CONFIG_BAUDRATE)
   {

   }
   /* Another command. */
   else
   {
      /* Get the data structure of md80. */
      pMd80dev = (tMd80_Device *)&md80Dev[app->cmd.motorId];

      memcpy((void *)&app->rsp.data[0], (void *)&pMd80dev->output.errorVector, sizeof(uint16_t));
      memcpy((void *)&app->rsp.data[2], (void *)&pMd80dev->output.temperature, sizeof(uint8_t));
      memcpy((void *)&app->rsp.data[3], (void *)&pMd80dev->output.position, sizeof(float));
      memcpy((void *)&app->rsp.data[7], (void *)&pMd80dev->output.velocity, sizeof(float));
      memcpy((void *)&app->rsp.data[11], (void *)&pMd80dev->output.torque, sizeof(float));
      memcpy((void *)&app->rsp.data[15], (void *)&pMd80dev->output.encoderPosition, sizeof(float));
      memcpy((void *)&app->rsp.data[19], (void *)&pMd80dev->output.encoderVelocity, sizeof(float));
      app->rsp.size = 23u;
   }

   /* Update to object. */
   Obj.md80_Respond.md80_dev_no = app->rsp.motorId;
   Obj.md80_Respond.command = app->rsp.frId;
   Obj.md80_Respond.size = app->rsp.size;
   Obj.md80_Respond.timestamp = xTaskGetTickCount()/portTICK_PERIOD_MS;
   memcpy((void *)&Obj.md80_Respond.dataRet0, (void *)&app->rsp.data[0], 4);
   memcpy((void *)&Obj.md80_Respond.dataRet1, (void *)&app->rsp.data[4], 4);
   memcpy((void *)&Obj.md80_Respond.dataRet2, (void *)&app->rsp.data[8], 4);
   memcpy((void *)&Obj.md80_Respond.dataRet3, (void *)&app->rsp.data[12], 4);
   memcpy((void *)&Obj.md80_Respond.dataRet4, (void *)&app->rsp.data[16], 4);
}

void canIdle_UpdateCmd (uint8_t md80id, uint8_t command, uint8_t size, uint8_t *cmd)
{
   bool isMd80IdExist = false;
   tCanIdle_Data * const pCanIdleM = (tCanIdle_Data *)&canIdle_Module;

   /* TODO: Determine whether id exist in list. */
   isMd80IdExist = true;

   if (true == isMd80IdExist)
   {
      /* Enter critical section. */
      taskENTER_CRITICAL();

      /* Save the cmd. */
      pCanIdleM->hostReq.motorId = md80id;
      pCanIdleM->hostReq.frId = command;
      pCanIdleM->hostReq.size = size;
      memcpy((uint8_t *)&pCanIdleM->hostReq.data[0], (uint8_t *)cmd, size);

      /* Set flag determine it having new command.*/
      pCanIdleM->isNewCommand = true;

      /* Exit critical section. */
      taskEXIT_CRITICAL();
   }
   else
   {
      /* Do nothing. */
   }
}

void canIdle_Init()
{

}

void canIdle_Deinit()
{

}

void canIdle_MainFunction()
{
   tCanIdle_Data * const pCanIdleM = (tCanIdle_Data *)&canIdle_Module;
   tCanIdle_States nextState = pCanIdleM->state;

   /* Update data control. */
   canIdle_UpdateDataControl();

   /* Run mode. */
   switch (nextState)
   {
   case CANIDLE_IDLE:
      nextState = canIdle_EventHandle(pCanIdleM);
      /* code */
      break;

   case CANIDLE_COMMAND_ENTER:
      nextState = canIdle_Command_Enter(pCanIdleM);
      break;

   case CANIDLE_COMMAND:
      nextState = canIdle_Command(pCanIdleM);
      break;

   case CANIDLE_COMMAND_EXIT:
      nextState = canIdle_Command_Exit(pCanIdleM);
      break;

   default:
      nextState = CANIDLE_IDLE;
      break;
   }

   pCanIdleM->state = nextState;

   if (CANIDLE_RUN == pCanIdleM->workState)
   {
      canIdle_Control(pCanIdleM);
   }
}
