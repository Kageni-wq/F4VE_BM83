/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "bm832.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static err_t BM83_event_handler ( BM83_t *dev );
static BM83_t BM83;
static log_t logger;


void application_init ( void ) 
{
    
    
    /** 
     * Logger initialization.
     * Default baud rate: 115200
     * Default log level: LOG_LEVEL_DEBUG
     * @note If USB_UART_RX and USB_UART_TX 
     * are defined as HAL_PIN_NC, you will 
     * need to define them manually for log to work. 
     * See @b LOG_MAP_USB_UART macro definition for detailed explanation.
     */


    // Click initialization.
    BM83_Initialise( &BM83 );

     if ( BM83_OK != BM83_set_device_name ( &BM83, DEVICE_NAME ) ){
      
     }
    
}

void application_task ( void ) 
{
    if ( BM83_OK == BM83_read_event ( &BM83 ) ) 
    {
        BM83_event_handler ( &BM83 );
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  application_init( );
    
    for ( ; ; ) 
    {
        application_task( );
    }

    return 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static err_t BM83_event_handler ( BM83_t *dev )
{
    err_t error_flag = BM83_OK;
    static BM83_eq_mode_t eq_mode = BM83_EQ_MODE_OFF;
    switch ( dev->event_packet.opcode )
    {
        case BM83_EVT_REPORT_SPP_DATA:
        {
            if ( 0 == dev->event_packet.param_buf[ 1 ] ) // single packet
            {                
                uint16_t payload_len = ( ( uint16_t ) dev->event_packet.param_buf[ 4 ] << 8 ) | 
                                                      dev->event_packet.param_buf[ 5 ];
                if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_PLAY ) )
                {
                    error_flag |= BM83_music_control ( &BM83, BM83_PLAY );
                    log_printf( &logger, " > play music\r\n\n" );
                } 
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_PAUSE ) )
                {
                    error_flag |= BM83_music_control ( &BM83, BM83_PAUSE );
                    log_printf( &logger, " > pause music\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_STOP ) )
                {
                    error_flag |= BM83_music_control ( &BM83, BM83_STOP );
                    log_printf( &logger, " > stop music\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_NEXT ) )
                {
                    error_flag |= BM83_music_control ( &BM83, BM83_NEXT_SONG );
                    log_printf( &logger, " > next song\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_PREVIOUS ) )
                {
                    error_flag |= BM83_music_control ( &BM83, BM83_PREVIOUS_SONG );
                    log_printf( &logger, " > previous song\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_VOLUME_UP ) )
                {
                    error_flag |= BM83_volume_up ( &BM83, 0, BM83_VOLUME_MASK_A2DP );
                    log_printf( &logger, " > volume up\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_VOLUME_DOWN ) )
                {
                    error_flag |= BM83_volume_down ( &BM83, 0, BM83_VOLUME_MASK_A2DP );
                    log_printf( &logger, " > volume down\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_UNMUTE ) )
                {
                    BM83_soft_unmute_output ( &BM83 );
                    log_printf( &logger, " > unmute output\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_MUTE ) )
                {
                    BM83_soft_mute_output ( &BM83 );
                    log_printf( &logger, " > mute output\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_CALL ) )
                {
                    uint8_t phone_number[ 19 ] = { 0 };
                    memcpy ( phone_number, strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_CALL ) + 5, payload_len - 7 );
                    error_flag |= BM83_make_call ( &BM83, 0x00, phone_number );
                    log_printf( &logger, " > calling number: %s\r\n\n", phone_number );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_VOICE ) )
                {
                    error_flag |= BM83_send_mmi_action ( dev, 0, BM83_MMI_VOICE_DIAL );
                    log_printf( &logger, " > start voice recognition\r\n\n" );
                }
                else if ( strstr ( &dev->event_packet.param_buf[ 6 ], COMMAND_MODE_UP ) )
                {
                    if ( BM83_EQ_MODE_USER1 == eq_mode )
                    {
                        eq_mode = BM83_EQ_MODE_OFF;
                    }
                    else
                    {
                        eq_mode++;
                    }
                    BM83_set_eq_mode ( &BM83, eq_mode );
                    log_printf( &logger, " > equalizer mode: " );
                    switch ( eq_mode )
                    {
                        case BM83_EQ_MODE_OFF:
                        {
                            log_printf( &logger, "off\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_SOFT:
                        {
                            log_printf( &logger, "soft\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_BASS:
                        {
                            log_printf( &logger, "bass\r\n\n" );
                            break;
                        } 
                        case BM83_EQ_MODE_TREBLE:
                        {
                            log_printf( &logger, "treble\r\n\n" );
                            break;
                        }  
                        case BM83_EQ_MODE_CLASSIC:
                        {
                            log_printf( &logger, "classic\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_ROCK:
                        {
                            log_printf( &logger, "rock\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_JAZZ:
                        {
                            log_printf( &logger, "jazz\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_POP:
                        {
                            log_printf( &logger, "pop\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_DANCE:
                        {
                            log_printf( &logger, "dance\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_RNB:
                        {
                            log_printf( &logger, "rnb\r\n\n" );
                            break;
                        }
                        case BM83_EQ_MODE_USER1:
                        {
                            log_printf( &logger, "user1\r\n\n" );
                            break;
                        }
                        default:
                        {
                            log_printf( &logger, "unknown\r\n\n" );
                            break;
                        }
                    }
                }
                else
                {
                    log_printf( &logger, " Unknown command: ", &dev->event_packet.param_buf[ 6 ] );
                    for ( uint16_t cnt = 0; cnt < payload_len; cnt++ )
                    {
                        log_printf( &logger, "%c", dev->event_packet.param_buf[ cnt + 6 ] );
                    }
                    log_printf( &logger, "\r\n\n" );
                }
            }
            break;
        }
        case BM83_EVT_BTM_STATE:
        {
            switch ( dev->event_packet.param_buf[ 0 ] )
            {
                case BM83_BTM_STATE_PAIRING_SUCCESSFULL:
                {
                    log_printf( &logger, " < Pairing successfull - linked device ID: %u\r\n\n", 
                                ( uint16_t ) dev->event_packet.param_buf[ 1 ] );
                    break;
                }
                case BM83_BTM_STATE_SCO_LINK_CONNECTED:
                {
                    log_printf( &logger, " < SCO link connected - linked device ID: %u\r\n\n", 
                                ( uint16_t ) dev->event_packet.param_buf[ 1 ] );
                    break;
                }
                case BM83_BTM_STATE_SCO_LINK_DISCONNECTED:
                {
                    log_printf( &logger, " < SCO link disconnected - linked device ID: %u\r\n\n", 
                                ( uint16_t ) dev->event_packet.param_buf[ 1 ] );
                    break;
                }
                case BM83_BTM_STATE_ACL_CONNECTED:
                {
                    log_printf( &logger, " < ACL connected - linked data base: %u\r\n\n", 
                                ( uint16_t ) dev->event_packet.param_buf[ 1 ] );
                    break;
                }
                case BM83_BTM_STATE_ACL_DISCONNECTED:
                {
                    log_printf( &logger, " < ACL disconnected - " );
                    if ( 0 == dev->event_packet.param_buf[ 1 ] )
                    {
                        log_printf( &logger, "disconnection\r\n\n" );
                    }
                    else if ( 1 == dev->event_packet.param_buf[ 1 ] )
                    {
                        log_printf( &logger, "link loss\r\n\n" );
                    }
                    break;
                }
                case BM83_BTM_STATE_STANDBY_STATE:
                {
                    log_printf( &logger, " < Standby state\r\n\n" );
                    log_printf( &logger, " > ReEnter pairing mode\r\n\n" );
                    error_flag |= BM83_enter_pairing_mode ( &BM83 );
                    break;
                }
                case BM83_BTM_STATE_UNKNOWN_AUDIO_SOURCE:
                {
                    log_printf( &logger, " < Unknown audio source\r\n\n" );
                    break;
                }
                case BM83_BTM_STATE_AUX_IN_AUDIO_SOURCE:
                {
                    log_printf( &logger, " < AUX-IN audio source\r\n\n" );
                    break;
                }
                case BM83_BTM_STATE_A2DP_AUDIO_SOURCE:
                {
                    log_printf( &logger, " < A2DP audio source\r\n\n" );
                    break;
                }
                default:
                {
                    log_printf( &logger, " < EVENT BTM STATE: " );
                    for ( uint16_t cnt = 0; cnt < dev->event_packet.param_len; cnt++ )
                    {
                        log_printf( &logger, "0x%.2X ", ( uint16_t ) dev->event_packet.param_buf[ cnt ] );
                    }
                    log_printf( &logger, "\r\n\n" );
                    break;
                }
            }
            break;
        }
        default: 
        {
            log_printf( &logger, " < EVENT 0x%.2X: ", ( uint16_t ) dev->event_packet.opcode );
            for ( uint16_t cnt = 0; cnt < dev->event_packet.param_len; cnt++ )
            {
                log_printf( &logger, "0x%.2X ", ( uint16_t ) dev->event_packet.param_buf[ cnt ] );
            }
            log_printf( &logger, "\r\n\n" );
            break;
        }
    }
    return error_flag;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
