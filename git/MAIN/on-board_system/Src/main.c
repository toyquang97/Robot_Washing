/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "driver_motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	int target;
	int previous;
	int real;
} speed;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int i ;			//bien dem
char RX_buffer[RX_MAX_BUFFER_SIZE];
int len;										//
char Rx_Ind;								//bien dem chuoi nhan dc tu uart
uint8_t Rx_data[2];					//khai bao bien de nhan tu thanh ghi DR cua uart4
int uart_flag = 0;					//bien co ngat cua uart khi nhan du 8 byte
volatile int Target_Speed = 0;						//bien gia tri van toc yeu cau robot chay
volatile int Real_Speed = 0;						//bien gia tri van toc duoc cap cho toc do robot
volatile int Pre_Speed = 0;
int temp_speed;													//bien trung gian de luu gia tri cua toc do robot
uint8_t sensor12;
uint8_t sensor35;
bool edge_mode = false;									//bien trang thai dang leo mep
int status = 0;							//bien trang thai cua robot:
														/*
														1: standing by khi nhan nut start
														2: forward
														3: backward
														0: default when start operation
														*/
 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//uart4 interrupt handle voi
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t i = 0;
	static uint8_t uart_flag = 0;
	if(huart->Instance == huart4.Instance){
	 
		if(Rx_Ind == 0){
			for(i = 0; i < RX_MAX_BUFFER_SIZE;i++){
				RX_buffer[i] = 0;
			}
		}
		else{
			__NOP();
		}
		if(Rx_data[0]!='X'){			//
			RX_buffer[Rx_Ind] = Rx_data[0];
			Rx_Ind++;
		}
		else{		//KHI KI TU LA 'X'
			if(Rx_Ind == RX_MAX_BUFFER_SIZE){
				uart_flag = 1;
			}
			else{
				UART_SendStr("lack of lenghth\n");
			}
			Rx_Ind = 0;
		}
		//only when status = 1 (stand by mode) 
		if(status == 1 && uart_flag == 1){
			if(RX_buffer[0]=='S'&RX_buffer[1]=='P'&RX_buffer[2]=='E'&RX_buffer[4]=='D'){
					Target_Speed = (RX_buffer[5]-48)*100 + (RX_buffer[6]-48)*10 + (RX_buffer[7]-48);
					Target_Speed = (Target_Speed > MAX_SPEED) ? MAX_SPEED : Target_Speed;
			}
			else if(RX_buffer[0]=='C'&RX_buffer[1]=='L'&RX_buffer[5]=='W'&RX_buffer[6]=='E'&RX_buffer[7]=='T'){	
					HAL_GPIO_WritePin(OUT0_GPIO_Port,OUT0_Pin,GPIO_PIN_SET);
			}
			else if(RX_buffer[0]=='C'&RX_buffer[1]=='L'&RX_buffer[5]=='D'&RX_buffer[6]=='R'&RX_buffer[7]=='Y'){	
					HAL_GPIO_WritePin(OUT0_GPIO_Port,OUT0_Pin,GPIO_PIN_RESET);
			}
			else{
				__NOP();
			}
			uart_flag = 0;
		}
		else{
			__NOP();
		}
		HAL_UART_Receive_IT(&huart4,Rx_data,1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	//inner timer take response to change speed of motor

	//tim2 interrupt handle void
	if(htim->Instance == htim2.Instance){ 
				if(Real_Speed < Target_Speed){
					Real_Speed++;
				}
				else if(Real_Speed == Target_Speed){
					Real_Speed = Target_Speed;
				}
				else {
					Real_Speed--;
				}
	}
	//timer 6 to read sensor
	if(htim->Instance == htim6.Instance){
		sensor12 = (HAL_GPIO_ReadPin(S1_GPIO_Port,S1_Pin) << 1) | HAL_GPIO_ReadPin(S2_GPIO_Port,S2_Pin);
		sensor35 = (HAL_GPIO_ReadPin(S5_GPIO_Port,S5_Pin) << 1) | HAL_GPIO_ReadPin(S3_GPIO_Port,S3_Pin);
	}
	//

}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

/*

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM5_Init();
  MX_UART4_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	UART_SendStr("\nHello World!\n");
	//initialize for driver motor
	driver_init();	
	HAL_UART_Receive_IT(&huart4,Rx_data,1);	// enable for receive interrupt
	HAL_TIM_Base_Start_IT(&htim6);			//timer interrupt to read sensor 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	Target_Speed = DEFAULT_SPEED_VALUE;

  while (1)
  {
		if(HAL_GPIO_ReadPin(START_GPIO_Port,START_Pin)){
			//when detect start button
			HAL_Delay(20);
			while(HAL_GPIO_ReadPin(START_GPIO_Port,START_Pin));
			status = 1;
			UART_SendStr("Start\n");
		}
		else{
			if(status == 1){
				//standing by mode
				Real_Speed = 0;
				/**/	
			}
			else if(status == 2){
				static uint8_t pre_sensor12 = 5;	
				/*Run Mode*/					
				/*warining if place robot incorrectly*/
				
					//save the previous value of sensor12 before change			
					switch(sensor12){
						
							case 0:						//sensor1 == 0 and sensor2 == 0
									/*run*/
									if(pre_sensor12 == 5){
										HAL_TIM_Base_Start_IT(&htim2);			//start increase real_speed
									}
									else if(pre_sensor12 == 2){
										
									}
									else if(pre_sensor12 == 1){
										
									}
									else{
										
									}
									break;
		
							case 2:						//sensor1 == 1 and sensor2 == 0
									if(pre_sensor12 == 0){		//when robot locate at the edge of panel
										Target_Speed /=2;				//			
									}
									else{
									
									}
									break;
								
							case 1:						//sensor1 == 0 and sensor2 == 1

									break;
							case 3:		
									
									break;								
							default:
									//////	
									break;

						}
							//store perious value of sensor to compare
						pre_sensor12 = (sensor12!=pre_sensor12) ? sensor12 : pre_sensor12;
				}
					/****/
				else if(status == 3){
					static uint8_t pre_sensor35 = 5;
					
					switch(sensor35){
						case 0:
							if(pre_sensor35 == 5){
								HAL_TIM_Base_Start_IT(&htim2);
							}
							else {
								__NOP();
							}
							break;
						case 2:
							break;
						case 1:
							break;
						case 3:
							break;
						default:
							break;
						
					/****/
						pre_sensor35 = (sensor35!=pre_sensor35) ? sensor35 : pre_sensor35;
					}
				}	
				else if(status == 4){
					Real_Speed = 0;
				}
				else{		//
						__NOP();
				}
					/****/
				driver_run(Real_Speed);
			}
		
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
