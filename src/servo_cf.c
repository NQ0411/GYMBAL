/*
 * servo_cf.c
 *
 *  Created on: Aug 7, 2023
 *      Author: ADMIN
 */

#include "servo.h"

const SERVO_CfgType SERVO_CfgParam[SERVO_NUM] =
{
	// Servo Motor 1 Configurations
    {
	    GPIOA,
		GPIO_PIN_0,
		TIM2,
		&TIM2->CCR1,
		TIM_CHANNEL_1,
		72000000,
		0.65,
		2.3
	},
	  {
	    GPIOA,
	    GPIO_PIN_1,
	    TIM2,
	    &TIM2->CCR2,
	    TIM_CHANNEL_2,
	    72000000,
	    0.75,
	    2.5
	  }
};


