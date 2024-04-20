#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "OLED.h"
#include "string.h"
#include "LED.h"
#include "Motor.h"
#include <jansson.h>

void Esp8266_Init(void)
{
	Serial_Init();
}

uint8_t Esp8266_WriteCmd(char *Command, char *Response1, char *Response2, uint8_t Count)
{
	uint16_t waitTime;
	uint16_t chr_cnts_0, chr_cnts_1;
	char cmd[strlen(Command) + 3];
	strcpy(cmd, Command);
	strcat(cmd, "\r\n");
	while(Count > 0)
	{
		Count --;
		waitTime = 5000;
		Serial_SendString(cmd);
		while(waitTime != 0)
		{
			chr_cnts_0 = strlen(Serial_RxFromEspBuf);
			Delay_ms(1);
			chr_cnts_1 = strlen(Serial_RxFromEspBuf);
			if (chr_cnts_1 != 0 && chr_cnts_1==chr_cnts_0)
			{
	//			Serial_OledShowString(Serial_RxFromEspBuf);
	//			Delay_s(5);
				break;//判断已经收到来自ESP8266一次完整的回复
			}
			else waitTime -= 1;
		}
		if(strstr(Serial_RxFromEspBuf, Response1) != NULL || strstr(Serial_RxFromEspBuf, Response2) != NULL) break;
	}
	Serial_RxFromEspBuf_Clear();
	if(Count > 0) return 1;
	else return 0;
}

uint8_t Esp8266_detect(void)
{
	if(Esp8266_WriteCmd("AT", "OK", "OK", 5) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"Esp8266 ERROR");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"Esp8266 OK");
	return 1;
}

uint8_t Esp8266_ModeNetSet(void) //
{
	
	if(Esp8266_WriteCmd("AT+RST", "OK", "OK", 5) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"RST Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"RST OK");
	Delay_ms(500);
	
	if(Esp8266_WriteCmd("AT+CWMODE=1", "OK", "OK", 5) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"STA Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"AT+CWMODE=1 OK");
	Delay_ms(500);

	if(Esp8266_WriteCmd("AT+RST", "OK", "OK", 5) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"2 RST Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"2 RST OK");
	Delay_ms(500);
	
	
	if(Esp8266_WriteCmd("AT+CWJAP?", "No", "No", 5))
	{
		if(Esp8266_WriteCmd("AT+CWJAP=\"Xiaomi_78AA\",\"a123456789\"", "OK", "OK", 10))
		{
			OLED_Clear();
			OLED_ShowString(1,1,"WIFI Failed");
			return 0;
		}
	}
	OLED_Clear();
	OLED_ShowString(1,1,"WIFI OK");
	Delay_s(5);
	
	if(Esp8266_WriteCmd("AT+CIPMUX=0", "OK", "OK", 5) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"AT+CIPMUX=0 Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"AT+CIPMUX=0 OK");
	Delay_ms(500);
	//设置ESP8266为单连接模式，只与一个服务器建立连接
	
	if(Esp8266_WriteCmd("AT+CIPSTART=\"TCP\",\"192.168.31.184\",8086", "OK", "ALREADY", 5) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"TCP Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"TCP OK");
	Delay_ms(500);
	
//	Count = 5;
//	//ESP8266设置为透传模式，在该模式下，ESP8266不理会任何命令，只充当数据传输的桥梁
//	while(Count >0)
//	{
//		if(Esp8266_WriteCmd("AT+CIPMODE=1")) break;
//		Delay_s(2);
//		Count --;
//	}
//	if(Count <= 0) return 0;
//	OLED_Clear();
//	OLED_ShowString(1,1,"AT+CIPMODE=1 OK");

	
//	Count = 5;
//	//ESP8266开启透传
//	while(Count >0)
//	{
//		if(Esp8266_WriteCmd("AT+CIPSEND")) break;
//		Delay_s(2);
//		Count --;
//	}
//	if(Count <= 0) return 0;
//	OLED_Clear();
//	OLED_ShowString(1,1,"AT+CIPSEND OK");
	
	return 1;
}

uint8_t Esp8266_WifiAndTcp_Connect(void)
{
	if(Esp8266_detect() == 1 && Esp8266_ModeNetSet() == 1) return 1;
	else return 0;
}

uint8_t ReconnectToTcp(void)
{
	OLED_Clear();
	OLED_ShowString(1,1,"RECONNECTING");
	if(Esp8266_WriteCmd("AT+CIPCLOSE", "OK", "ERROR", 5))
	{	
		if(Esp8266_WriteCmd("AT+CIPSTART=\"TCP\",\"192.168.31.184\",8086", "OK", "ALREADY", 5))
		{
			OLED_Clear();
			OLED_ShowString(1,1,"RECONNECTED");
			return 1;
		}
		else
		{
			OLED_Clear();
			OLED_ShowString(1,1,"May be no wifi");
			if(Esp8266_WriteCmd("AT+CWJAP=\"Xiaomi_78AA\",\"a123456789\"", "OK", "OK", 10))
			{
				OLED_Clear();
				OLED_ShowString(1,1,"wifi OK");
				if(Esp8266_WriteCmd("AT+CIPSTART=\"TCP\",\"192.168.31.184\",8086", "OK", "ALREADY", 5))
				{
					OLED_Clear();
					OLED_ShowString(1,1,"RECONNECTED");
					return 1;
				}
				else
				{
					Serial_OledShowString(Serial_RxFromEspBuf);
					Delay_s(2);
					OLED_Clear();
					OLED_ShowString(1,1,"ESP8266 ERROR");
					Esp8266_WriteCmd("AT+RST", "OK", "OK", 5);
					Esp8266_WifiAndTcp_Connect();
					return 0;
				}
			}
			else
			{
				Serial_OledShowString(Serial_RxFromEspBuf);
				Delay_s(2);
				OLED_Clear();
				OLED_ShowString(1,1,"ESP8266 ERROR");
				Esp8266_WriteCmd("AT+RST", "OK", "OK", 5);
				Esp8266_WifiAndTcp_Connect();
				return 0;
			}
				
		}
	}
	else
	{
		Serial_OledShowString(Serial_RxFromEspBuf);
		Delay_s(2);
		OLED_Clear();
		OLED_ShowString(1,1,"ESP8266 ERROR");
		Esp8266_WriteCmd("AT+RST", "OK", "OK", 5);
		Esp8266_WifiAndTcp_Connect();
		return 0;
	}
}






