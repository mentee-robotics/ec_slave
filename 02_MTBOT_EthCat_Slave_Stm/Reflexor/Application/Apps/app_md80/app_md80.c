/**
 * @file app_md80.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "app_md80.h"
#include <string.h>

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define MD80_ETH_TO_REAL(x)         (x / 100.0f)
#define MD80_REAL_TO_ETH(x)         (x * 100.0f)
#define APP_MD80_TASK_DELAY         (1)     /**< Delay 1ms. */
#define APP_MD80_START_ADDR         (10U)
#define APP_MD80_END_ADDR           (2000U)
#define APP_MD80_TIME_MS_SCAN       (5U)

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static tAppMd80_Data * pAppMd80 = NULL;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/
static void App_Md80_UpdateResp (tAppMd80_Data * const app);

static void App_Md80_ScanDevice (tAppMd80_Data * const app)
{
   uint32_t addr = 1u;
   uint8_t iter = 0u;
   uint8_t data[2u] = {0u};
   uint8_t timeout = APP_MD80_TIME_MS_SCAN;
   uint8_t dataResp[64u] = {0};
   uint8_t dataSize = 0;
   uint16_t md80Addr[APP_MD80_TOTAL_DEV_SUPPORT] = {0};
   uint8_t md80num = 0;

   /* Prepare the frame used to scan. */
   data[0] = MD80_FRAME_GET_INFO;
   data[1] = 0x00;

   /* Start scan md80 on bus. */
   for (addr = APP_MD80_START_ADDR; addr <= APP_MD80_END_ADDR; addr++)
   {
      if (true == canIdle_SendToAddr(addr, data, 2u))
      {
         while (timeout--)
         {
            if (true == canIdle_ReadAddr((uint32_t *)&md80Addr[md80num], (uint8_t *)dataResp, (uint8_t *)&dataSize))
            {
               md80num++;
               break;
            }

            osDelay(1);
         }

         timeout = APP_MD80_TIME_MS_SCAN;
      }
   }

   /* Data return. */
   if (md80num > 0)
   {
      memcpy (app->rsp.data, md80Addr, md80num * 2u);
      
      /* Size of data return. */
      app->rsp.size = md80num * 2u;

      /* Update number of md80 detected on can bus. */
      app->numMd80Det = md80num;
   }
}


static void App_Md80_Begin (tAppMd80_Data * const app)
{
   if (app->workState == APP_MD80_STOP)
   {
      app->workState = APP_MD80_RUN;
   }
}

static void App_Md80_End (tAppMd80_Data * const app)
{
   if (app->workState == APP_MD80_RUN)
   {
      app->workState = APP_MD80_STOP;
   }
}

static void App_Md80_GenericFrame (tAppMd80_Data * const app)
{
   uint8_t cmd;
   uint8_t md80Id;
   tMd80_Device * pMd80dev = NULL;
   uint8_t md80En = 0u;
   uint8_t md80Mode = MD80_IDLE;
   float currentLimit = 0.0f;
   uint16_t torqueBandwidth = 0u;
   float kp = 0.0f;
   float ki = 0.0f;
   float kd = 0.0f;
   float iWindup = 0.0f;

   /* Get command. */
   cmd = app->cmd.data[0];

   /* Get md80 id. */
   md80Id = app->cmd.motorId;

   /* Get the data structure of md80. */
   pMd80dev = (tMd80_Device *)app->pMd80[md80Id];

   /* Base on command. */
   switch (cmd)
   {
   case MD80_FRAME_ZERO_ENCODER:
      md80_SetEncoderZero (pMd80dev);
      break;
   
   case MD80_FRAME_FLASH_LED:
      md80_ConfigBlink (pMd80dev);
      break;

   case MD80_FRAME_MOTOR_ENABLE:
      md80En = app->cmd.data[1];
      md80_ControlMd80Enable (pMd80dev, md80En);
      break;
   
   case MD80_FRAME_CONTROL_SELECT:
      md80Mode = app->cmd.data[1];
      md80_ControlMd80Mode (pMd80dev, md80Mode);
      break;
   
   case MD80_FRAME_BASE_CONFIG:
      memcpy((void*)&currentLimit, (void *)&app->cmd.data[1], sizeof(float));
      md80_SetCurrentLimit(pMd80dev, currentLimit);
      break;
   
   case MD80_FRAME_SET_BANDWIDTH:
      memcpy ((void *)&torqueBandwidth, (void *)&app->cmd.data[1], sizeof(torqueBandwidth));
      /** FIXME: Add the function of MD80 to update bandwidth.*/
      break;

   case MD80_FRAME_POS_CONTROL:
      memcpy ((void *)&kp, (void *)&app->cmd.data[1], sizeof(kp));
      memcpy ((void *)&ki, (void *)&app->cmd.data[5], sizeof(ki));
      memcpy ((void *)&kd, (void *)&app->cmd.data[9], sizeof(kd));
      memcpy ((void *)&iWindup, (void *)&app->cmd.data[13], sizeof(iWindup));

      /* Update parameters for control position. */
      md80_SetPositionControllerParams (pMd80dev, kp, ki, kd, iWindup);
      break;

   case MD80_FRAME_VEL_CONTROL:
      memcpy ((void *)&kp, (void *)&app->cmd.data[1], sizeof(kp));
      memcpy ((void *)&ki, (void *)&app->cmd.data[5], sizeof(ki));
      memcpy ((void *)&kd, (void *)&app->cmd.data[9], sizeof(kd));
      memcpy ((void *)&iWindup, (void *)&app->cmd.data[13], sizeof(iWindup));

      /* Update parameters for control Velocity mode. */
      md80_SetVelocityControllerParams (pMd80dev, kp, ki, kd, iWindup);
      break;
      
   case MD80_FRAME_IMP_CONTROL:
      memcpy ((void *)&kp, (void *)&app->cmd.data[1], sizeof(kp));
      memcpy ((void *)&kd, (void *)&app->cmd.data[5], sizeof(kd));

      /* Update parameter for control Impedance mode. */
      md80_SetImpedanceControllerParams(pMd80dev, kp, kd);
      break;

   default:
      break;
   }
}

