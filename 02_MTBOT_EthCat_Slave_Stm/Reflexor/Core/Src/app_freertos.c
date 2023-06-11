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
osThreadId appTestTaskHandle;
osThreadId ethCatSlvTaskHandle;
osThreadId canIdleHandle;
osThreadId appMd80Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void testTask(void const * argument);
void ethCat_Task(void const * argument);
void canIdleTask(void const * argument);
void AppMd80_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

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

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of appTestTask */
//  osThreadDef(appTestTask, testTask, osPriorityNormal, 0, 128);
//  appTestTaskHandle = osThreadCreate(osThread(appTestTask), NULL);

  /* definition and creation of ethCatSlvTask */
  osThreadDef(ethCatSlvTask, ethCat_Task, osPriorityHigh, 0, 512);
  ethCatSlvTaskHandle = osThreadCreate(osThread(ethCatSlvTask), NULL);

  /* definition and creation of canIdle */
  osThreadDef(canIdle, canIdleTask, osPriorityHigh, 0, 512);
  canIdleHandle = osThreadCreate(osThread(canIdle), NULL);

  /* definition and creation of appMd80 */
  osThreadDef(appMd80, AppMd80_Task, osPriorityNormal, 0, 1424);
  appMd80Handle = osThreadCreate(osThread(appMd80), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_testTask */
/**
  * @brief  Function implementing the appTestTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_testTask */
__weak void testTask(void const * argument)
{
  /* USER CODE BEGIN testTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END testTask */
}

/* USER CODE BEGIN Header_ethCat_Task */
/**
* @brief Function implementing the ethCatSlvTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ethCat_Task */
__weak void ethCat_Task(void const * argument)
{
  /* USER CODE BEGIN ethCat_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ethCat_Task */
}

/* USER CODE BEGIN Header_canIdleTask */
/**
* @brief Function implementing the canIdle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_canIdleTask */
__weak void canIdleTask(void const * argument)
{
  /* USER CODE BEGIN canIdleTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END canIdleTask */
}

/* USER CODE BEGIN Header_AppMd80_Task */
/**
* @brief Function implementing the appMd80 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppMd80_Task */
__weak void AppMd80_Task(void const * argument)
{
  /* USER CODE BEGIN AppMd80_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AppMd80_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

