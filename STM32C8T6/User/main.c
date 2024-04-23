#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "InteractWithTcp.h"
#include "Pump.h"
#include "LED.h"
#include "Valve1.h"
#include "Motor.h"
#include "OLED.h"
#include "Pump.h"
#include "Valve1.h"
#include "HeatingCore.h"
#include "Serial.h"
#include "_28BYJ48.h"
#include "Esp8266.h"
#include "jansson.h"
#include "MyRtc.h"
int main(void)
{	
	OLED_Init();
	OLED_ShowString(1,1,"OK");
	MyRTC_Init();
	MyRTC_SetTime();
//	Esp8266_Init();
//	if(Esp8266_Mqtt_Connect() == 0)
//	{
//		OLED_ShowString(2,1,"Error");

//	}
	while (1)
	{
		MyRTC_ReadTime();
		OLED_ShowNum(4,1,MyRTC_Time[3],2);
		OLED_ShowNum(4,4,MyRTC_Time[4],2);
		OLED_ShowNum(4,7,MyRTC_Time[5],2);
		ControlDevice();
		Delay_ms(500);

	}

}

