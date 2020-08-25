#include "stm32f10x.h"                  // Device header
#include "stdint.h"

/* Private include ---------------------------------------------------------*/
#include "driver_motor.h"
#include "uart.h"
/* Private macro -------------------------------------------------------------*/
#define COL1 (1<<0)
#define COL2 (1<<1)
#define COL3 (1<<2)
#define COL4 (1<<3)

#define ROW1 (1<<4)
#define ROW2 (1<<5)
#define ROW3 (1<<6)
#define ROW4 (1<<7)

#define SAMPLE_TIME_MS	10
#define SAMPLE_COUNT		(SAMPLE_TIME_MS)

#define THRESHOLD_TIME_MS 2
#define THRESHOLD				(THRESHOLD_TIME_MS)

#define KEY_PRESS_MASK	0b11000111
#define KEY_REL_MASK		0b11100011

/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
RCC_ClocksTypeDef RCC_ClockFreq;
ErrorStatus HSEStartUpStatus;
static __IO uint32_t TimingDelay;
// Keyboard variables
uint32_t keys_debounce = 0; // Debounce counter for keyboard
uint32_t keys_last     = 0; // Previous keys state
uint32_t keys_ready    = 1; // Is keyboard readings updated?
uint16_t keys_pressed  = 0; // Pressed keys
uint32_t keys_lkp      = 0; // For readings update detection

int col = 0;
int row = -1;

char char_array[4][4] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

int int_array[4][4] = {
	{1,2,3,0xA},
	{4,5,6,0xB},
	{7,8,9,0xC},
	{0xF,0,0xE,0xD},
};

uint8_t key_samples[4][4] = {{0},{0},{0},{0}};
uint8_t key_pressed[4][4] = {{0},{0},{0},{0}};
uint8_t key_released[4][4] = {{0},{0},{0},{0}};

RCC_ClocksTypeDef RCC_ClockFreq;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

void update_sample(int row);
void update_key_press();
char get_char_key();
int get_key_pressed();

/* Private configuration -------------------------------------------------------------*/
void GPIO_Config();
void TIM3_Init();		//this timer take response to scan all input
/**/
void SysTick_Handler(){
	TimingDelay_Decrement();
}

void TIM3_IRQHandler(){

	int current_low = -1;
	
	if(GPIOB->IDR&GPIO_IDR_IDR4){
		current_low = 0;
	}
	
	if(GPIOB->IDR&GPIO_IDR_IDR5){
		current_low = 1;
	}
	
	if(GPIOB->IDR&GPIO_IDR_IDR6){
		current_low = 2;
	}
	
	if(GPIOB->IDR&GPIO_IDR_IDR7){
		current_low = 3;
	}
	
	update_sample(current_low);
	update_key_press();
	col++;
	if(col >3){
		col = 0;
	}
	
	GPIOA->ODR = 1<<col;
	TIM3->SR &=~TIM_SR_UIF;
}

/**/

int main(){
	
	SystemInit();
	  /* This function fills the RCC_ClockFreq structure with the current
     frequencies of different on chip clocks (for debug purpose) */
  RCC_GetClocksFreq(&RCC_ClockFreq);
  /* Enable Clock Security System(CSS): this will generate an NMI exception
     when HSE clock fails */
  RCC_ClockSecuritySystemCmd(ENABLE);
	//enable MCO pin
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRH |= GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1;
	GPIOA->CRH &=~ GPIO_CRH_CNF8_0;
	RCC_MCOConfig(RCC_MCO_HSE);
	
	/*NVIC Configuration*/
	NVIC_Configuration();
	
	/*GPIO Configuration*/
	GPIO_Config();
	TIM3_Init();
	/*Uart Configuration*/
	UART_Init(115200);
	UART_SendStr("Hello World!");
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN) | (RCC_APB2ENR_IOPAEN);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//GPIOA->CRL |= (0b1011<<0);
	

	
	/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 1023;
	TIM_TimeBaseStructure.TIM_Prescaler = (RCC_ClockFreq.PCLK1_Frequency/Tim_PWM_Freq_Out) - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
	*/
	
	
	/*systick initialize to generate delay function*/
	if(SysTick_Config(RCC_ClockFreq.SYSCLK_Frequency/1000)){
		while(1);
	}
	//driver_init();
	//driver_run(20);
	int count;
	while(1){
			if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET){
				UART_SendStr("khoa");
			}
	}
}


/*

*/
void NVIC_Configuration(void){

  /* Enable and configure RCC global IRQ channel */
	NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/*
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/*Initialization for matrix keypad*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}
/*
	Timer 3 initilization
*/
void TIM3_Init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC_GetClocksFreq(&RCC_ClockFreq);
	TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->PSC = RCC_ClockFreq.PCLK1_Frequency/1000;
	TIM3->ARR = 999;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->EGR |= TIM_EGR_UG;
	TIM3->CR1 |= TIM_CR1_CEN;	
	NVIC_EnableIRQ(TIM3_IRQn);
}

/*
keypad function
*/
void update_sample(int row){
	//update the current column with new values
	for(int i = 0; i < 4; i++){
		uint8_t curr_history = key_samples[i][col];
		
		curr_history = curr_history << 1;
		
		if(row == i){
			curr_history |= 1;
		}
		
		key_samples[i][col] = curr_history;
	}
}

void update_key_press(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j <4; j++){
			if((key_samples[i][j]&KEY_PRESS_MASK) == 0b00000111){
				key_pressed[i][j] = 1;
				key_samples[i][j] = 0xFF;
			}
			
			if((key_samples[i][j] & KEY_REL_MASK) == 0b11100000){
				key_released[i][j] = 1;
				key_samples[i][j] =0x00;
			}
		}
	}
}

char get_char_key(){
	for(int i = 0; i < 4;i++){
		for(int j = 0; j < 4; j++){
			if(key_released[i][j] == 1 && key_pressed[i][j] == 1){
					key_released[i][j] = 0;
					key_pressed[i][j] = 0;
					return char_array[i][j];
			}
		}
	}
	
	return '\0';
}

int get_key_pressed(){
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(key_released[i][j] == 1 && key_pressed[i][j] == 1) {
				key_released[i][j] = 0;
				key_pressed[i][j] = 0;
				return int_array[i][j];
			}
		}
	}

	return -1;
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