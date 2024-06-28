/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
	IMG_CAMERA = 0,
	IMG_EAT,
	IMG_WC
} ImageIdx;
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
#define OV7670_RCK_Pin GPIO_PIN_3
#define OV7670_RCK_GPIO_Port GPIOE
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OV7670_WREN_Pin GPIO_PIN_7
#define OV7670_WREN_GPIO_Port GPIOF
#define OV7670_RRST_Pin GPIO_PIN_8
#define OV7670_RRST_GPIO_Port GPIOF
#define OV7670_WRST_Pin GPIO_PIN_9
#define OV7670_WRST_GPIO_Port GPIOF
#define USB_FS_PWR_EN_Pin GPIO_PIN_10
#define USB_FS_PWR_EN_GPIO_Port GPIOF
#define PH0_MCU_Pin GPIO_PIN_0
#define PH0_MCU_GPIO_Port GPIOH
#define PH1_MCU_Pin GPIO_PIN_1
#define PH1_MCU_GPIO_Port GPIOH
#define LD3320_IRQ_Pin GPIO_PIN_0
#define LD3320_IRQ_GPIO_Port GPIOA
#define LD3320_IRQ_EXTI_IRQn EXTI0_IRQn
#define BUZZER_Pin GPIO_PIN_2
#define BUZZER_GPIO_Port GPIOA
#define OV7670_VSYNC_Pin GPIO_PIN_3
#define OV7670_VSYNC_GPIO_Port GPIOA
#define OV7670_VSYNC_EXTI_IRQn EXTI3_IRQn
#define LD3320_SCK_Pin GPIO_PIN_5
#define LD3320_SCK_GPIO_Port GPIOA
#define LD3320_MISO_Pin GPIO_PIN_6
#define LD3320_MISO_GPIO_Port GPIOA
#define LD3320_MOSI_Pin GPIO_PIN_7
#define LD3320_MOSI_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LCD_MOSI_Pin GPIO_PIN_2
#define LCD_MOSI_GPIO_Port GPIOB
#define SCCB_SCL_Pin GPIO_PIN_7
#define SCCB_SCL_GPIO_Port GPIOE
#define SCCB_SDA_Pin GPIO_PIN_8
#define SCCB_SDA_GPIO_Port GPIOE
#define MOTOR_1_Pin GPIO_PIN_11
#define MOTOR_1_GPIO_Port GPIOE
#define INFRARED_Pin GPIO_PIN_13
#define INFRARED_GPIO_Port GPIOE
#define MOTOR_2_Pin GPIO_PIN_14
#define MOTOR_2_GPIO_Port GPIOE
#define LD3320_CS_Pin GPIO_PIN_10
#define LD3320_CS_GPIO_Port GPIOB
#define LD3320_WR_Pin GPIO_PIN_11
#define LD3320_WR_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define LCD_LED_Pin GPIO_PIN_15
#define LCD_LED_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define OV7670_OE_Pin GPIO_PIN_10
#define OV7670_OE_GPIO_Port GPIOD
#define USB_FS_OVCR_Pin GPIO_PIN_7
#define USB_FS_OVCR_GPIO_Port GPIOG
#define OV7670_RST_Pin GPIO_PIN_6
#define OV7670_RST_GPIO_Port GPIOC
#define USB_FS_VBUS_Pin GPIO_PIN_9
#define USB_FS_VBUS_GPIO_Port GPIOA
#define USB_FS_ID_Pin GPIO_PIN_10
#define USB_FS_ID_GPIO_Port GPIOA
#define USB_FS_N_Pin GPIO_PIN_11
#define USB_FS_N_GPIO_Port GPIOA
#define USB_FS_P_Pin GPIO_PIN_12
#define USB_FS_P_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_10
#define LCD_SCK_GPIO_Port GPIOC
#define OV7670_D0_Pin GPIO_PIN_0
#define OV7670_D0_GPIO_Port GPIOD
#define OV7670_D1_Pin GPIO_PIN_1
#define OV7670_D1_GPIO_Port GPIOD
#define OV7670_D2_Pin GPIO_PIN_2
#define OV7670_D2_GPIO_Port GPIOD
#define OV7670_D3_Pin GPIO_PIN_3
#define OV7670_D3_GPIO_Port GPIOD
#define OV7670_D4_Pin GPIO_PIN_4
#define OV7670_D4_GPIO_Port GPIOD
#define OV7670_D5_Pin GPIO_PIN_5
#define OV7670_D5_GPIO_Port GPIOD
#define OV7670_D6_Pin GPIO_PIN_6
#define OV7670_D6_GPIO_Port GPIOD
#define OV7670_D7_Pin GPIO_PIN_7
#define OV7670_D7_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_5
#define LCD_DC_GPIO_Port GPIOB
#define LD3320_RST_Pin GPIO_PIN_0
#define LD3320_RST_GPIO_Port GPIOE
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