static void App_Md80_AddMd80 (tAppMd80_Data * const app)
{
   uint16_t canAddr = 0u;
   uint8_t iter = 0u;

   if (app->cmd.size > 2u)
   {
      /* Get the data configuration of motor. */
      canAddr = (((uint16_t)app->cmd.data[0] << 8u)&0xFF00U) | (((uint16_t)app->cmd.data[1])&0x00FFU);

      for (iter = 0u; iter < APP_MD80_TOTAL_DEV_SUPPORT; iter ++)
      {
         if (false == app->pMd80[iter]->config.isMd80Detected)
         {
            /* Initialize md80. */
            md80_Init ((tMd80_Device *)app->pMd80[iter]);

            /* Update the id. */
            app->pMd80[iter]->config.canId = iter;
            
            /* Update the can address TX and RX for motor. */
            canIdle_SetAddrTxRxDev (iter, canAddr, canAddr);

            /* Enable motor. */
            app->pMd80[iter]->config.isMd80Detected = true;

            break;
         }
      }
   }
}

static void App_Md80_RemoveMd80 (tAppMd80_Data * const app)
{
   /* FIXME: Update the code for this function.*/
}

static void App_Md80_UpdateDataControl(void)
{
   /* MD80_DEV_0. */
   if (true == pAppMd80->pMd80[0]->local.isEnabled)
   {
      /* Update the data control for md80. */
      pAppMd80->pMd80[0]->input.velocity = MD80_ETH_TO_REAL(Obj.md80_0_DataControl.Velocity);
      pAppMd80->pMd80[0]->input.position = MD80_ETH_TO_REAL(Obj.md80_0_DataControl.Position);
      pAppMd80->pMd80[0]->input.torque = MD80_ETH_TO_REAL(Obj.md80_0_DataControl.Torque);

      md80_SetTorque((tMd80_Device *)&pAppMd80->pMd80[0], pAppMd80->pMd80[0]->input.torque);
      md80_SetTargetVelocity((tMd80_Device *)&pAppMd80->pMd80[0], pAppMd80->pMd80[0]->input.velocity);
      md80_SetTargetPosition((tMd80_Device *)&pAppMd80->pMd80[0], pAppMd80->pMd80[0]->input.position);

      Obj.md80_0_DataReturn.enabled = true;
      Obj.md80_0_DataReturn.Mode = pAppMd80->pMd80[0]->local.mode;
      Obj.md80_0_DataReturn.Position = MD80_REAL_TO_ETH(pAppMd80->pMd80[0]->output.position);
      Obj.md80_0_DataReturn.Temperature = MD80_REAL_TO_ETH(pAppMd80->pMd80[0]->output.temperature);
      Obj.md80_0_DataReturn.Torque = MD80_REAL_TO_ETH(pAppMd80->pMd80[0]->output.torque);
      Obj.md80_0_DataReturn.Velocity = MD80_REAL_TO_ETH(pAppMd80->pMd80[0]->output.velocity);
   }
   else
   {
      Obj.md80_0_DataReturn.enabled = false;
   }

   /* MD80_DEV_1. */
   if (true == pAppMd80->pMd80[1]->local.isEnabled)
   {
      /* Update the data control for md80. */
      pAppMd80->pMd80[1]->input.velocity = MD80_ETH_TO_REAL(Obj.md80_1_DataControl.Velocity);
      pAppMd80->pMd80[1]->input.position = MD80_ETH_TO_REAL(Obj.md80_1_DataControl.Position);
      pAppMd80->pMd80[1]->input.torque = MD80_ETH_TO_REAL(Obj.md80_1_DataControl.Torque);

      md80_SetTorque((tMd80_Device *)&pAppMd80->pMd80[1], pAppMd80->pMd80[1]->input.torque);
      md80_SetTargetVelocity((tMd80_Device *)&pAppMd80->pMd80[1], pAppMd80->pMd80[1]->input.velocity);
      md80_SetTargetPosition((tMd80_Device *)&pAppMd80->pMd80[1], pAppMd80->pMd80[1]->input.position);

      Obj.md80_1_DataReturn.enabled = true;
      Obj.md80_1_DataReturn.Mode = pAppMd80->pMd80[1]->local.mode;
      Obj.md80_1_DataReturn.Position = MD80_REAL_TO_ETH(pAppMd80->pMd80[1]->output.position);
      Obj.md80_1_DataReturn.Temperature = MD80_REAL_TO_ETH(pAppMd80->pMd80[1]->output.temperature);
      Obj.md80_1_DataReturn.Torque = MD80_REAL_TO_ETH(pAppMd80->pMd80[1]->output.torque);
      Obj.md80_1_DataReturn.Velocity = MD80_REAL_TO_ETH(pAppMd80->pMd80[1]->output.velocity);
   }
   else
   {
      Obj.md80_1_DataReturn.enabled = false;
   }

   /* MD80_DEV_2. */
   if (true == pAppMd80->pMd80[2]->local.isEnabled)
   {
      /* Update the data control for md80. */
      pAppMd80->pMd80[3]->input.velocity = MD80_ETH_TO_REAL(Obj.md80_2_DataControl.Velocity);
      pAppMd80->pMd80[3]->input.position = MD80_ETH_TO_REAL(Obj.md80_2_DataControl.Position);
      pAppMd80->pMd80[3]->input.torque = MD80_ETH_TO_REAL(Obj.md80_2_DataControl.Torque);

      md80_SetTorque((tMd80_Device *)&pAppMd80->pMd80[2], pAppMd80->pMd80[2]->input.torque);
      md80_SetTargetVelocity((tMd80_Device *)&pAppMd80->pMd80[2], pAppMd80->pMd80[2]->input.velocity);
      md80_SetTargetPosition((tMd80_Device *)&pAppMd80->pMd80[2], pAppMd80->pMd80[2]->input.position);

      Obj.md80_2_DataReturn.enabled = true;
      Obj.md80_2_DataReturn.Mode = pAppMd80->pMd80[2]->local.mode;
      Obj.md80_2_DataReturn.Position = MD80_REAL_TO_ETH(pAppMd80->pMd80[2]->output.position);
      Obj.md80_2_DataReturn.Temperature = MD80_REAL_TO_ETH(pAppMd80->pMd80[2]->output.temperature);
      Obj.md80_2_DataReturn.Torque = MD80_REAL_TO_ETH(pAppMd80->pMd80[2]->output.torque);
      Obj.md80_2_DataReturn.Velocity = MD80_REAL_TO_ETH(pAppMd80->pMd80[2]->output.velocity);
   }
   else
   {
      Obj.md80_2_DataReturn.enabled = false;
   }
   
   /* MD80_DEV_0. */
   if (true == pAppMd80->pMd80[3]->local.isEnabled)
   {
      /* Update the data control for md80. */
      pAppMd80->pMd80[3]->input.velocity = MD80_ETH_TO_REAL(Obj.md80_3_DataControl.Velocity);
      pAppMd80->pMd80[3]->input.position = MD80_ETH_TO_REAL(Obj.md80_3_DataControl.Position);
      pAppMd80->pMd80[3]->input.torque = MD80_ETH_TO_REAL(Obj.md80_3_DataControl.Torque);

      md80_SetTorque((tMd80_Device *)&pAppMd80->pMd80[3], pAppMd80->pMd80[3]->input.torque);
      md80_SetTargetVelocity((tMd80_Device *)&pAppMd80->pMd80[3], pAppMd80->pMd80[3]->input.velocity);
      md80_SetTargetPosition((tMd80_Device *)&pAppMd80->pMd80[3], pAppMd80->pMd80[3]->input.position);

      Obj.md80_3_DataReturn.enabled = true;
      Obj.md80_3_DataReturn.Mode = pAppMd80->pMd80[3]->local.mode;
      Obj.md80_3_DataReturn.Position = MD80_REAL_TO_ETH(pAppMd80->pMd80[3]->output.position);
      Obj.md80_3_DataReturn.Temperature = MD80_REAL_TO_ETH(pAppMd80->pMd80[3]->output.temperature);
      Obj.md80_3_DataReturn.Torque = MD80_REAL_TO_ETH(pAppMd80->pMd80[3]->output.torque);
      Obj.md80_3_DataReturn.Velocity = MD80_REAL_TO_ETH(pAppMd80->pMd80[3]->output.velocity);
   }
   else
   {
      Obj.md80_3_DataReturn.enabled = false;
   }

}

