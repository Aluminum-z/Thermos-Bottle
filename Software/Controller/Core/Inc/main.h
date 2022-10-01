/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;

extern SPI_HandleTypeDef hspi2;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim21;
extern TIM_HandleTypeDef htim22;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TE_SLEEP_Pin GPIO_PIN_13
#define TE_SLEEP_GPIO_Port GPIOC
#define TEMP2_Pin GPIO_PIN_0
#define TEMP2_GPIO_Port GPIOH
#define TEMP1_Pin GPIO_PIN_1
#define TEMP1_GPIO_Port GPIOH
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_2
#define LCD_RS_GPIO_Port GPIOB
#define FAN_EN_Pin GPIO_PIN_11
#define FAN_EN_GPIO_Port GPIOB
#define KEY_MODE_Pin GPIO_PIN_12
#define KEY_MODE_GPIO_Port GPIOB
#define KEY_ADD_Pin GPIO_PIN_8
#define KEY_ADD_GPIO_Port GPIOA
#define KEY_SUB_Pin GPIO_PIN_9
#define KEY_SUB_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
