/*
 * Servo.c
 *
 *  Created on: Aug 7, 2023
 *      Author: ADMIN
 */

#include "Servo.h"
#include "servo_cf.h"

typedef struct
{
	uint16_t  Period_Min;
	uint16_t  Period_Max;
}SERVO_info;

static SERVO_info gs_SERVO_info[SERVO_NUM] = {0};


void SERVO_Init(uint16_t au16_SERVO_Instance)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_HandleTypeDef htim;
    uint32_t PSC_Value = 0;
    uint32_t ARR_Value = 0;
	/*--------[ Configure The Servo PWM GPIO Pin ]-------*/

    if(SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOA)
    {
    	__HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if(SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOB)
    {
    	__HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if(SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if(SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    else if(SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
	GPIO_InitStruct.Pin = SERVO_CfgParam[au16_SERVO_Instance].SERVO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SERVO_CfgParam[au16_SERVO_Instance].SERVO_GPIO, &GPIO_InitStruct);

	/*--------[ Calculate The PSC & ARR Values To Maximize PWM Resolution ]-------*/

	/* Those Equations Sets The F_pwm = 50Hz & Maximizes The Resolution*/
	PSC_Value = (uint32_t) (SERVO_CfgParam[au16_SERVO_Instance].TIM_CLK / 3276800.0);
	ARR_Value = (uint32_t) ((SERVO_CfgParam[au16_SERVO_Instance].TIM_CLK / (50.0*(PSC_Value+1.0)))-1.0);

	/*--------[ Configure The Servo PWM Timer Channel ]-------*/

	/*--[Check The Timer & Enable Its Clock]--*/
	if(SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
	}
	else if(SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
	}
	else if(SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
	}
	else if(SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();
	}

	htim.Instance = SERVO_CfgParam[au16_SERVO_Instance].TIM_Instance;
	htim.Init.Prescaler = PSC_Value;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = ARR_Value;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(&htim);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, SERVO_CfgParam[au16_SERVO_Instance].PWM_TIM_CH);

	/*--------[ Calculate & Save The Servo Pulse Information ]-------*/

	gs_SERVO_info[au16_SERVO_Instance].Period_Min = (uint16_t) (ARR_Value * (SERVO_CfgParam[au16_SERVO_Instance].MinPulse/20.0));
	gs_SERVO_info[au16_SERVO_Instance].Period_Max = (uint16_t) (ARR_Value * (SERVO_CfgParam[au16_SERVO_Instance].MaxPulse/20.0));

	/*--------[ Start The PWM Channel ]-------*/

	HAL_TIM_PWM_Start(&htim, SERVO_CfgParam[au16_SERVO_Instance].PWM_TIM_CH);
}

/* Moves A Specific Motor To A Specific Degree That Can Be Float Number */
void SERVO_MoveTo(uint16_t au16_SERVO_Instance, float af_Angle)
{
	uint16_t au16_Pulse = 0;

	au16_Pulse = ((af_Angle*(gs_SERVO_info[au16_SERVO_Instance].Period_Max - gs_SERVO_info[au16_SERVO_Instance].Period_Min))/180.0)
			+ gs_SERVO_info[au16_SERVO_Instance].Period_Min;

	*(SERVO_CfgParam[au16_SERVO_Instance].TIM_CCRx) = au16_Pulse;
}

