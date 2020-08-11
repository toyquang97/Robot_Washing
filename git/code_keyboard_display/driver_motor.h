#ifndef __DRIVER_MOTOR_H
#define __DRIVER_MOTOR_H

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#define Tim_PWM_Freq_Out	2560000
#define TIM_PWM_MOTOR_0		TIM5
#define TIM_PWM_MOTOR_1		TIM3
#define MAX_SPEED 				50

#define ENA0_Pin GPIO_Pin_0
#define ENA0_GPIO_Port GPIOC
#define ENB0_Pin GPIO_Pin_1
#define ENB0_GPIO_Port GPIOC
#define ENA1_Pin GPIO_Pin_2
#define ENA1_GPIO_Port GPIOC
#define ENB1_Pin GPIO_Pin_3
#define ENB1_GPIO_Port GPIOC
#define INA1_Pin GPIO_Pin_7
#define INA1_GPIO_Port GPIOC
#define INB1_Pin GPIO_Pin_8
#define INB1_GPIO_Port GPIOC

#define INA0_Pin GPIO_Pin_1
#define INA0_GPIO_Port GPIOA
#define INB0_Pin GPIO_Pin_2
#define INB0_GPIO_Port GPIOA

#define PWM0_Pin GPIO_Pin_0
#define PWM0_GPIO_Port GPIOA
#define PWM1_Pin GPIO_Pin_6
#define PWM1_GPIO_Port GPIOA


extern GPIO_InitTypeDef GPIO_InitStructure;
extern RCC_ClocksTypeDef RCC_ClockFreq;
extern TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
extern TIM_OCInitTypeDef  TIM_OCInitStructure;

void driver_pin_init(void);
void driver_pwm_init(void);
void driver_init(void);
/**/
void driver_run(int);

/**/


#endif