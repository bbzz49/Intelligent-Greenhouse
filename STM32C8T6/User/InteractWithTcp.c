
#include "stm32f10x.h"// Device header
#include "String.h"
#include "Delay.h"
#include "jansson.h"
#include "Serial.h"
#include "OLED.h"
#include "LED.h"
#include "Dth11.h"
#include "PhSensor.h"
#include "DS18B20.h"
#include "BH1750.h"
#include "SoilHumSensor.h"
#include "Pump.h"
#include "Motor.h"
#include "Valve1.h"
#include "Valve2.h"
#include "HeatingCore.h"
#include "Esp8266.h"
#include "_28BYJ48.h"
#include "sgp30.h"
#include "stdlib.h"
#include "MyRtc.h"
#include "MyFlash.h"
#include "Store.h"

extern Flash Store_Data1[];

void Module_Init(void)
{
	OLED_Init();
	Esp8266_Init();
	Esp8266_Mqtt_Connect();
	BH1750_Init();
	Pump_Init();
	Valve1_Init();
	Valve2_Init();
	HeatingCore_Init();
	SoilHumSensor_Init();
	DS18B20_Init();
	PhSensor_Init();
	SGP30_Init();
	Motor_Init();
	Dth11_Init();
	LED_Init();
	_28BYJ48_Init();
}

void SendPropertyToMqtt(void)
{	
//	Dth11_Read();
//	Multiple_read_BH1750();
//	SGP30_Read();
//	SoilHumValue_Get();
//	DS18B20_ReadT();
//	PhSensorValue_Get();
	char buffer1[300];
	char buffer2[300];
	char *cmd = "AT+MQTTPUB=0,\"dev/stm\",\"%s\",0,0\r\n";
	char *jsonString = "{\\\"airT\\\":%.1f\\,\\\"airH\\\":%.1f\\,\\\"light\\\":%d\\,\\\"soilH\\\":%.1f\\,\\\"soilT\\\":%.1f\\,\\\"ph\\\":%.1f\\,\\\"co2\\\":%d\\,\\\"phDevErr\\\":%d\\,\\\"ATHDevErr\\\":%d\\,\\\"soilTDevErr\\\":%d\\,\\\"soilHDevErr\\\":%d\\,\\\"co2DevErr\\\":%d\\,\\\"lightDevErr\\\":%d}";
	sprintf(buffer1 , jsonString, air_temperature, air_humidity, light, 
	soil_moisture, soil_temperature, ph_value, CO2Data, PhSensor_WorkStatus, 
	DTH11_WorkStatus, DS18B20_WorkStatus, soilHumSensor_WorkStatus, SGP30_WorkStatus, BH1750_WorkStatus);
	sprintf(buffer2, cmd, buffer1);
	if(strlen(buffer2) >=256)
	{
		OLED_ShowString(1,1,"ATcmd is too long");
		Delay_s(60);
	}

	Serial_SendString(buffer2);
	
	Serial_RxFromEspBuf_Clear();
}


