/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define EINK_DC_Pin GPIO_PIN_0
#define EINK_DC_GPIO_Port GPIOC
#define EINK_BUSY_Pin GPIO_PIN_1
#define EINK_BUSY_GPIO_Port GPIOC
#define EINK_RST_Pin GPIO_PIN_2
#define EINK_RST_GPIO_Port GPIOC
#define EINK_CS_Pin GPIO_PIN_4
#define EINK_CS_GPIO_Port GPIOA
#define M0_PWM_BL_Pin GPIO_PIN_0
#define M0_PWM_BL_GPIO_Port GPIOB
#define M0_PWM_CL_Pin GPIO_PIN_1
#define M0_PWM_CL_GPIO_Port GPIOB
#define M0_PWM_AL_Pin GPIO_PIN_13
#define M0_PWM_AL_GPIO_Port GPIOB
#define KEY_A_Pin GPIO_PIN_8
#define KEY_A_GPIO_Port GPIOC
#define KEY_B_Pin GPIO_PIN_9
#define KEY_B_GPIO_Port GPIOC
#define M0_PWM_AH_Pin GPIO_PIN_8
#define M0_PWM_AH_GPIO_Port GPIOA
#define M0_PWM_BH_Pin GPIO_PIN_9
#define M0_PWM_BH_GPIO_Port GPIOA
#define M0_PWM_CH_Pin GPIO_PIN_10
#define M0_PWM_CH_GPIO_Port GPIOA
#define M0_CS_Pin GPIO_PIN_2
#define M0_CS_GPIO_Port GPIOD
#define VMOTOR_EN_Pin GPIO_PIN_8
#define VMOTOR_EN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