static void App_Md80_EventHandle (tAppMd80_Data * const app)
{
   /** Determine if there has new command. */
   if (true == app->isNewCommand)
   {
      /* Allow change to Command state when this app is in Idle and Control states. */
      if (app->state == APP_MD80_IDLE)
      {
         /* Update next state to Command. */
         app->state = APP_MD80_COMMAND_ENTER;

         /* Clear the flag.*/
         app->isNewCommand = false;
      }
   }
   else 
   {
      /* Keep is in Control state. */
   }
}

static void App_Md80_Command_Enter (tAppMd80_Data * const app)
{
   tAppMd80_States nextState = APP_MD80_COMMAND_ENTER;

   /* Copy data to local. */
   memcpy ((void *)&app->cmd, (void *)&app->hostReq, sizeof(tAppMd80_Msg));

   /* Go to next state. */
   nextState = APP_MD80_COMMAND;

   /* Update next state. */
   app->state = nextState;
}

static void App_Md80_Command (tAppMd80_Data * const app)
{   
   tAppMd80_States nextState = APP_MD80_COMMAND;

   switch (app->cmd.frId)
   {
   case BUS_FRAME_NONE:
      /* code */
      break;

   case BUS_FRAME_PING_START:
      /* Scan device on bus can. */
      App_Md80_ScanDevice(app);
      break;

   case BUS_FRAME_MD80_ADD:
      /* Add md80 with id. */
      App_Md80_AddMd80(app);
      break;

   case BUS_FRAME_MD80_GENERIC_FRAME:
      /* Send data configuration. */
      App_Md80_GenericFrame (app);
      break;

   case BUS_FRAME_BEGIN:
      App_Md80_Begin(app);
      break;
   
   case BUS_FRAME_END:
      App_Md80_End(app);
      break;
   
   case BUS_FRAME_RESET:
      /* FIXME: Update the function for this bus frame. */
      break;
   
   default:
      break;
   }

   /* Go to next state after finished handling the command. */
   nextState = APP_MD80_COMMAND_EXIT;

   /* Update state. */
   app->state = nextState;
}

