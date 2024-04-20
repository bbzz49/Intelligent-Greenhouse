#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "LED.h"
uint8_t light_status;
uint8_t light_workmode;

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(LED_IN1_GPIO_CLK | LED_IN2_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_IN1_GPIO_PIN | LED_IN2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_IN1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_Init(LED_IN2_GPIO_PORT, &GPIO_InitStructure);
	
	PWM_Init();
}

void LED_SetStrength(int8_t Strength)
{
	if (Strength >= 0)
	{
		LED_IN1 = 1;
		LED_IN2 = 0;
		PWM_SetCompare4(Strength);
	}
}

void LED_Control(uint8_t Switch)
{
	if(Switch == 0) 	 {LED_SetStrength(0); 	light_status = 0;}
	else if(Switch == 1) {LED_SetStrength(25); 	light_status = 1;}
	else if(Switch == 2) {LED_SetStrength(50); 	light_status = 2;}
	else if(Switch == 3) {LED_SetStrength(100); light_status = 3;}
}

