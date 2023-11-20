/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define RX_BUFFER_SIZE 256
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Ver_D0_Pin GPIO_PIN_13
#define Ver_D0_GPIO_Port GPIOC
#define Ver_D1_Pin GPIO_PIN_14
#define Ver_D1_GPIO_Port GPIOC
#define Ver_A_Pin GPIO_PIN_0
#define Ver_A_GPIO_Port GPIOC
#define ETC_SYNC1_Pin GPIO_PIN_0
#define ETC_SYNC1_GPIO_Port GPIOA
#define ETC_SYNC0_Pin GPIO_PIN_1
#define ETC_SYNC0_GPIO_Port GPIOA
#define ETC_IRQ_Pin GPIO_PIN_2
#define ETC_IRQ_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_4
#define LED_R_GPIO_Port GPIOC
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LAN_Reset_CMD_Pin GPIO_PIN_11
#define LAN_Reset_CMD_GPIO_Port GPIOB
#define EXT_LED_Pin GPIO_PIN_5
#define EXT_LED_GPIO_Port GPIOB
#define FDCAN_RX_Pin GPIO_PIN_8
#define FDCAN_RX_GPIO_Port GPIOB
#define FDCAN_TX_Pin GPIO_PIN_9
#define FDCAN_TX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

typedef enum {
	Debug =0,
	ETHCat,
	canIdle,
	canIdle_scan,
	canIde_bringUp_motor,
	canM,
	canM_command,
	canM_response,


}messageOrigin;

typedef struct {
	uint64_t timestamp; // in nanoseconds
	uint32_t params[5];
	messageOrigin origin;


}logMessageStruct;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