static void App_Md80_Command_Exit (tAppMd80_Data * const app)
{
   tAppMd80_States nextState = APP_MD80_COMMAND_EXIT;

   /* Enter critical section. */
   taskENTER_CRITICAL();

   /* Update the frame id and motor id. */
   app->rsp.frId = app->cmd.frId;
   app->rsp.motorId = app->cmd.motorId;

   /* Call function interrupt when it */
   App_Md80_UpdateResp(app);

   /* Exit critical section. */
   taskEXIT_CRITICAL();

   /* Go back to Idle state. */
   nextState = APP_MD80_IDLE;
   
   /* Update state. */
   app->state = nextState;
}

static void App_Md80_Control (tAppMd80_Data * const app)
{
   uint8_t iter = 0u;

   for (iter = 0u; iter < app->numMd80Det; iter++)
   {
      md80_MainFunction ((tMd80_Device *)app->pMd80[iter]);
   }
}

static void App_Md80_UpdateResp (tAppMd80_Data * const app)
{
   uint8_t iter = 0u;
   uint8_t md80Id = 0u;
   if (app->rsp.frId == BUS_FRAME_PING_START)
   {
      /* Copy list address of md80. */
      if (app->numMd80Det > 0u)
      {
         for (iter = 0u; iter < app->numMd80Det; iter++)
         {
            Obj.md80_addrs[iter] = (uint32_t)app->rsp.data[iter*2u] + ((uint32_t)app->rsp.data[(iter*2u) + 1u] << 8);
         }
      }
      else 
      {

      }
   }
   /* Another command. */
   else 
   {
      md80Id = app->cmd.motorId;
      memcpy ((void *)&app->rsp.data[0], (void *)&app->pMd80[md80Id]->output.errorVector, sizeof(uint16_t));
      memcpy ((void *)&app->rsp.data[2], (void *)&app->pMd80[md80Id]->output.temperature, sizeof(uint8_t));
      memcpy ((void *)&app->rsp.data[3], (void *)&app->pMd80[md80Id]->output.position, sizeof(float));
      memcpy ((void *)&app->rsp.data[7], (void *)&app->pMd80[md80Id]->output.velocity, sizeof(float));
      memcpy ((void *)&app->rsp.data[11], (void *)&app->pMd80[md80Id]->output.torque, sizeof(float));
      app->rsp.size = 14u;

      /* Enter critical section. */
      taskENTER_CRITICAL();
      
      /* Update to object. */
      Obj.md80_Respond[0] = md80Id;
      Obj.md80_Respond[1] = app->rsp.frId;
      Obj.md80_Respond[2] = app->rsp.size;
      memcpy((void *)&Obj.md80_Respond[3], (void *)&app->rsp.data[0], app->rsp.size);

      /* Exit critical section. */
      taskEXIT_CRITICAL();
   }
}

