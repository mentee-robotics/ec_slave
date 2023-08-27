/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "uartDma.h"
#include "lan9252.h"
#include "ethCat.h"
#include "canM.h"
#include "canIdle.h"
#include "appTest.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId appTestHandle;
osThreadId ethCatHandle;
osThreadId canMHandle;
osThreadId canIdleHandle;
osTimerId timerCounter500usHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void appTestTask(void const * argument);
void ethCatTask(void const * argument);
void canMTask(void const * argument);
void canIdleTask(void const * argument);
void timerCounterCb(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);
void vApplicationDaemonTaskStartupHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN DAEMON_TASK_STARTUP_HOOK */
void vApplicationDaemonTaskStartupHook(void)
{
  /* Initialize the uartDma driver. */
  uartDma_init();

  /* Initialize the lan9252 driver. */
  lan9252_Init();
}
/* USER CODE END DAEMON_TASK_STARTUP_HOOK */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of timerCounter500us */
  osTimerDef(timerCounter500us, timerCounterCb);
  timerCounter500usHandle = osTimerCreate(osTimer(timerCounter500us), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  xTimerStop(timerCounter500usHandle, pdMS_TO_TICKS(1));
  xTimerChangePeriod(timerCounter500usHandle, 1, 100);
  xTimerStart(timerCounter500usHandle, pdMS_TO_TICKS(1));

  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of appTest */
  osThreadDef(appTest, appTestTask, osPriorityLow, 0, 128);
  appTestHandle = osThreadCreate(osThread(appTest), NULL);

  /* definition and creation of ethCat */
  osThreadDef(ethCat, ethCatTask, osPriorityHigh, 0, 1024);
  ethCatHandle = osThreadCreate(osThread(ethCat), NULL);

  /* definition and creation of canM */
  osThreadDef(canM, canMTask, osPriorityHigh, 0, 1024);
  canMHandle = osThreadCreate(osThread(canM), NULL);

  /* definition and creation of canIdle */
  osThreadDef(canIdle, canIdleTask, osPriorityHigh, 0, 512);
  canIdleHandle = osThreadCreate(osThread(canIdle), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_appTestTask */
/**
  * @brief  Function implementing the appTest thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_appTestTask */
void appTestTask(void const * argument)
{
  /* USER CODE BEGIN appTestTask */
  AppTest_Init();

  /* Infinite loop */
  for(;;)
  {
    AppTest_MainFunction();

    osDelay(1);
  }
  /* USER CODE END appTestTask */
}

/* USER CODE BEGIN Header_ethCatTask */
/**
* @brief Function implementing the ethCat thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ethCatTask */
void ethCatTask(void const * argument)
{
  /* USER CODE BEGIN ethCatTask */
  /* Initialize the modules which used by this application. */
  ethCat_Init();

  /* Infinite loop */
  for(;;)
  {
    /* Run main function of ethCat. */
    ethCat_MainFunction();

    osDelay(1);
  }
  /* USER CODE END ethCatTask */
}

/* USER CODE BEGIN Header_canMTask */
/**
* @brief Function implementing the canM thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_canMTask */
void canMTask(void const * argument)
{
  /* USER CODE BEGIN canMTask */
  canM_Init (&canM_Module);

  /* Infinite loop */
  for(;;)
  {
    /* Run main function of canM. */
    canM_MainFunction (&canM_Module);

    osDelay(1);
  }
  /* USER CODE END canMTask */
}

/* USER CODE BEGIN Header_canIdleTask */
/**
* @brief Function implementing the canIdle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_canIdleTask */
void canIdleTask(void const * argument)
{
  /* USER CODE BEGIN canIdleTask */
  canIdle_Init();

  /* Infinite loop */
  for(;;)
  {
    /* Run the main function of CanIdle. */
    canIdle_MainFunction();

    osDelay(1);
  }
  /* USER CODE END canIdleTask */
}

/* timerCounterCb function */
__weak void timerCounterCb(void const * argument)
{
  /* USER CODE BEGIN timerCounterCb */

  /* USER CODE END timerCounterCb */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

