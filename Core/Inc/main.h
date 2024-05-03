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
#define K1_Pin GPIO_PIN_3
#define K1_GPIO_Port GPIOE
#define K0_Pin GPIO_PIN_4
#define K0_GPIO_Port GPIOE
#define D1_Pin GPIO_PIN_6
#define D1_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_7
#define D3_GPIO_Port GPIOA
#define MFB_Pin GPIO_PIN_8
#define MFB_GPIO_Port GPIOB
#define rst_pin GPIO_PIN_9
#define rst_port GPIOB
#define sm_port  GPIOB
#define sm_pin  GPIO_PIN_7
#define rst_port  GPIOB
#define rst_pin GPIO_PIN_9
#define mod_port GPIOB
#define mod_pin GPIO_PIN_6
#define rx_port  GPIOA
#define rx_pin  GPIO_PIN_10
#define tx_port  GPIOA
#define tx_pin  GPIO_PIN_9

// Bluetooth device name
#define DEVICE_NAME                 "BT Audio 3 click"

// SPP Commands list
#define COMMAND_PLAY                "play"          // play music
#define COMMAND_PAUSE               "pause"         // pause music
#define COMMAND_STOP                "stop"          // stop music
#define COMMAND_NEXT                "next"          // next song
#define COMMAND_PREVIOUS            "prev"          // previous song
#define COMMAND_VOLUME_UP           "up"            // volume up
#define COMMAND_VOLUME_DOWN         "down"          // volume down
#define COMMAND_UNMUTE              "unmute"        // unmute output
#define COMMAND_MUTE                "mute"          // mute output
#define COMMAND_MODE_UP             "mode"          // switch equalizer mode
#define COMMAND_CALL                "call"          // call command followed by the nuber
#define COMMAND_VOICE               "voice"         // start voice recognition app

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
