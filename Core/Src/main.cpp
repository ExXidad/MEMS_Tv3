/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MirrorcleMems.hpp"
#include <AD5667.hpp>
#include <GPIO.hpp>
#include <Timer.hpp>
#include <cstdio>
#include <etl/math_constants.h>
#include <retarget.h>
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

Timer updateLoopTimer(&htim3);

MirrorcleMEMS mirrorcleMems({MEMS_ENBL_GPIO_Port, MEMS_ENBL_Pin},
                            {&htim2, TIM_CHANNEL_4},
                            {&hi2c1,
                             {DAC_CLR_GPIO_Port, DAC_CLR_Pin},
                             {DAC_LDAC_GPIO_Port, DAC_LDAC_Pin},
                             0x0F});

GPIO LED_GPIO(LED_GPIO_Port, LED_Pin);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  RetargetInit(&huart1);
  mirrorcleMems.start();
//  mirrorcleMems.foo();
//  mirrorcleMems.writeAxisSignal(MirrorcleMEMS::AXIS::Y, 0.5);
//  mirrorcleMems.writeAxisSignal(MirrorcleMEMS::AXIS::X, 0.5);
  mirrorcleMems.ad5667_.sendSignal(AD5667::COMMAND::WRITE_UPDATE_ALL, AD5667::CHANNEL::A, 0.5);
  mirrorcleMems.ad5667_.sendSignal(AD5667::COMMAND::WRITE_UPDATE_ALL, AD5667::CHANNEL::B, 0.5);

  updateLoopTimer.startTimer();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  scalar x = 0;
  constexpr scalar dx = 0.05;
  while (1) {
//    LED_GPIO.toggle();
//    mirrorcleMems.writeAxisSignal(MirrorcleMEMS::AXIS::X, x);
//    mirrorcleMems.writeAxisSignal(MirrorcleMEMS::AXIS::Y, 0);
//    mirrorcleMems.ad5667_.LDAC_GPIO().toggle();
    mirrorcleMems.ad5667_.sendSignal(AD5667::COMMAND::WRITE_UPDATE_ALL, AD5667::CHANNEL::B, x);
    x += dx;
    x = (x > 1) ? (x - 1) : x;
//        printf("Toggle!\n");
    HAL_Delay(200);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim == updateLoopTimer) {
    //    static indexType i;
    //    static scalar dt = 1 / updateLoopTimer.updateFrequency();
    //    constexpr static scalar amplitude = 0.2;
    //    constexpr static scalar period = 1;
    //    constexpr static scalar omega =
    //        2 * etl::private_math_constants::math_helper<scalar>::pi / period;
    //    scalar phase = omega * i * dt;
    //    phase = (phase>360)?(phase-360):phase;
    //    printf("%f\n",phase);
    //    mirrorcleMems.writeAxisSignal(MirrorcleMEMS::AXIS::X,
    //                                           amplitude * cos(phase));
//        mirrorcleMems.writeAxisSignal(MirrorcleMEMS::AXIS::X,
    //                                  amplitude * sin(phase));
    //    i++;
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
