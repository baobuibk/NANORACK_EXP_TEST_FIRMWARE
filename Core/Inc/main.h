/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adg1414.h"
#include "mcp4902.h"
#include "adc_dma_device.h"
#include "ads8327.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint16_t adc_rec_buf[50010];
extern uint16_t *adc_ptr;
extern uint32_t adc_rec_ind;
extern uint32_t adc_rec_total;

extern MCP4902_Device_t DAC_device;
extern ADG1414_Device_t laser_int;
extern ADG1414_Device_t laser_ext;
extern ADC_DMA_Device_t mcu_adc;
extern ADG1414_Device_t photo_sw;
extern ADS8327_Device_t photo_adc;
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
#define ADC_TEMP_1_Pin LL_GPIO_PIN_0
#define ADC_TEMP_1_GPIO_Port GPIOC
#define ADC_TEMP_2_Pin LL_GPIO_PIN_1
#define ADC_TEMP_2_GPIO_Port GPIOC
#define ADC_TEMP_3_Pin LL_GPIO_PIN_2
#define ADC_TEMP_3_GPIO_Port GPIOC
#define ADC_TEMP_4_Pin LL_GPIO_PIN_3
#define ADC_TEMP_4_GPIO_Port GPIOC
#define ADC_INT_Pin LL_GPIO_PIN_2
#define ADC_INT_GPIO_Port GPIOA
#define ADC_EXT_Pin LL_GPIO_PIN_3
#define ADC_EXT_GPIO_Port GPIOA
#define ADC_TEMP_5_Pin LL_GPIO_PIN_4
#define ADC_TEMP_5_GPIO_Port GPIOC
#define ADC_TEMP_6_Pin LL_GPIO_PIN_5
#define ADC_TEMP_6_GPIO_Port GPIOC
#define ADC_TEMP_7_Pin LL_GPIO_PIN_0
#define ADC_TEMP_7_GPIO_Port GPIOB
#define ADC_TEMP_8_Pin LL_GPIO_PIN_1
#define ADC_TEMP_8_GPIO_Port GPIOB
#define STATUS_LED_Pin LL_GPIO_PIN_11
#define STATUS_LED_GPIO_Port GPIOE
#define MCP4902_CS_Pin LL_GPIO_PIN_12
#define MCP4902_CS_GPIO_Port GPIOE
#define ADG1414_EXT_CS_Pin LL_GPIO_PIN_13
#define ADG1414_EXT_CS_GPIO_Port GPIOE
#define ADG1414_INT_CS_Pin LL_GPIO_PIN_14
#define ADG1414_INT_CS_GPIO_Port GPIOE
#define MCP4902_LATCH_Pin LL_GPIO_PIN_15
#define MCP4902_LATCH_GPIO_Port GPIOE
#define PD_SCK_Pin LL_GPIO_PIN_13
#define PD_SCK_GPIO_Port GPIOB
#define PD_MISO_Pin LL_GPIO_PIN_14
#define PD_MISO_GPIO_Port GPIOB
#define PD_MOSI_Pin LL_GPIO_PIN_15
#define PD_MOSI_GPIO_Port GPIOB
#define PD_SW_CS_Pin LL_GPIO_PIN_9
#define PD_SW_CS_GPIO_Port GPIOD
#define PD_ADC_CS_Pin LL_GPIO_PIN_10
#define PD_ADC_CS_GPIO_Port GPIOD
#define PD_ADC_CV_Pin LL_GPIO_PIN_11
#define PD_ADC_CV_GPIO_Port GPIOD
#define PD_ADC_EOC_Pin LL_GPIO_PIN_8
#define PD_ADC_EOC_GPIO_Port GPIOC
#define FRAM_SCK_Pin LL_GPIO_PIN_10
#define FRAM_SCK_GPIO_Port GPIOC
#define FRAM_MISO_Pin LL_GPIO_PIN_11
#define FRAM_MISO_GPIO_Port GPIOC
#define FRAM_MOSI_Pin LL_GPIO_PIN_12
#define FRAM_MOSI_GPIO_Port GPIOC
#define FRAM_CS_Pin LL_GPIO_PIN_2
#define FRAM_CS_GPIO_Port GPIOD
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
