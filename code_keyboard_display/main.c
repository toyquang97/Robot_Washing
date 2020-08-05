#include "stm32f10x.h"                  // Device header
uint32_t Tick;
unsigned int row = 0;
void ST7920_Init();
void SysTick_Handler(void){
	Tick++;
}

void delay_ms(uint32_t time){
	uint32_t temp = Tick;
	while((Tick-temp)<time);
}

int main(){
	SysTick_Config(SystemCoreClock/1000);
	/*Enable clock for gpio port A*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	/*
	set PA0:PA3 as pull-up input, PA4:PA7 as 2MHz push-pull output 
	set high PA4:PA7
	*/
	GPIOA->CRL = 0x22228888;
	GPIOA->ODR = 0xFF;	
	/*
	Enable interrupt for input
	*/
	AFIO->EXTICR[0] = ~0xFFFF; 
	EXTI->IMR = 0x0F;
	EXTI->FTSR = 0x0F;
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	unsigned int column = 1 ;
	unsigned int key = 0;
	while(1){
		if(row != 0){
			delay_ms(20);	
			while(1){
				
			}
			key = row*4 + column; 
			
			if(key==16){
			
			}
			
			/*refresh lai cac bien*/
			GPIOA->ODR |= (0xF<<4);
			column = 1;
			row = 0;
		}
	}

}

void EXTI0_IRQHandler(void){
	row = 1;
	EXTI->PR |= EXTI_PR_PR0;
}

void EXTI1_IRQHandler(void){
	row = 2;
	EXTI->PR |= EXTI_PR_PR1;
}

void EXTI2_IRQHandler(void){
	row = 3;
	EXTI->PR |= EXTI_PR_PR2;
}

void EXTI3_IRQHandler(void){
	row = 4;
	EXTI->PR |= EXTI_PR_PR3;
}

/*initialize for i2c buffer device*/
void I2C_Init(){
	
}