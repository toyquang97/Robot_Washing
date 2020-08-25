#include "driver_motor.h"

/**/

void driver_init(void){
	/*This function will set all enable pin and start 2 timer for pwm generation of 2 dc motor controller*/
	HAL_GPIO_WritePin(ENA0_GPIO_Port,ENA0_Pin,1);
	HAL_GPIO_WritePin(ENB0_GPIO_Port,ENB0_Pin,1);
	HAL_GPIO_WritePin(ENA1_GPIO_Port,ENA1_Pin,1);
	HAL_GPIO_WritePin(ENB1_GPIO_Port,ENB1_Pin,1);
	
	HAL_TIM_PWM_Start(&TIM_PWM_MOTOR_0,TIM_CHANNEL_MOTOR_0);
	HAL_TIM_PWM_Start(&TIM_PWM_MOTOR_1,TIM_CHANNEL_MOTOR_1);
}

void driver_run(int speed){
		if(speed > 0){			//forward
			//motor_0 clockwise
			HAL_GPIO_WritePin(INA0_GPIO_Port,INA0_Pin,1);
			HAL_GPIO_WritePin(INB0_GPIO_Port,INB0_Pin,0);
			//motor_1 counter-clockwise
			HAL_GPIO_WritePin(INA1_GPIO_Port,INA1_Pin,1);
			HAL_GPIO_WritePin(INB1_GPIO_Port,INB1_Pin,0);
		}
		else if (speed == 0){	//brake
			HAL_GPIO_WritePin(INA0_GPIO_Port,INA0_Pin,1);
			HAL_GPIO_WritePin(INB0_GPIO_Port,INB0_Pin,1);
			HAL_GPIO_WritePin(INA1_GPIO_Port,INA1_Pin,1);
			HAL_GPIO_WritePin(INB1_GPIO_Port,INB1_Pin,1);			
		}
		else {						
			//motor_0 counter-clockwise
			HAL_GPIO_WritePin(INA0_GPIO_Port,INA0_Pin,0);
			HAL_GPIO_WritePin(INB0_GPIO_Port,INB0_Pin,1);
			//motor_1 clockwise
			HAL_GPIO_WritePin(INA1_GPIO_Port,INA1_Pin,0);
			HAL_GPIO_WritePin(INB1_GPIO_Port,INB1_Pin,1);
			speed = -speed;
			
		}
		if(speed > MAX_SPEED){
			speed = MAX_SPEED;
		}
		uint16_t temp = (uint16_t)(speed*(TIM_PWM_MOTOR_0.Init.Period)/MAX_SPEED);
		__HAL_TIM_SET_COMPARE(&TIM_PWM_MOTOR_0,TIM_CHANNEL_MOTOR_0,temp);
		__HAL_TIM_SET_COMPARE(&TIM_PWM_MOTOR_1,TIM_CHANNEL_MOTOR_1,temp);
	
}




