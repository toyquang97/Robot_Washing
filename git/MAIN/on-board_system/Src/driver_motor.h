#ifndef __DRIVER_MOTOR_H
#define __DRIVER_MOTOR_H

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "math.h"
#define Tim_PWM_Freq_Out	250000
#define TIM_PWM_MOTOR_0		htim5
#define TIM_PWM_MOTOR_1		htim3
#define TIM_CHANNEL_MOTOR_0 TIM_CHANNEL_1
#define TIM_CHANNEL_MOTOR_1 TIM_CHANNEL_MOTOR_0
#define MAX_SPEED 				50


void driver_init(void);
/**/
void driver_run(int);

/**/


#endif