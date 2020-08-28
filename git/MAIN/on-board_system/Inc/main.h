/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdint.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define RX_MAX_BUFFER_SIZE 8

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OUT5_Pin GPIO_PIN_13
#define OUT5_GPIO_Port GPIOC
#define OUT6_Pin GPIO_PIN_14
#define OUT6_GPIO_Port GPIOC
#define OUT7_Pin GPIO_PIN_15
#define OUT7_GPIO_Port GPIOC
#define ENA0_Pin GPIO_PIN_0
#define ENA0_GPIO_Port GPIOC
#define ENB0_Pin GPIO_PIN_1
#define ENB0_GPIO_Port GPIOC
#define ENA1_Pin GPIO_PIN_2
#define ENA1_GPIO_Port GPIOC
#define ENB1_Pin GPIO_PIN_3
#define ENB1_GPIO_Port GPIOC
#define PWM0_Pin GPIO_PIN_0
#define PWM0_GPIO_Port GPIOA
#define INA0_Pin GPIO_PIN_1
#define INA0_GPIO_Port GPIOA
#define INB0_Pin GPIO_PIN_2
#define INB0_GPIO_Port GPIOA
#define CS0_Pin GPIO_PIN_3
#define CS0_GPIO_Port GPIOA
#define LORA_SCK_Pin GPIO_PIN_5
#define LORA_SCK_GPIO_Port GPIOA
#define LORA_MISO_Pin GPIO_PIN_6
#define LORA_MISO_GPIO_Port GPIOA
#define LORA_MOSI_Pin GPIO_PIN_7
#define LORA_MOSI_GPIO_Port GPIOA
#define OUT0_Pin GPIO_PIN_4
#define OUT0_GPIO_Port GPIOC
#define OUT1_Pin GPIO_PIN_5
#define OUT1_GPIO_Port GPIOC
#define S1_Pin GPIO_PIN_0
#define S1_GPIO_Port GPIOB
#define S2_Pin GPIO_PIN_1
#define S2_GPIO_Port GPIOB
#define S3_Pin GPIO_PIN_2
#define S3_GPIO_Port GPIOB
#define S4_Pin GPIO_PIN_10
#define S4_GPIO_Port GPIOB
#define S5_Pin GPIO_PIN_11
#define S5_GPIO_Port GPIOB
#define START_Pin GPIO_PIN_12
#define START_GPIO_Port GPIOB
#define START_EXTI_IRQn EXTI15_10_IRQn
#define STOP_Pin GPIO_PIN_13
#define STOP_GPIO_Port GPIOB
#define STOP_EXTI_IRQn EXTI15_10_IRQn
#define FORWARD_Pin GPIO_PIN_14
#define FORWARD_GPIO_Port GPIOB
#define FORWARD_EXTI_IRQn EXTI15_10_IRQn
#define BACKWARD_Pin GPIO_PIN_15
#define BACKWARD_GPIO_Port GPIOB
#define BACKWARD_EXTI_IRQn EXTI15_10_IRQn
#define PWM1_Pin GPIO_PIN_6
#define PWM1_GPIO_Port GPIOC
#define INA1_Pin GPIO_PIN_7
#define INA1_GPIO_Port GPIOC
#define INB1_Pin GPIO_PIN_8
#define INB1_GPIO_Port GPIOC
#define CS1_Pin GPIO_PIN_9
#define CS1_GPIO_Port GPIOC
#define LORA_RST_Pin GPIO_PIN_12
#define LORA_RST_GPIO_Port GPIOA
#define OUT3_Pin GPIO_PIN_15
#define OUT3_GPIO_Port GPIOA
#define OUT4_Pin GPIO_PIN_12
#define OUT4_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
