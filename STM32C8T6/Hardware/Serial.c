#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"
#include "String.h"
char Serial_RxFromEspBuf[500];
char Serial_RxFromTcpBuf[500];
char Rx_Temp[500];
uint8_t Serial_RxFlag;//串口是否完整接收一个定义的字符串
uint16_t pRx_State;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(Serial_TX_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = Serial_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Serial_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Serial_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Serial_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for( i = 0;i < Length;i ++ )
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint16_t i;
	for(i = 0;String[i] != '\0';i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while(Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i = 0;i < Length;i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}
 

void Serial_RxFromEspBuf_Clear()
{	
	memset(Serial_RxFromEspBuf, 0, sizeof Serial_RxFromEspBuf);
}

void Serial_RxFromTcpBuf_Clear()
{	
	Serial_RxFlag = 0;
	memset(Serial_RxFromTcpBuf, 0, sizeof Serial_RxFromTcpBuf);
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}



void Serial_OledShowString(char *String)
{
	uint8_t i,Line = 1,Column = 1;
	for (i = 0; i < 64; i++)
	{	
		
		if(String[i] != '\0') OLED_ShowChar(Line,Column,String[i]);
		else OLED_ShowChar(Line,Column,' ');
		Column ++;
		if(Column > 16)
		{
			Column = 1;
			Line ++;
		}
	}
}



void USART1_IRQHandler(void)
{	
	static uint8_t Rx_State = 1;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		if(pRx_State > 498) pRx_State = 0;
		if(RxData == '\'') RxData = '\"';
		if(Rx_State == 1)//状态机为1，表示当前在等待正常字节数据
		{ 
			if(RxData == '\r')//接收到ESP8266一次通信结束标志之一，把状态机改为2
			{
				Rx_State = 2;
			}
			else if(RxData == ':')
			{
				if(strstr(Rx_Temp, "IPD") != NULL)
				{
					pRx_State = 0;
					Rx_State = 3;
					memset(Rx_Temp, 0, sizeof Rx_Temp);
				}
				else
				{
					Rx_Temp[pRx_State] = RxData;
					pRx_State ++;
				}
			}
			else
			{	
				Rx_Temp[pRx_State] = RxData;
				pRx_State ++;
			}
		} 
		else if(Rx_State == 2)//状态机为2，等待ESP8266一次通信结束的最后标志，把状态机改为1，等待下次通信
		{
			if(RxData == '\n')
			{
				Rx_State = 1;
				pRx_State = 0;
				if((strlen(Rx_Temp) + strlen(Serial_RxFromEspBuf)) < 500)
				{
					strcat(Serial_RxFromEspBuf, Rx_Temp);
				}
				else
				{
					memset(Serial_RxFromEspBuf, 0, sizeof Serial_RxFromTcpBuf);
					strcat(Serial_RxFromEspBuf, Rx_Temp);
				}
				memset(Rx_Temp, 0, sizeof Rx_Temp);
			}
		}
		else if(Rx_State == 3)
		{
			if(RxData == '}')//接收到TCP一次通信结束标志，把状态机改为1，等待下次通信
			{
				Rx_State = 1;
				Rx_Temp[pRx_State] = RxData;
				pRx_State ++;
				Rx_Temp[pRx_State] = '\n';
				if((strlen(Rx_Temp) + strlen(Serial_RxFromTcpBuf)) < 500)
				{
					strcat(Serial_RxFromTcpBuf, Rx_Temp);
				}
				else
				{
					memset(Serial_RxFromTcpBuf, 0, sizeof Serial_RxFromTcpBuf);
					strcat(Serial_RxFromTcpBuf, Rx_Temp);
				}
				memset(Rx_Temp, 0, sizeof Rx_Temp);
				pRx_State = 0;
				Serial_RxFlag = 1;
			}
			else
			{	
				Rx_Temp[pRx_State] = RxData;
				pRx_State ++;
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	} 
}

