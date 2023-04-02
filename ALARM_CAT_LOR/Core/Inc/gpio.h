/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define LED1_1() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET)
#define LED1_0() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET)

#define LED2_1() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
#define LED2_0() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define LED2_T() HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13)
	
#define PWK_GPIO_SET_HIGH  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET)  //PEN��������Ϊ�ߵ�ƽ
#define PWK_GPIO_SET_LOW   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET) //PEN��������Ϊ�͵�ƽ


#define PEN_GPIO_SET_HIGH  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)  //PEN��������Ϊ�ߵ�ƽ
#define PEN_GPIO_SET_LOW   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET) //PEN��������Ϊ�͵�ƽ

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
