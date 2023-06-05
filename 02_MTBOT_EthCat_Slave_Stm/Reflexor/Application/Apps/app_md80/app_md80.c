/**
 * @file app_md80.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "app_md80.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define MD80_ETH_TO_REAL(x) (x / 100.0f)
#define MD80_REAL_TO_ETH(x) (x * 100.0f)

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static tAppMd80_Data app_md80;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

static void App_Md80_UpdateDataControl(void)
{
   /* MD80_DEV_0. */
   if (true == Obj.md80_0_DataControl.enable)
   {
      /* Update the data control for md80. */
      md80_Dev[0].input.velocity = MD80_ETH_TO_REAL(Obj.md80_0_DataControl.Velocity);
      md80_Dev[0].input.position = MD80_ETH_TO_REAL(Obj.md80_0_DataControl.Position);
      md80_Dev[0].input.torque = MD80_ETH_TO_REAL(Obj.md80_0_DataControl.Torque);

      md80_SetTorque(&md80_Dev[0], md80_Dev[0].input.torque);
      md80_SetTargetVelocity(&md80_Dev[0], md80_Dev[0].input.velocity);
      md80_SetTargetPosition(&md80_Dev[0], md80_Dev[0].input.position);

      Obj.md80_0_DataReturn.enabled = true;
      Obj.md80_0_DataReturn.Mode = md80_Dev[0].local.mode;
      Obj.md80_0_DataReturn.Position = MD80_REAL_TO_ETH(md80_Dev[0].output.position);
      Obj.md80_0_DataReturn.Temperature = MD80_REAL_TO_ETH(md80_Dev[0].output.temperature);
      Obj.md80_0_DataReturn.Torque = MD80_REAL_TO_ETH(md80_Dev[0].output.torque);
      Obj.md80_0_DataReturn.Velocity = MD80_REAL_TO_ETH(md80_Dev[0].output.velocity);

      if (Obj.md80_0_DataControl.Mode != md80_Dev[0].input.mode)
      {
         md80_Dev[0].input.mode = Obj.md80_0_DataControl.Mode;

         switch (md80_Dev[0].input.mode)
         {
         case MD80_IDLE:
            md80_ControlMd80Mode(&md80_Dev[0], MD80_IDLE);
            break;
         case MD80_POSITION_PID:
            md80_ControlMd80Mode(&md80_Dev[0], MD80_POSITION_PID);

            break;

         case MD80_VELOCITY_PID:
            md80_ControlMd80Mode(&md80_Dev[0], MD80_VELOCITY_PID);

            break;

         case MD80_DEPRECATED:
            md80_ControlMd80Mode(&md80_Dev[0], MD80_DEPRECATED);

            break;

         case MD80_IMPEDANCE:
            md80_ControlMd80Mode(&md80_Dev[0], MD80_IMPEDANCE);
            md80_SetImpedanceControllerParams(&md80_Dev[0], md80_Dev[0].config.impedanceController.kp, md80_Dev[0].config.impedanceController.kd);
            break;

         default:
            break;
         }
      }

      if ((false == md80_Dev[0].local.isEnabled) && (true == md80_Dev[0].config.isMd80Detected))
      {
         /* Reset encoder at current position */
         md80_SetEncoderZero(&md80_Dev[0]);

         /* Enable the drive */
         md80_ControlMd80Enable(&md80_Dev[0], true);

         /* Set max torque: 50 Nm. */
         md80_SetMaxTorque(&md80_Dev[0], md80_Dev[0].config.torqueMax);
         md80_SetMaxVelocity(&md80_Dev[0], md80_Dev[0].config.velocityMax);
         md80_SetCurrentLimit(&md80_Dev[0], md80_Dev[0].config.currentMax);

         md80_Dev[0].local.isEnabled = true;
      }
   }
   else
   {
      Obj.md80_0_DataReturn.enabled = false;
      md80_Dev[0].local.isEnabled = false;
   }

   /* MD80_DEV_1. */
   if (true == Obj.md80_1_DataControl.enable)
   {
      /* Update the data control for md80. */
      md80_Dev[1].input.velocity = Obj.md80_1_DataControl.Velocity;
      md80_Dev[1].input.position = Obj.md80_1_DataControl.Position;
      md80_Dev[1].input.torque = Obj.md80_1_DataControl.Torque;

      md80_SetTorque(&md80_Dev[1], md80_Dev[1].input.torque);
      md80_SetTargetVelocity(&md80_Dev[1], md80_Dev[1].input.velocity);
      md80_SetTargetPosition(&md80_Dev[1], md80_Dev[1].input.position);

      Obj.md80_1_DataReturn.enabled = true;
      Obj.md80_1_DataReturn.Mode = md80_Dev[1].local.mode;
      Obj.md80_1_DataReturn.Position = MD80_REAL_TO_ETH(md80_Dev[1].output.position);
      Obj.md80_1_DataReturn.Temperature = MD80_REAL_TO_ETH(md80_Dev[1].output.temperature);
      Obj.md80_1_DataReturn.Torque = MD80_REAL_TO_ETH(md80_Dev[1].output.torque);
      Obj.md80_1_DataReturn.Velocity = MD80_REAL_TO_ETH(md80_Dev[1].output.velocity);

      if (Obj.md80_1_DataControl.Mode != md80_Dev[1].input.mode)
      {
         md80_Dev[1].input.mode = Obj.md80_1_DataControl.Mode;

         switch (md80_Dev[1].input.mode)
         {
         case MD80_IDLE:
            md80_ControlMd80Mode(&md80_Dev[1], MD80_IDLE);
            break;
         case MD80_POSITION_PID:
            md80_ControlMd80Mode(&md80_Dev[1], MD80_POSITION_PID);

            break;

         case MD80_VELOCITY_PID:
            md80_ControlMd80Mode(&md80_Dev[1], MD80_VELOCITY_PID);

            break;

         case MD80_DEPRECATED:
            md80_ControlMd80Mode(&md80_Dev[1], MD80_DEPRECATED);

            break;

         case MD80_IMPEDANCE:
            md80_ControlMd80Mode(&md80_Dev[1], MD80_IMPEDANCE);
            md80_SetImpedanceControllerParams(&md80_Dev[1], md80_Dev[1].config.impedanceController.kp, md80_Dev[1].config.impedanceController.kd);
            break;

         default:
            break;
         }
      }

      if ((false == md80_Dev[1].local.isEnabled) && (true == md80_Dev[1].config.isMd80Detected))
      {
         /* Reset encoder at current position */
         md80_SetEncoderZero(&md80_Dev[1]);

         /* Enable the drive */
         md80_ControlMd80Enable(&md80_Dev[1], true);

         /* Set max torque: 50 Nm. */
         md80_SetMaxTorque(&md80_Dev[1], md80_Dev[1].config.torqueMax);
         md80_SetMaxVelocity(&md80_Dev[1], md80_Dev[1].config.velocityMax);
         md80_SetCurrentLimit(&md80_Dev[1], md80_Dev[1].config.currentMax);

         md80_Dev[1].local.isEnabled = true;
      }
   }
   else
   {
      Obj.md80_1_DataReturn.enabled = false;
      md80_Dev[1].local.isEnabled = false;
   }

   /* MD80_DEV_2. */
   if (true == Obj.md80_2_DataControl.enable)
   {
      /* Update the data control for md80. */
      md80_Dev[2].input.velocity = Obj.md80_2_DataControl.Velocity;
      md80_Dev[2].input.position = Obj.md80_2_DataControl.Position;
      md80_Dev[2].input.torque = Obj.md80_2_DataControl.Torque;

      md80_SetTorque(&md80_Dev[2], md80_Dev[2].input.torque);
      md80_SetTargetVelocity(&md80_Dev[2], md80_Dev[2].input.velocity);
      md80_SetTargetPosition(&md80_Dev[2], md80_Dev[2].input.position);

      Obj.md80_2_DataReturn.enabled = true;
      Obj.md80_2_DataReturn.Mode = md80_Dev[2].local.mode;
      Obj.md80_2_DataReturn.Position = MD80_REAL_TO_ETH(md80_Dev[2].output.position);
      Obj.md80_2_DataReturn.Temperature = MD80_REAL_TO_ETH(md80_Dev[2].output.temperature);
      Obj.md80_2_DataReturn.Torque = MD80_REAL_TO_ETH(md80_Dev[2].output.torque);
      Obj.md80_2_DataReturn.Velocity = MD80_REAL_TO_ETH(md80_Dev[2].output.velocity);

      if (Obj.md80_2_DataControl.Mode != md80_Dev[2].input.mode)
      {
         md80_Dev[2].input.mode = Obj.md80_2_DataControl.Mode;

         switch (md80_Dev[2].input.mode)
         {
         case MD80_IDLE:
            md80_ControlMd80Mode(&md80_Dev[2], MD80_IDLE);
            break;
         case MD80_POSITION_PID:
            md80_ControlMd80Mode(&md80_Dev[2], MD80_POSITION_PID);

            break;

         case MD80_VELOCITY_PID:
            md80_ControlMd80Mode(&md80_Dev[2], MD80_VELOCITY_PID);

            break;

         case MD80_DEPRECATED:
            md80_ControlMd80Mode(&md80_Dev[2], MD80_DEPRECATED);

            break;

         case MD80_IMPEDANCE:
            md80_ControlMd80Mode(&md80_Dev[2], MD80_IMPEDANCE);
            md80_SetImpedanceControllerParams(&md80_Dev[2], md80_Dev[2].config.impedanceController.kp, md80_Dev[1].config.impedanceController.kd);
            break;

         default:
            break;
         }
      }

      if ((false == md80_Dev[2].local.isEnabled) && (true == md80_Dev[2].config.isMd80Detected))
      {
         /* Reset encoder at current position */
         md80_SetEncoderZero(&md80_Dev[2]);

         /* Enable the drive */
         md80_ControlMd80Enable(&md80_Dev[2], true);

         /* Set max torque: 50 Nm. */
         md80_SetMaxTorque(&md80_Dev[2], md80_Dev[2].config.torqueMax);
         md80_SetMaxVelocity(&md80_Dev[2], md80_Dev[2].config.velocityMax);
         md80_SetCurrentLimit(&md80_Dev[2], md80_Dev[2].config.currentMax);

         md80_Dev[2].local.isEnabled = true;
      }
   }
   else
   {
      Obj.md80_2_DataReturn.enabled = false;
      md80_Dev[2].local.isEnabled = false;
   }

   /* MD80_DEV_3. */
   if (true == Obj.md80_3_DataControl.enable)
   {
      /* Update the data control for md80. */
      md80_Dev[3].input.velocity = Obj.md80_3_DataControl.Velocity;
      md80_Dev[3].input.position = Obj.md80_3_DataControl.Position;
      md80_Dev[3].input.torque = Obj.md80_3_DataControl.Torque;

      md80_SetTorque(&md80_Dev[3], md80_Dev[3].input.torque);
      md80_SetTargetVelocity(&md80_Dev[3], md80_Dev[3].input.velocity);
      md80_SetTargetPosition(&md80_Dev[3], md80_Dev[3].input.position);

      Obj.md80_3_DataReturn.enabled = true;
      Obj.md80_3_DataReturn.Mode = md80_Dev[3].local.mode;
      Obj.md80_3_DataReturn.Position = MD80_REAL_TO_ETH(md80_Dev[3].output.position);
      Obj.md80_3_DataReturn.Temperature = MD80_REAL_TO_ETH(md80_Dev[3].output.temperature);
      Obj.md80_3_DataReturn.Torque = MD80_REAL_TO_ETH(md80_Dev[3].output.torque);
      Obj.md80_3_DataReturn.Velocity = MD80_REAL_TO_ETH(md80_Dev[3].output.velocity);

      if (Obj.md80_3_DataControl.Mode != md80_Dev[3].input.mode)
      {
         md80_Dev[3].input.mode = Obj.md80_3_DataControl.Mode;

         switch (md80_Dev[3].input.mode)
         {
         case MD80_IDLE:
            md80_ControlMd80Mode(&md80_Dev[3], MD80_IDLE);
            break;
         case MD80_POSITION_PID:
            md80_ControlMd80Mode(&md80_Dev[3], MD80_POSITION_PID);

            break;

         case MD80_VELOCITY_PID:
            md80_ControlMd80Mode(&md80_Dev[3], MD80_VELOCITY_PID);

            break;

         case MD80_DEPRECATED:
            md80_ControlMd80Mode(&md80_Dev[3], MD80_DEPRECATED);

            break;

         case MD80_IMPEDANCE:
            md80_ControlMd80Mode(&md80_Dev[3], MD80_IMPEDANCE);
            md80_SetImpedanceControllerParams(&md80_Dev[3], md80_Dev[3].config.impedanceController.kp, md80_Dev[1].config.impedanceController.kd);
            break;

         default:
            break;
         }
      }

      if ((false == md80_Dev[3].local.isEnabled) && (true == md80_Dev[3].config.isMd80Detected))
      {
         /* Reset encoder at current position */
         md80_SetEncoderZero(&md80_Dev[3]);

         /* Enable the drive */
         md80_ControlMd80Enable(&md80_Dev[3], true);

         /* Set max torque: 50 Nm. */
         md80_SetMaxTorque(&md80_Dev[3], md80_Dev[3].config.torqueMax);
         md80_SetMaxVelocity(&md80_Dev[3], md80_Dev[3].config.velocityMax);
         md80_SetCurrentLimit(&md80_Dev[3], md80_Dev[3].config.currentMax);

         md80_Dev[3].local.isEnabled = true;
      }
   }
   else
   {
      Obj.md80_3_DataReturn.enabled = false;
      md80_Dev[3].local.isEnabled = false;
   }
}

static void App_Md80_SubMotorControl(tMd80_Device *const dev)
{
}

static void App_Md80_Idle(tMd80_Device *const app)
{
}

static void App_Md80_NewCommand(tAppMd80_Data *const app)
{
}

static void App_Md80_ProcessEnter(tAppMd80_Data *const app)
{
}

static void App_Md80_Process(tAppMd80_Data *const app)
{
}

static void App_Md80_Process_Exit(tAppMd80_Data *const app)
{
}

void App_Md80_Init()
{
   /* Start scan the md80 on bus can. */
   md80_Scan();
}

void App_Md80_MainFunction()
{
   uint8_t iter = 0U;

   for (iter = 0U; iter < MD80_NUM_DEV; iter++)
   {
      md80_MainFunction((tMd80_Device *)&md80_Dev[iter]);
   }
}

void md80sTask(void const *argument)
{
   /* USER CODE BEGIN md80sTask */
   App_Md80_Init();

   /* Infinite loop */
   for (;;)
   {
      /* Update the data communication between ethCat and md80s. */
      App_Md80_UpdateDataControl();

      /* Run the main function of this app. */
      App_Md80_MainFunction();

      /* Delay task. */
      osDelay(1);
   }
   /* USER CODE END md80sTask */
}
