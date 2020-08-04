#ifndef __VNH3SP30_H
#define __VNH3SP30_H

#include "stm32f10x.h"                  // Device header
#include <stdint.h>

#define TIM_MOTOR_0			TIM5
#define TIM_MOTOR_1 		TIM3

typedef enum{
	VNH3SP30_DC_0,
	VNH3SP30_DC_1
} DC_number;

typedef enum{
	VNH3SP30_RUN,
	VNH3SP30_BRAKE
} DC_operation_mode;

typedef enum{
	VNH3SP30_WAIT,
	VNH3SP30_STOP,
	VNH3SP30_STEADY_RUN,
	VNH3SP30_INCREASE_DECRESE
} DC_Status;

typedef enum{
	VNH3SP30_CW,
	VNH3SP30_CCW
} DC_direction;

void VNH3SP30_Config_Pin();
void VNH3SP30_Config_PWM();
void VNH3SP30_DC_Control(DC_number,DC_operation_mode,DC_direction,int);


#endif

