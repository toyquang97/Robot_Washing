#include "stm32f10x.h"                  // Device header
#include "vnh3sp30.h"

int temp_control = 0;

void OSC_Config();
void GPIO_Config();

int main(){
	OSC_Config();													//config for system clock	
	GPIO_Config();												//config for IO use in system
	VNH3SP30_Config_Pin();								//config pin for VNH3sp30 driver
	VNH3SP30_Config_PWM();								//operation for VNH3sp30 driver
	
	while(1){
	
	}
}

void OSC_Config(){
	/*FREQUENCY OF OSCILLATOR IS 8MHz*/
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR&RCC_CR_HSERDY));
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;
	RCC->CFGR |= RCC_CFGR_PLLMULL4;
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR&RCC_CR_PLLRDY));
	RCC->CFGR |= RCC_CFGR_SW_1;
	while(!(RCC->CFGR&RCC_CFGR_SWS_1));
	RCC->CFGR |= RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_3;
}

void GPIO_Config(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;				//ENABLE clock for portB
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;				//enable clock for portC
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;				//ENABLE clock for portA
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;				//ENABLE clock for AFIO
	
	/*Configurate for control button*/
	GPIOB->CRH &= ~(GPIO_CRH_MODE12);					//SET Start_Bt (PB12) as  floating input 
	GPIOB->CRH |= GPIO_CRH_CNF12_0;
	GPIOB->CRH &= ~(GPIO_CRH_MODE13);					//SET Stop_Bt (PB13) as  floating input 
	GPIOB->CRH |= GPIO_CRH_CNF13_0;
	GPIOB->CRH &= ~(GPIO_CRH_MODE14);					//SET Forward_Bt (PB14) as  floating input 
	GPIOB->CRH |= GPIO_CRH_CNF14_0;
	GPIOB->CRH &= ~(GPIO_CRH_MODE14);					//SET Backward_Bt (PB15) as  floating input 
	GPIOB->CRH |= GPIO_CRH_CNF14_0;
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PB | AFIO_EXTICR4_EXTI14_PB | AFIO_EXTICR4_EXTI15_PB; // SET LINE INTERRUPT FOR EXTI13,14,15
	EXTI->IMR |= EXTI_IMR_MR13 | EXTI_IMR_MR14 | EXTI_IMR_MR15;					//ENABLE interrupt mask for exti13,14,15
	EXTI->FTSR |= EXTI_FTSR_TR13 | EXTI_FTSR_TR14 | EXTI_FTSR_TR15;			//interrupt when detect falling edge

	/*Configurate for infrared sensor */
	GPIOB->CRL &= ~(GPIO_CRL_MODE0);					//SET sensor_S1 (PB0) as  floating input 
	GPIOB->CRL |= GPIO_CRL_CNF0_0;
	GPIOB->CRL &= ~(GPIO_CRL_MODE1);					//SET Start_Bt (PB1) as  floating input 
	GPIOB->CRL |= GPIO_CRL_CNF1_0;
	GPIOB->CRL &= ~(GPIO_CRL_MODE2);					//SET Start_Bt (PB2) as  floating input 
	GPIOB->CRL |= GPIO_CRL_CNF2_0;
	GPIOB->CRH &= ~(GPIO_CRH_MODE10);					//SET Start_Bt (PB10) as  floating input 
	GPIOB->CRH |= GPIO_CRH_CNF10_0;
	GPIOB->CRH &= ~(GPIO_CRH_MODE11);					//SET Start_Bt (PB11) as  floating input 
	GPIOB->CRH |= GPIO_CRH_CNF11_0;
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB | AFIO_EXTICR1_EXTI1_PB | AFIO_EXTICR1_EXTI2_PB; // SET LINE INTERRUPT FOR EXTI0,1,2
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PB | AFIO_EXTICR3_EXTI11_PB; // SET LINE INTERRUPT FOR EXTI10,11
	EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2 | EXTI_IMR_MR10 | EXTI_IMR_MR11; //enable interrupt mask for exti0,1,2,10,11
	EXTI->FTSR |= EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR10 | EXTI_FTSR_TR11;//interrupt when detect falling edge
	/*Set priority and enable IRQ via NVIC*/
	NVIC_SetPriority(EXTI0_IRQn,1);
	NVIC_SetPriority(EXTI1_IRQn,1);
	NVIC_SetPriority(EXTI2_IRQn,1);
	NVIC_SetPriority(EXTI15_10_IRQn,2);					
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	/*Confiure for extension output*/
	GPIOC->CRL |= GPIO_CRL_MODE4_1; 		//set OUT0 (PC4) as push-pull output, low frequency
	GPIOC->CRL &= ~(GPIO_CRL_CNF4);
	GPIOC->CRL |= GPIO_CRL_MODE4_1; 		//set OUT1 (PC5) as push-pull output, low frequency
	GPIOC->CRL &= ~(GPIO_CRL_CNF4);
	GPIOC->CRL |= GPIO_CRL_MODE4_1; 		//set OUT3 (PA15) as push-pull output, low frequency
	GPIOC->CRL &= ~(GPIO_CRL_CNF4);
	GPIOC->CRL |= GPIO_CRL_MODE4_1; 		//set OUT4 (PC12) as push-pull output, low frequency
	GPIOC->CRL &= ~(GPIO_CRL_CNF4);
	GPIOC->CRL |= GPIO_CRL_MODE4_1; 		//set OUT5 (PC4) as push-pull output, low frequency
	GPIOC->CRL &= ~(GPIO_CRL_CNF4);
	GPIOC->CRL |= GPIO_CRL_MODE4_1; 		//set OUT6 (PC5) as push-pull output, low frequency
	GPIOC->CRL &= ~(GPIO_CRL_CNF4);
	GPIOC->CRL |= GPIO_CRL_MODE4_1; 		//set OUT7 (PA15) as push-pull output, low frequency
	GPIOC->CRL &= ~(GPIO_CRL_CNF4);

}
/*handler for ext0 - sensor S1*/
void EXTI0_IRQHandler(void){
	/*sensor S1 detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR0){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR0);			//clear it pending 
	}
}
/*handler for ext1 - sensor S2*/
void EXTI1_IRQHandler(void){
	/*sensor S2 detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR1){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR1);			//clear it pending 
	}
}
/*handler for exti2 - sensor S3*/
void EXTI2_IRQHandler(void){
	/*sensor S3 detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR2){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR2);			//clear it pending 
	}
}

/*
handler for 
	exti10 - sensor S4,
	exti11 - sensor S5,
	exti13 - stop_bt,
	exti14 - forward_bt,
	exti15 - backward_bt
*/
void EXTI15_10_IRQHandler(void){
	/*sensor S4 detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR10){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR10);			//clear it pending 
	}
	/*sensor S5 detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR11){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR11);			//clear it pending 
	}
	/*stop_bt detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR13){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR13);			//clear it pending 
	}
	/*forward_bt detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR14){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR14);			//clear it pending 
	}
	/*backward_bt detect interrupt*/
	if(EXTI->PR&EXTI_PR_PR15){
		/*execute code */
		EXTI->PR &= ~(EXTI_PR_PR15);			//clear it pending 
	}
}


