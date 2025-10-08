/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
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

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"

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
osThreadId_t ledTaskHandle;
osThreadId_t uartTaskHandle;

/* Private function prototypes -----------------------------------------------*/
void StartLedTask(void *argument);
void StartUartTask(void *argument);

/* USER CODE BEGIN 0 */
// LED Task
void StartLedTask(void *argument)
{
  for(;;)
  {
    BSP_LED_Toggle(LED_GREEN);
    osDelay(500);
  }
}

// UART Task
void StartUartTask(void *argument)
{
  for(;;)
  {
    printf("UART Task Running...\r\n");
    osDelay(1000);
  }
}
/* USER CODE END 0 */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_THREADS */
  // Create LED task
  const osThreadAttr_t ledTask_attributes = {
    .name = "ledTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128 * 4
  };
  ledTaskHandle = osThreadNew(StartLedTask, NULL, &ledTask_attributes);

  // Create UART task
  const osThreadAttr_t uartTask_attributes = {
    .name = "uartTask",
    .priority = (osPriority_t) osPriorityBelowNormal,
    .stack_size = 128 * 4
  };
  uartTaskHandle = osThreadNew(StartUartTask, NULL, &uartTask_attributes);
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END defaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

