#include "stm32f10x.h"                  // Device header
#include "stdint.h"
#include "string.h"
/* Private include ---------------------------------------------------------*/
#include "uart.h"
#include "driver_motor.h"
/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
volatile int Real_Vel = 0;
volatile int temp = 0;
int Velocity = 0;
volatile uint8_t sensor = 0b00000000;
uint8_t RX_Buffer[Max_RXBuffer_Size];
volatile int uart_flag = 0;
RCC_ClocksTypeDef RCC_ClockFreq;
GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_ClockFreq;
EXTI_InitTypeDef EXTI_InitStructure;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
USART_InitTypeDef UART;

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void TIM2_Init(void);
void GPIO_Config();

/*interrupt handler*/
void SysTick_Handler(){
	TimingDelay_Decrement();
	//GPIOC->ODR ^= (1<<4);
}

void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line13)!=RESET){
		Real_Vel = 0;
		temp = 0;
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14)==SET){
		temp = 2;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if(EXTI_GetITStatus(EXTI_Line15)==SET){
		temp = 3;
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
	if(EXTI_GetITStatus(EXTI_Line10)==SET){
		sensor ^= (1<<0);
		GPIOB->ODR ^= (1<<4);
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	
}
//sensor S1
void EXTI0_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0)==SET){
		sensor ^= (1<<3);
		GPIOC->ODR ^= (1<<4);
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line1)==SET){
		sensor ^= (1<<2);
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
void EXTI2_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line2)==SET){
		sensor ^= (1<<1);
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void TIM2_IRQHandler(void){
	if(TIM2->SR&TIM_SR_UIF){
		if(Real_Vel < Velocity){
			Real_Vel++;
		}
		else if(Real_Vel == Velocity){
			Real_Vel = Velocity;
			TIM2->CR1 &= ~TIM_CR1_CEN;
		}
		else{
			Real_Vel--;
		}
		TIM2->SR&=~TIM_SR_UIF;
		
	}
}


int main(){

	SystemInit();
	//SystemCoreClockUpdate();
	  /* This function fills the RCC_ClockFreq structure with the current
     frequencies of different on chip clocks (for debug purpose) */
  RCC_GetClocksFreq(&RCC_ClockFreq);
  /* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails */
  //RCC_ClockSecuritySystemCmd(ENABLE);
	//enable MCO pin
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1;
	GPIOA->CRH &=~ GPIO_CRH_CNF8_0;
	RCC_MCOConfig(RCC_MCO_HSE);
	
	/*GPIO Configuration*/
	GPIO_Config();
	/*NVIC Configuration*/
	NVIC_Configuration();
	
	TIM2_Init();

	
	/*UART Configuration*/
	UART_Init(115200);
	
	UART_SendStr("Hello World!\n");
	
	/*systick initialize to generate delay function*/
	
	if(SysTick_Config(RCC_ClockFreq.PCLK1_Frequency/1000)){
		while(1);
	}
	/*configuration for driver*/
	driver_init();
	int Pre_Vel = 0;
	int i = 0;
	Velocity = 0;
	driver_run(10);
	while(1){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==1){
			Delay(20);
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12));
			UART_SendStr("Start\n");
			sensor = 0b00001111;
			temp = 1;
		}
		else{
			if(Real_Vel != Pre_Vel){
				UART_SendInt(Real_Vel);
				UART_SendChar('\n');
				Pre_Vel = Real_Vel;
			}

			if(temp > 1){
				if(temp == 2 ){
						
				}
				if(temp == 3){
					
				}
				
				
		}
			/**/
			if(uart_flag == 1){
				Delay(50);
				if((RX_Buffer[0]=='S')&(RX_Buffer[1]=='P')&(RX_Buffer[2]=='E')&(RX_Buffer[4]=='D')){
					Velocity = (RX_Buffer[5]-48)*100 + (RX_Buffer[6]-48)*10 + (RX_Buffer[7]-48);
					UART_SendStr("Result=");
					UART_SendInt(Velocity);
					UART_SendChar('\n');	
				}
				uart_flag = 0;
			}
		}
	
	}
}


void NVIC_Configuration(void){
	NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable and configure RCC global IRQ channel */
	NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
	Delay function
*/
void Delay(__IO uint32_t nTime){
	TimingDelay = nTime;
	while(TimingDelay!=0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/*
	gpio for system configuration
*/

void GPIO_Config(){
	
	/*Initialize for  button,Sensors*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
																		//START				//STOP				//FWD				//BWD					
	GPIO_InitStructure.GPIO_Mode = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 
			//S1				//S2					//S3						//S4
	| GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*connect ExtiLine13 to STOP,FWD,BWD button*/
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PB | AFIO_EXTICR4_EXTI13_PB | AFIO_EXTICR4_EXTI15_PB;
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB | AFIO_EXTICR1_EXTI1_PB | AFIO_EXTICR1_EXTI2_PB;
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PB;
	/*configure exti line*/
	EXTI->IMR |= EXTI_IMR_MR13 | EXTI_IMR_MR14 | EXTI_IMR_MR15 | EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2 |EXTI_IMR_MR10;
	EXTI->RTSR |= EXTI_RTSR_TR13 |EXTI_RTSR_TR0 | EXTI_RTSR_TR1 | EXTI_RTSR_TR2 |EXTI_RTSR_TR10;
	EXTI->FTSR |= EXTI_FTSR_TR14 | EXTI_FTSR_TR15;
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	
	/*Initialize for extension output*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	/**/

	
}
/*
	inner timer
*/

void TIM2_Init(void){
	
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = (RCC_ClockFreq.PCLK1_Frequency/1000)-1;
	TIM2->ARR = 999;
	TIM2->CR1 |= TIM_CR1_URS;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->EGR |= TIM_EGR_UG;
	
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM2_IRQn);	
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/