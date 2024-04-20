#include "stm32f10x.h"                  // Device header
#include "IC.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include <string.h> 
#include <stdio.h>
#include "jansson.h"
#include "InteractWithTcp.h"
uint8_t tumid_int,tumid_frac,temp_int,temp_frac,check_sum;
uint8_t i,lst_pulse_perid[43] = {0},lst_bit[40];
float air_temperature,air_humidity;
uint8_t DTH11_ErrorCount;
uint8_t DTH11_WorkStatus;
void Dth11_Init(void)
{
	IC_Init();
}

uint8_t Pow(uint8_t X,uint8_t Y)
{
	uint8_t i;
	uint8_t result = 1;
	for(i = 0;i < Y;i ++)
	{
		result *= X;
	}
	return result;
}

void Dth11_Read(void)
{	
	i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置GPIO口为推挽输出，为了向Dth11发送开始读取温度的信号
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	Delay_ms(20);
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//设置GPIO口为上拉输入的模式，接收来自Dth11的回复
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	Delay_ms(100);
	//接收回复完成后，根据每次引脚电平检测到上升沿的间隔判断收到的数据位是1还是0
	uint8_t j;
	for(i = 0,j = 3;i<40;i ++,j ++)
	{
		if(lst_pulse_perid[j] > 70 && lst_pulse_perid[j] < 90) lst_bit[i] = 0;
		else if(lst_pulse_perid[j] > 115 && lst_pulse_perid[j] < 135) lst_bit[i] = 1;
		else lst_bit[i] = 2;
	}
	tumid_int = 0;
	tumid_frac = 0;
	temp_int = 0;
	temp_frac = 0;
	check_sum = 0;
	//收到的数据一共40bit
	for(j = 0;j < 8;j ++)//表示湿度整数位
	{
		tumid_int += lst_bit[j]*Pow(2,7-j);
	}
	for(j = 8;j < 16;j ++)//表示湿度小数位
	{
		tumid_frac += lst_bit[j]*Pow(2,15-j);
	}
	for(j = 16;j < 24;j ++)//表示温度整数位
	{
		temp_int += lst_bit[j]*Pow(2,23-j);
	}
	for(j = 24;j < 32;j ++)//表示温度小数位
	{
		temp_frac += lst_bit[j]*Pow(2,31-j);
	}
	for(j = 32;j < 40;j ++)//表示校验位
	{
		check_sum += lst_bit[j]*Pow(2,39-j);
	}
	if(check_sum == (tumid_int + tumid_frac + temp_int + temp_frac))//数据无误
	{
		air_temperature = temp_int + temp_frac / 10.0;
		air_temperature = (uint16_t)(air_temperature * 10) / 10.0;//保留一位小数
		
		air_humidity = tumid_int;
		DTH11_ErrorCount = 0;
		DTH11_WorkStatus = 0;
	}
	else
	{	
		DTH11_ErrorCount ++;
		if(DTH11_ErrorCount == 3)
		{
			air_temperature = 0;
			air_humidity = 0;
			DTH11_WorkStatus = 1;
		}		
	}
	memset(lst_pulse_perid, 0, sizeof lst_pulse_perid);
}

void Dth11_OLED_Show(void)
{
	uint8_t i,Line = 1,Column = 1;
	for(i = 0;i < 20;i ++)
	{
		OLED_ShowNum(Line,Column,lst_pulse_perid[i],3);
		Column += 3 ;
		if(Column >= 16)
		{
			Column = 1;
			Line ++;
		}
	}
}


/**
  * @brief	捕获到引脚上升沿时，将当前计数器的值记录下来
  * @param	无
  * @retval	无
  */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
	{
		lst_pulse_perid[i] = TIM_GetCapture1(TIM3) + 1;
		i++;
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
	}
}

