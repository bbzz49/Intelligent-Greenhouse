#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Motor.h"

uint8_t fan_status;
uint8_t fan_workmode;

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(Motor_IN1_GPIO_CLK | Motor_IN2_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Motor_IN1_GPIO_PIN | Motor_IN2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor_IN1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_Init(Motor_IN2_GPIO_PORT, &GPIO_InitStructure);
	PWM_Init();
}

void Motor_SetSpeed(int8_t Speed)
{
	if (Speed < 0)
	{
		Motor_IN2 = 1;
		Motor_IN1 = 0;
		PWM_SetCompare3(Speed);
	}
	else
	{
		Motor_IN2 = 0;
		Motor_IN1 = 1;
		PWM_SetCompare3(-Speed);
	}
}

void Motor_Control(uint8_t Switch)
{
	if(Switch == 0) 	 {Motor_SetSpeed(0);	fan_status = 0;}
	else if(Switch == 1) {Motor_SetSpeed(40);	fan_status = 1;}
	else if(Switch == 2) {Motor_SetSpeed(70);	fan_status = 2;}
	else if(Switch == 3) {Motor_SetSpeed(100);	fan_status = 3;}
	else if(Switch == 4) {Motor_SetSpeed(-40);	fan_status = 4;}
	else if(Switch == 5) {Motor_SetSpeed(-70);	fan_status = 5;}
	else if(Switch == 6) {Motor_SetSpeed(-100);	fan_status = 6;}
}
