#include "vnh3sp30.h"

void VNH3SP30_Config_Pin(){
	//enable clock for gpio port A and port C
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;	
	//enable clock for AFIO
	//RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	//set ENA0, ENB0, ENA1, ENB1 (PC0,PC1,PC2,PC3) as push-pull output, low frequency
	GPIOC->CRL |= GPIO_CRL_MODE0_1 | GPIO_CRL_MODE1_1 | GPIO_CRL_MODE2_1 | GPIO_CRL_MODE3_1;
	GPIOC->CRL &=~ (GPIO_CRL_CNF0); 
	GPIOC->CRL &=~ (GPIO_CRL_CNF1);
	GPIOC->CRL &=~ (GPIO_CRL_CNF2);
	GPIOC->CRL &=~ (GPIO_CRL_CNF3);
	//set INA0, INB0, INA1, INB1 (PA1,PA2,PC7,PC8) as push-pull output, low frequency
	GPIOA->CRL |= GPIO_CRL_MODE1_1 | GPIO_CRL_MODE2_1;
	GPIOC->CRL |= GPIO_CRL_MODE7_1;
	GPIOC->CRH |= GPIO_CRH_MODE8_1;
	//set PWM0, PWM1 (PA0,PC6) as alternative push-pull output, high frequency
	GPIOA->CRL |= GPIO_CRL_MODE0;
	GPIOA->CRL |= GPIO_CRL_CNF0_1;
	GPIOC->CRL |= GPIO_CRL_MODE6;
	GPIOC->CRL |= GPIO_CRL_CNF6_1;
	//set CS0,CS1 (PA3, PC9) as analog input
	GPIOA->CRL &= ~(15<<12);	
	GPIOC->CRH &= ~(15<<4);
}

void VNH3SP30_Config_PWM(){
	//clock for timer 3 and timer 5 is 16MHz
	//enable clock for timer 3 & timer 5
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM5EN;
	//set operation mode for timer 5 C1, timer 3 C1
	/*set OC1 is output in corresponding pin*/
	TIM_MOTOR_1->CCER |= TIM_CCER_CC1E;
	TIM_MOTOR_0->CCER |= TIM_CCER_CC1E;
	/*Enable auto-reload preload*/
	TIM_MOTOR_1->CR1 |= TIM_CR1_ARPE;
	TIM_MOTOR_0->CR1 |= TIM_CR1_ARPE;
	/*set timer pwm of both side as pwm mode 1, enable OC1 preload*/ 
	TIM_MOTOR_1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
	TIM_MOTOR_0->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
	/*parameter for pwm*/
	TIM_MOTOR_1->PSC = 6400;		//Fpwm = 2.5KHZ
	TIM_MOTOR_0->PSC = 6400;
	TIM_MOTOR_1->ARR = 1023;
	TIM_MOTOR_0->ARR = 1023;
	TIM_MOTOR_1->CCR1 = 0;
	TIM_MOTOR_0->CCR1 = 0;
	/*update generation*/
	TIM_MOTOR_1->EGR |= TIM_EGR_UG;
	TIM_MOTOR_0->EGR |= TIM_EGR_UG;
	/*counter enable*/
	TIM_MOTOR_1->CR1 |= TIM_CR1_CEN;
	TIM_MOTOR_0->CR1 |= TIM_CR1_CEN;
}
/*
parameter:
- NUM: which dc motor side is chose
- Direction: clock wise or counter clock wise
- speed : rev/s, max_speed =  3000 rev/min ~ 50 rev/s
*/
void VNH3SP30_DC_Control(DC_number num,DC_operation_mode mode,DC_direction dir,int speed){
	uint16_t temp = 0;
	//if dc 0 side chosed
	if(num == VNH3SP30_DC_0){
		//set ENA,ENB high
		GPIOC->BSRR |= GPIO_BSRR_BS0 | GPIO_BSRR_BS1;
		//which mode are chose
		if(mode == VNH3SP30_RUN){
			if(speed < 0){
				if(dir == VNH3SP30_CW)
					dir = VNH3SP30_CCW;
				if(dir == VNH3SP30_CCW)
					dir = VNH3SP30_CW;
				speed = -speed;
			}	
			//which direction
			if(dir == VNH3SP30_CW){
				GPIOA->BSRR |= GPIO_BSRR_BS1;
				GPIOA->BRR |= GPIO_BRR_BR2;
			}
			if(dir == VNH3SP30_CCW) {
				GPIOA->BSRR |= GPIO_BSRR_BS1;
				GPIOA->BRR |= GPIO_BRR_BR2;
			}
			//write data into CCR to generate PWM			
			temp = 1023*speed/50;
			TIM_MOTOR_0->CCR1 = temp;
		}
		//brake mode
		if(mode == VNH3SP30_BRAKE){
			GPIOA->BSRR |= GPIO_BSRR_BS1;
			GPIOA->BSRR |= GPIO_BSRR_BS2;
		}	
	}
	//if dc 1 side chosed
	if(num == VNH3SP30_DC_1){
		//set ENA,ENB high
		GPIOC->BSRR |= GPIO_BSRR_BS2 | GPIO_BSRR_BS3;
		//which mode are chose
		if(mode == VNH3SP30_RUN){
			if(speed < 0){
				if(dir == VNH3SP30_CW)
					dir = VNH3SP30_CCW;
				if(dir == VNH3SP30_CCW)
					dir = VNH3SP30_CW;
				speed = -speed;
			}	
			//which direction
			if(dir == VNH3SP30_CW){
				GPIOC->BSRR |= GPIO_BSRR_BS7;
				GPIOC->BRR |= GPIO_BRR_BR8;
			}
			if(dir == VNH3SP30_CCW) {
				GPIOC->BSRR |= GPIO_BSRR_BS8;
				GPIOC->BRR |= GPIO_BRR_BR7;
			}
			//write data into CCR to generate PWM			
			temp = 1023*speed/50;
			TIM_MOTOR_0->CCR1 = temp;		
		}
		//brake mode
		if(mode == VNH3SP30_BRAKE){
			GPIOA->BSRR |= GPIO_BSRR_BS1;
			GPIOA->BSRR |= GPIO_BSRR_BS2;
		}	
	}
}





