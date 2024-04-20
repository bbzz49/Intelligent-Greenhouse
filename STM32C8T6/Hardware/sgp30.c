#include "stm32f10x.h" 
#include "Delay.h"
#include "Serial.h"
#include "jansson.h"
#include "My_I2C.h"
#include "OLED.h"
#include "InteractWithTcp.h"
#include "sgp30.h"
uint16_t CO2Data,TVOCData;
uint8_t SGP30_ErrorCount;
uint8_t SGP30_WorkStatus;
//初始化IIC接口
void SGP30_Init(void)
{
  My_I2C_Init();//SGP30_GPIO_Init();
  SGP30_Write(0x20, 0x03);
//	SGP30_ad_write(0x20,0x61);
//	SGP30_ad_write(0x01,0x00);
}

uint8_t SGP30_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;									//????????
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//????????
	{
		Timeout --;										//???,?????
		if (Timeout == 0) return 0;								//???0?,????
	}
	return 1;
}

uint8_t SGP30_Write(u8 HCmd, u8 LCmd) //向SGP30写入控制命令，一共16位，分俩个字节发送，先发高位
{
//	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); //等待EV5
	
	I2C_Send7bitAddress(I2C2, SGP30_write, I2C_Direction_Transmitter);	//写地址
	if(SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == 0) return 0;
	
	I2C_SendData(I2C2, HCmd);											
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			
	
	I2C_SendData(I2C2, LCmd);												
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				
	
	I2C_GenerateSTOP(I2C2, ENABLE);
	Delay_ms(100);
	return 1;
}

uint8_t SGP30_Read(void)//读取数据，为当前地址读模式
{
	uint32_t Dat;
	uint8_t crc;
    if(SGP30_Write(0x20,0x08) == 0)//0x2008 测量空气质量命令
	{
		SGP30_ErrorCount+=1;
		if(SGP30_ErrorCount == 3)
		{
			SGP30_WorkStatus = 1;
			CO2Data = 0;
		}			
		return 1;
	}

	
	I2C_GenerateSTART(I2C2, ENABLE);
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, SGP30_read, I2C_Direction_Receiver);	
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);	
	
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);	
	Dat = I2C_ReceiveData(I2C2);
	
	Dat <<= 8;
	
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//??EV7
	Dat += I2C_ReceiveData(I2C2);
	
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//??EV7
	crc = I2C_ReceiveData(I2C2);
	crc = crc;
	
	Dat <<= 8;
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//??EV7
	Dat += I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);									//??????????????????
	I2C_GenerateSTOP(I2C2, ENABLE);											//???????????????????
	
	Dat <<= 8;
	SGP30_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//??EV7
	Dat += I2C_ReceiveData(I2C2);											//???????
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	CO2Data = (Dat & 0xffff0000) >> 16;
	
	SGP30_ErrorCount = 0;
	SGP30_WorkStatus = 0;
	return 0;
}





