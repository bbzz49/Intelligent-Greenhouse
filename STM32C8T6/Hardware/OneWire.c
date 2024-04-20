#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OneWire.h"

void OneWire_Init(void)
{
	RCC_APB2PeriphClockCmd(OneWire_DQ_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = OneWire_DQ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OneWire_DQ_GPIO_PORT, &GPIO_InitStructure);
}

void OneWire_Set_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = OneWire_DQ_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OneWire_DQ_GPIO_PORT, &GPIO_InitStructure);
}

void OneWire_Set_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = OneWire_DQ_GPIO_PIN;
	GPIO_Init(OneWire_DQ_GPIO_PORT, &GPIO_InitStructure);
}


void OneWire_SendBit(uint8_t Bit)
{
	OneWire_Set_Out();
	OneWire_DQ_OUT = 0;
	Delay_us(2);
	if(Bit) OneWire_DQ_OUT = 1;
	else OneWire_DQ_OUT = 0;
	Delay_us(60);
	OneWire_DQ_OUT = 1;
}

uint8_t OneWire_ReceiveBit(void)
{
	uint8_t Bit;
	OneWire_Set_Out();
	OneWire_DQ_OUT = 0;
	Delay_us(2);
	OneWire_DQ_OUT = 1;
	OneWire_Set_In();
	Bit = OneWire_DQ_IN;
	Delay_us(60);
	return Bit;
}

void OneWire_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0;i < 8;i ++)
	{
		OneWire_SendBit(Byte&(0x01<<i));
	}
}

uint8_t OneWire_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	for(i = 0;i < 8;i ++)
	{
		if(OneWire_ReceiveBit()){Byte|=(0x01<<i);}
	}
	return Byte;
}
