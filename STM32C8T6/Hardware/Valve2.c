#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint8_t valve2_status;
uint8_t valve2_workmode;

void Valve2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
}

void Valve2_Control(uint8_t Switch)
{
	if(Switch == 1) {GPIO_SetBits(GPIOB, GPIO_Pin_14); valve2_status = 1;}
	else {GPIO_ResetBits(GPIOB, GPIO_Pin_14); valve2_status = 0;}
}