void ControlDevice(void)
{
	char recv_cmd[200] = {0};  
	char json[200] = {0};
	if(Serial_GetRxFlag() == 1)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"Start");
		Delay_ms(500);
		for(uint16_t i = 0,j = 0;i < 500 ;i ++)
		{
			if(Serial_RxFromEspBuf[i] == '}')
			{
				recv_cmd[j ++] = Serial_RxFromEspBuf[i];
				recv_cmd[j] = '\0';
				for(char *p = recv_cmd;*p != '\0';p ++)
				{
					if(*p == '{')
					{
						strcpy(json, p);
						break;
					}
				}
				if(strstr(recv_cmd,"prop/set") != NULL)
				{
					json_error_t error;
					json_t *root;
					const char *key;
					json_t *value;
					root = json_loads(json, 0, &error);
					void *iter = json_object_iter(root);
					if(json_is_object(root))
					{
						while(iter)
						{
							key = json_object_iter_key(iter);
							value = json_object_iter_value(iter);

							if(strcmp(key, "airT_Max") == 0) Store_Data1[0].value = json_real_value(value);
							else if(strcmp(key, "airT_Min") == 0) Store_Data1[1].value = json_real_value(value);
							else if(strcmp(key, "airH_Max") == 0) Store_Data1[2].value = json_real_value(value);
							else if(strcmp(key, "airH_Min") == 0) Store_Data1[3].value = json_real_value(value);
							else if(strcmp(key, "ph_Max") == 0) Store_Data1[4].value = json_real_value(value);
							else if(strcmp(key, "ph_Min") == 0) Store_Data1[5].value = json_real_value(value);
							else if(strcmp(key, "soilT_Max") == 0) Store_Data1[6].value = json_real_value(value);
							else if(strcmp(key, "soilT_Min") == 0) Store_Data1[7].value = json_real_value(value);
							else if(strcmp(key, "soilH_Max") == 0) Store_Data1[8].value = json_real_value(value);
							else if(strcmp(key, "soilH_Min") == 0) Store_Data1[9].value = json_real_value(value);
							else if(strcmp(key, "co2_Max") == 0) Store_Data2[0] = json_integer_value(value);
							else if(strcmp(key, "co2_Min") == 0) Store_Data2[1] = json_real_value(value);
							else if(strcmp(key, "light_Max") == 0) Store_Data2[2] = json_integer_value(value);
							else if(strcmp(key, "light_Min") == 0) Store_Data2[3] = json_integer_value(value);
							else if(strcmp(key, "Led_StartTime") == 0) Store_Data2[4] = json_integer_value(value);
							else if(strcmp(key, "Led_EndTime") == 0) Store_Data2[5] = json_integer_value(value);
							else if(strcmp(key, "Fan_StartTime") == 0) Store_Data2[6] = json_integer_value(value);
							else if(strcmp(key, "Fan_EndTime") == 0) Store_Data2[7] = json_integer_value(value);
							else if(strcmp(key, "Pump_StartTime") == 0) Store_Data2[8] = json_integer_value(value);
							else if(strcmp(key, "Pump_EndTime") == 0) Store_Data2[9] = json_integer_value(value);
							else if(strcmp(key, "Curtain_StartTime") == 0) Store_Data2[10] = json_integer_value(value);
							else if(strcmp(key, "Curtain_EndTime") == 0) Store_Data2[11] = json_integer_value(value);
							else if(strcmp(key, "Led_Mode") == 0) Store_Data2[12] = json_integer_value(value);
							else if(strcmp(key, "Fan_Mode") == 0) Store_Data2[13] = json_integer_value(value);
							else if(strcmp(key, "Valve1_Mode") == 0) Store_Data2[14] = json_integer_value(value);
							else if(strcmp(key, "Valve2_Mode") == 0) Store_Data2[15] = json_integer_value(value);
							else if(strcmp(key, "Pump_Mode") == 0) Store_Data2[16] = json_integer_value(value);
							else if(strcmp(key, "HeatingCore_Mode") == 0) Store_Data2[17] = json_integer_value(value);
							else if(strcmp(key, "Curtain_Mode") == 0) Store_Data2[18] = json_integer_value(value);

							iter = json_object_iter_next(root, iter);
						}
						Store_Save();
					}
					json_decref(root);
				}
				else if(strstr(recv_cmd,"service/call") != NULL)
				{
					json_error_t error;
					json_t *root;
					char *identifier;
					uint8_t Switch;
					
					root = json_loads((const char*)json, 0, &error); 
					if(json_is_object(root))   
					{
						identifier = (char *)json_string_value(json_object_get(root, "identifier"));
						Switch = json_integer_value(json_object_get(root, "switch"));
					}
					json_decref(root);
					if(strcmp(identifier,"lightControl") == 0 && Store_Data2[12] == 0) {LED_Control(Switch);}
					else if(strcmp(identifier,"fanControl") == 0 && Store_Data2[13] == 0) {Motor_Control(Switch);}
					else if(strcmp(identifier,"valve1Control") == 0 && Store_Data2[14] == 0) {Valve1_Control(Switch);}
					else if(strcmp(identifier,"valve2Control") == 0 && Store_Data2[15] == 0) {Valve2_Control(Switch);}
					else if(strcmp(identifier,"pumpControl") == 0 && Store_Data2[16] == 0) {Pump_Control(Switch);}
					else if(strcmp(identifier,"hcControl") == 0 && Store_Data2[17] == 0) {HeatingCore_Control(Switch);}
					else if(strcmp(identifier,"smotorControl") == 0 && Store_Data2[18] == 0) {SteeperMotor_Control(Switch);}
				}

				Serial_OledShowString(json);
				Delay_s(5);
				memset(json, 0, sizeof json);
				memset(recv_cmd, 0, sizeof recv_cmd);
				j = 0;
			}
			else if(Serial_RxFromEspBuf[i] == '\n')
			{
				if(Serial_RxFromEspBuf[i + 1] == '\0')
				{
					OLED_Clear();
					OLED_ShowString(1,1,"complete");
					Delay_ms(500);
					Serial_RxFromEspBuf_Clear();
					Serial_GetRxFlag();
					break;
				}
				else
				{
					OLED_Clear();
					OLED_ShowString(1,1,"continue");
					Delay_ms(500);
					continue;
				}
			}
			else recv_cmd[j ++] = Serial_RxFromEspBuf[i];
		}
	}
	MyRTC_ReadTime();
	switch (Store_Data2[12])//LED
	{
		case 1:
		{
			if(light < Store_Data2[3]) LED_Control(1);
			else if(light > Store_Data2[2]) LED_Control(0);
			break;	
		}
		case 2:
		{
			uint16_t current_time;
			current_time = MyRTC_Time[3] * 60 + MyRTC_Time[4];
			if((current_time - Store_Data2[4]) >= 0 &&  (current_time - Store_Data2[5]) <= 0) LED_Control(1);
			else LED_Control(0);
			break;
		}
	}
	switch (Store_Data2[13])//FAN
	{
		case 1:
		{
			if(CO2Data > Store_Data2[0]) Motor_Control(1);
			else if(CO2Data > Store_Data2[1]) Motor_Control(0);
			break;	
		}
		case 2:
		{
			uint16_t current_time = MyRTC_Time[3] * 60 + MyRTC_Time[4];
			if((current_time - Store_Data2[6]) >=0 &&  (current_time - Store_Data2[7]) <=0) Motor_Control(1);
			else Motor_Control(0);
			break;
		}
	}
	switch (Store_Data2[14])//Valve1
	{
		case 1:
		{
			if(ph_value < Store_Data1[5].value) Valve1_Control(1);
			else Valve1_Control(0);
			break;
		}	
	}
	switch (Store_Data2[15])//Valve2
	{
		case 1:
		{
			if(ph_value > Store_Data1[4].value) Valve1_Control(1);
			else Valve1_Control(0);
			break;
		}	
	}
	switch (Store_Data2[16])//Curtain
	{
		case 1:
		{
			if(light < Store_Data2[3]) LED_Control(1);
			else if(light > Store_Data2[2]) LED_Control(0);
			break;
		}	
		case 2:
		{
			uint16_t current_time = MyRTC_Time[3] * 60 + MyRTC_Time[4];
			if((current_time - Store_Data2[10]) >=0 &&  (current_time - Store_Data2[11]) <=0) SteeperMotor_Control(1);
			else SteeperMotor_Control(0);
			break;
		}
	}
	switch (Store_Data2[17])//Pump
	{
		case 1:
		{
			if(soil_moisture < Store_Data1[9].value) Pump_Control(1);
			else if(soil_moisture > Store_Data1[8].value) Pump_Control(0);
			break;	
		}
		case 2:
		{
			uint16_t current_time = MyRTC_Time[3] * 60 + MyRTC_Time[4];
			if((current_time - Store_Data2[8]) >=0 &&  (current_time - Store_Data2[9]) <=0) Pump_Control(1);
			else Pump_Control(0);
			break;
		}
	}
	switch (Store_Data2[18])//HeatingCore
	{
		case 1:
		{
			if(soil_temperature < Store_Data1[7].value) HeatingCore_Control(1);
			else if(soil_temperature > Store_Data1[6].value) HeatingCore_Control(0);
			break;	
		}
	}
}
