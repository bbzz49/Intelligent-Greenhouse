#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Serial.h"
#include "OLED.h"
#include "string.h"
#include "LED.h"
#include "Motor.h"
// #include <jansson.h>

void Esp8266_Init(void)
{
	Serial_Init();
}

uint8_t Esp8266_WriteCmd(char *Command, char *Response1, char *Response2, uint8_t Count,uint8_t Clean_buff)
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
				Serial_OledShowString(Serial_RxFromEspBuf);
				Delay_s(1);
				break;
			}
			else waitTime -= 1;
		}
		if(strstr(Serial_RxFromEspBuf, Response1) != NULL || strstr(Serial_RxFromEspBuf, Response2) != NULL) break;
	}
	if(Clean_buff == 1) Serial_RxFromEspBuf_Clear();
	if(Count > 0) return 1;
	else return 0;
}

uint8_t Esp8266_detect(void)
{
	if(Esp8266_WriteCmd("AT", "OK", "OK", 5, 1) == 0)
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
	
	if(Esp8266_WriteCmd("AT+RST", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"RST Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"RST OK");
	Delay_ms(500);
	
	if(Esp8266_WriteCmd("AT+CWMODE=1", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"STA Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"AT+CWMODE=1 OK");
	Delay_ms(500);

	if(Esp8266_WriteCmd("AT+RST", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"2 RST Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"2 RST OK");
	Delay_ms(500);
	
	if(Esp8266_WriteCmd("ATE0", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"ATE0 Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"ATE0 OK");
	Delay_ms(500);
	
	if(Esp8266_WriteCmd("AT+CWJAP?", "No", "No", 5, 1))
	{
		if(Esp8266_WriteCmd("AT+CWJAP=\"Xiaomi_78AA\",\"a123456789\"", "OK", "OK", 10, 1))
		{
			OLED_Clear();
			OLED_ShowString(1,1,"WIFI Failed");
			return 0;
		}
	}
	OLED_Clear();
	OLED_ShowString(1,1,"WIFI OK");
	Delay_s(5);
	
	if(Esp8266_WriteCmd("AT+MQTTUSERCFG=0,1,\"python-mqtt-stm32\",\"stm32\",\"123456789\",0,0,\"\"", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"USERCFG Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"USERCFG OK");
	Delay_ms(500);
	
	if(Esp8266_WriteCmd("AT+MQTTCONNCFG=0,120,1,\"\",\"\",0,0", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"CONNCFG Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"CONNCFG OK");
	Delay_ms(500);
	
	if(Esp8266_WriteCmd("AT+MQTTCONN=0,\"118.31.35.191\",1883,1", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"CONN Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"CONN OK");
	Delay_ms(500);
	
	if(Esp8266_WriteCmd("AT+MQTTSUB=0,\"prop/set\",2", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"SUB Failed");
		return 0;
	}
	if(Esp8266_WriteCmd("AT+MQTTSUB=0,\"service/call\",2", "OK", "OK", 5, 1) == 0)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"SUB Failed");
		return 0;
	}
	OLED_Clear();
	OLED_ShowString(1,1,"SUB OK");
	
	return 1;
}

uint8_t Esp8266_Mqtt_Connect(void)
{
	if(Esp8266_detect() == 1 && Esp8266_ModeNetSet() == 1) return 1;
	else return 0;
}