void App_Md80_UpdateCmd (uint8_t md80id, uint8_t command, uint8_t size, uint8_t * cmd)
{
   bool isMd80IdExist = false;

   /* TODO: Determine whether id exist in list. */
   isMd80IdExist = true;

   if (true == isMd80IdExist)
   {
      /* Enter critical section. */
      taskENTER_CRITICAL();

      /* Save the cmd. */
      pAppMd80->hostReq.motorId = md80id;
      pAppMd80->hostReq.frId = command;
      pAppMd80->hostReq.size = size;
      memcpy(pAppMd80->hostReq.data, cmd, size);

      /* Set flag determine it having new command.*/
      pAppMd80->isNewCommand = true;

      /* Exit critical section. */
      taskEXIT_CRITICAL();
   }
   else 
   {
      /* Do nothing. */
   }
}

void App_Md80_Init()
{
   uint8_t iter = 0u;

   /* Create the data structure. */
   pAppMd80 = (tAppMd80_Data *)pvPortMalloc(sizeof(tAppMd80_Data));

   /* Create the data structure for md80s. */
   tMd80_Device * pMd80_Devs = (tMd80_Device *)pvPortMalloc(sizeof(tMd80_Device) * APP_MD80_TOTAL_DEV_SUPPORT);

   /* Create the data structures for */
   if ((NULL == pAppMd80) || (NULL == pMd80_Devs))
   {
      /* Can not allocate the data structure for this app. */
      /**FIXME - Alert and stop this app. */
   }
   else
   {
      /* Set all the values of data structures md80 to default value. */
      bzero(pMd80_Devs, sizeof(tMd80_Device) * APP_MD80_TOTAL_DEV_SUPPORT);

      for (iter = 0u; iter < APP_MD80_TOTAL_DEV_SUPPORT; iter++)
      {
         pAppMd80->pMd80[iter] = (tMd80_Device *)&pMd80_Devs[iter];
      }
   }
}

void App_Md80_Deinit()
{
   vPortFree (pAppMd80->pMd80);
   vPortFree (pAppMd80);
}

void App_Md80_MainFunction()
{
   App_Md80_EventHandle (pAppMd80);
   
   /* Run mode. */
   switch (pAppMd80->state)
   {
   case APP_MD80_IDLE:
      /* code */
      break;

   case APP_MD80_COMMAND_ENTER:
      App_Md80_Command_Enter(pAppMd80);
      break;

   case APP_MD80_COMMAND:
      App_Md80_Command(pAppMd80);
      break;

   case APP_MD80_COMMAND_EXIT:
      App_Md80_Command_Exit(pAppMd80);
      break;

   default:
      break;
   }

   if (APP_MD80_RUN == pAppMd80->workState)
   {
      App_Md80_Control (pAppMd80);
   }
}

void AppMd80_Task (void const *argument)
{
   /* USER CODE BEGIN md80sTask */
   App_Md80_Init();

   /* Infinite loop */
   for (;;)
   {
      /* Run the main function of this app. */
      App_Md80_MainFunction();

      /* Update data control. */
      App_Md80_UpdateDataControl();

      /* Delay task. */
      osDelay(APP_MD80_TASK_DELAY);
   }
   /* USER CODE END md80sTask */
}
