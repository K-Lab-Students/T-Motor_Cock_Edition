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
#include "can.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ak_drv.h"
#include "stdbool.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TABLE_SAMPLING 360
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
static inline double ConvertDegreesToRadians (double degree){
 return degree * 3.14f / 180.0;}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t TxMailbox = 0;
int runParam = 0;
int sineIndex = 180;
int maxLeftTest, maxRightTest = 0;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
MotorValues_t RMD_Actuator;

char TxData[8] = {0x80,0x85,0xB5,0x0D,0x03,0xE8,0x03,0xE8};
uint8_t RxData[9] = {0};

char trans_str[100] = {0};

bool buttonIsPressed = 0;
bool isStoped = 0;

int sinTable[TABLE_SAMPLING];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void sinSpin(MotorValues_t* motor){

	comm_can_set_pos_spd(motor->id, sinTable[sineIndex++], 5000, 20000);
	HAL_Delay(10);

		if(isStoped != 1){

			for(uint32_t i = 0; i < 180;i++){
			  comm_can_set_pos_spd(motor->id, i, 50000, 20000);
			  HAL_Delay(50);
			}

		if(sineIndex>= 360) sineIndex = 0;

		for(int deg = 0; deg <= 360;deg++){
		  HAL_Delay(10);
		  comm_can_set_pos_spd(motor->id, deg, 4000, 20000);
			}
		}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1)
{
  if(HAL_CAN_GetRxMessage(hcan1, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
	  if(RxHeader.StdId == RMD_Actuator.id){
		motor_receive(&RMD_Actuator, RxData, RxHeader.StdId);
//		sprintf(trans_str, "ID: %d Current: %f Speed: %f Position: %f Temperature: %d \r\n ", RMD_Actuator.id, RMD_Actuator.current, RMD_Actuator.speed, RMD_Actuator.position, RMD_Actuator.temperature);
		sprintf(trans_str, "%f \r\n ", RMD_Actuator.position);
		HAL_UART_Transmit_IT(&huart2, trans_str, strlen(trans_str));
	  	  }
    }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
//    uint32_t er = HAL_CAN_GetError(hcan);
//    sprintf(trans_str,"ER CAN %lu %08lX\n", er, er);
//    HAL_UART_Transmit(&huart2, (uint8_t*)trans_str, strlen(trans_str), 100);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
        if(htim->Instance == TIM1)
        {
        		buttonIsPressed = 0;

        		HAL_TIM_Base_Stop_IT(&htim1);
        		TIM1->CNT = 0;
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  __HAL_TIM_CLEAR_FLAG(&htim1, TIM_SR_UIF);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  HAL_CAN_Stop(&hcan1);

  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE);

  RxData[8] = '\n';

  HAL_CAN_Start(&hcan1);

  motorInit(RMD_Actuator, 0);
  comm_can_set_origin(RMD_Actuator.id, 1);
  HAL_Delay(100);
  comm_can_set_pos(RMD_Actuator.id, 5);
				//  HAL_Delay(2000);
				//  comm_can_set_pos(MOTOR_CAN_ID, -270);
				//  HAL_Delay(2000);
				//  comm_can_set_pos(MOTOR_CAN_ID, -360);

//  double a = 0.0;
//  for (uint32_t i = 0; i < TABLE_SAMPLING; i++){
//	  sinTable[i] = round((sin(ConvertDegreesToRadians(a)) + 1.0) * 180.0);
//	  a += (360.0 / (TABLE_SAMPLING * 1.0));
//  }

  HAL_Delay(100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */




//	  HAL_UART_Transmit_IT(&huart2, (uint8_t*) "Cock and Ball \n", strlen("Cock and Ball \n"));
	 /* if((RMD_Actuator.position > 0) &&(RMD_Actuator.position < 180)){
		  comm_can_set_pos(RMD_Actuator.id, 200);
	  }

	  else{
		  comm_can_set_pos(RMD_Actuator.id, 0);
	  }*/
//	 sinSpin();

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if((GPIO_Pin == GPIO_PIN_13) && !buttonIsPressed && !isStoped) {
	HAL_TIM_Base_Start_IT(&htim1);
	buttonIsPressed = 1;

	runParam = 2;
	comm_can_set_pos_spd(RMD_Actuator.id, 0, 50000, 20000);
	isStoped = true;
	}

	if((GPIO_Pin == GPIO_PIN_13) && !buttonIsPressed && isStoped) {
		HAL_TIM_Base_Start_IT(&htim1);
		buttonIsPressed = 1;

		runParam = 0;

		isStoped = false;
	}
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
