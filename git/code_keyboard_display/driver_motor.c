#include "driver_motor.h"

/**/
void driver_pin_init(void){
	/**/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
	/**/
	GPIO_InitStructure.GPIO_Pin = ENA0_Pin | ENB0_Pin | ENA1_Pin | ENB1_Pin | INA1_Pin | INB1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	/**/
	GPIO_InitStructure.GPIO_Pin = INA0_Pin | INB0_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/**/

	/**/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = PWM1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/**/
}


void driver_pwm_init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5|RCC_APB1Periph_TIM3,ENABLE);
	
	/**/
	RCC_GetClocksFreq(&RCC_ClockFreq);

	/*timer FOR motor 0*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 1023;
	TIM_TimeBaseStructure.TIM_Prescaler = 8;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM_PWM_MOTOR_0,&TIM_TimeBaseStructure);
	/*PWM1 mode configuration: Channel 1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM_PWM_MOTOR_0,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM_PWM_MOTOR_0, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM_PWM_MOTOR_0, ENABLE);


	/*timer FOR motor 1*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Period = 1023;
	TIM_TimeBaseStructure.TIM_Prescaler = (RCC_ClockFreq.PCLK1_Frequency/Tim_PWM_Freq_Out) - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM_PWM_MOTOR_1,&TIM_TimeBaseStructure);
	/*PWM1 mode configuration: Channel 1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM_PWM_MOTOR_1,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM_PWM_MOTOR_1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM_PWM_MOTOR_1, ENABLE);

	TIM_Cmd(TIM_PWM_MOTOR_1, ENABLE);
}


void driver_init(void){
	/**/
	driver_pin_init();
	driver_pwm_init();
	/**/
	GPIO_WriteBit(ENA0_GPIO_Port,ENA0_Pin,1);
	GPIO_WriteBit(ENA0_GPIO_Port,ENB0_Pin,1);
	GPIO_WriteBit(ENA1_GPIO_Port,ENA1_Pin,1);
	GPIO_WriteBit(ENB1_GPIO_Port,ENB1_Pin,1);
}


void driver_run(int speed){
		if(speed > 0){			//forward
			//motor_0 clockwise
			GPIO_WriteBit(INA0_GPIO_Port,INA0_Pin,1);
			GPIO_WriteBit(INB0_GPIO_Port,INB0_Pin,0);
			//motor_1 counter-clockwise
			GPIO_WriteBit(INA1_GPIO_Port,INA1_Pin,0);
			GPIO_WriteBit(INB1_GPIO_Port,INB1_Pin,1);	
		}
		else if (speed == 0){	//brake
			GPIO_WriteBit(INA0_GPIO_Port,INA0_Pin,1);
			GPIO_WriteBit(INB0_GPIO_Port,INB0_Pin,1);		
			GPIO_WriteBit(INA1_GPIO_Port,INA1_Pin,1);
			GPIO_WriteBit(INB1_GPIO_Port,INB1_Pin,1);				
		}
		else {						
			//motor_0 counter-clockwise
			GPIO_WriteBit(INA0_GPIO_Port,INA0_Pin,0);
			GPIO_WriteBit(INB0_GPIO_Port,INB0_Pin,1);		
			//motor_1 clockwise
			GPIO_WriteBit(INA1_GPIO_Port,INA1_Pin,1);
			GPIO_WriteBit(INB1_GPIO_Port,INB1_Pin,0);		
			
		}
		uint16_t temp = (uint16_t)(speed*1023/MAX_SPEED);
		TIM_PWM_MOTOR_1->CCR1 = temp;
		TIM_PWM_MOTOR_0->CCR1 = temp;
}