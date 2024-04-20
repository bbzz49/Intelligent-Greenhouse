#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "Delay.h"
#include "jansson.h"
#include "Serial.h"
#include "InteractWithTcp.h"
#include "OLED.h"
#define BH1750_ADDRESS		0x46
uint32_t light;
uint8_t BH1750_ErrorCount;
uint8_t BH1750_WorkStatus;
void Single_Write_BH1750(uint8_t REG_Address)
{
    MyI2C_Start();                  //起始信号
    MyI2C_SendByte(BH1750_ADDRESS);   //发送设备地址+写信号
    MyI2C_SendByte(REG_Address);    //内部寄存器地址，
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，
    MyI2C_Stop();                   //发送停止信号
}

void BH1750_Init(void)
{
	MyI2C_Init();
	Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
	Delay_ms(200);
}

uint8_t Multiple_read_BH1750(void)
{   
	uint32_t Data = 0;	
    MyI2C_Start();                               //起始信号
    if(MyI2C_SendByte(BH1750_ADDRESS + 1) == 0)//发送设备地址+读信号
	{
		BH1750_ErrorCount++;
		if(BH1750_ErrorCount == 3)
		{
			BH1750_WorkStatus = 1;
			light = 0;
		}
		return 0;
	}

	Data = MyI2C_ReceiveByte();
	Data = Data << 8;
	MyI2C_SendAck(0);

	Data |= MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	
	MyI2C_Stop(); 
	
	light = Data;
    Delay_ms(5);
	
	BH1750_ErrorCount = 0;
	BH1750_WorkStatus = 0;
	return 1;
}

