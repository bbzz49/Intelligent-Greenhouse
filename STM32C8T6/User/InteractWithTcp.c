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

void Module_Init(void)
{
	OLED_Init();
	Esp8266_Init();
	Esp8266_WifiAndTcp_Connect();
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

void SendEventToTcp(char *identifier,uint8_t name)
{
	json_t *root;
	char *jsonString;
	char lengthString[4];
	char cmd[20];
	uint8_t length;
	root = json_pack("{sssi}",
		"identifier",identifier,
		"name",name
				);
	jsonString = json_dumps(root, JSON_COMPACT);

	length = strlen(jsonString);
    sprintf(lengthString, "%d", length);
	strcpy(cmd, "AT+CIPSEND=");
	strcat(cmd, lengthString);
	OLED_Clear();
	OLED_ShowNum(1, 1, length, 3);
	if(Esp8266_WriteCmd(cmd, "OK", "OK", 5))
	{
		Serial_SendString(jsonString);
		Serial_RxFromEspBuf_Clear();
	}
	else ReconnectToTcp();

	json_decref(root);
	free(jsonString);
	Delay_s(1);
}

void SendPropertyToTcp(void)
{	
	Dth11_Read();
	Multiple_read_BH1750();
	SGP30_Read();
	SoilHumValue_Get();
	DS18B20_ReadT();
	PhSensorValue_Get();
	json_t *root;
	char *jsonString;
	char lengthString[4];
	char cmd[20];
	uint16_t length;
	root = json_pack("{sfsfsisfsfsfsisisisisisisi}",
				"airTemperature",air_temperature,
				"airHumidity",air_humidity,
				"lightIntensity",light,
				"soilMoisture",soil_moisture,
				"soilTemperature",soil_temperature,
				"phValue",ph_value,
				"co2Concentration",CO2Data,
				"phSensorError",PhSensor_WorkStatus,
				"airTempAndHumSensorError",DTH11_WorkStatus,
				"soilTempSensorError",DS18B20_WorkStatus,
				"soilHumSensorError",soilHumSensor_WorkStatus,
				"co2SensorError",SGP30_WorkStatus,
				"lightSensorError",BH1750_WorkStatus
				);
	
	jsonString = json_dumps(root, JSON_COMPACT);
	length = strlen(jsonString);
    sprintf(lengthString, "%d", length);
	strcpy(cmd, "AT+CIPSEND=");
	strcat(cmd, lengthString);
	OLED_Clear();
	OLED_ShowNum(1, 1, length, 3);
	
	if(Esp8266_WriteCmd(cmd, "OK", "OK", 5))
	{
		Serial_SendString(jsonString);
		Serial_RxFromEspBuf_Clear();
	}
	else ReconnectToTcp();
	json_decref(root);
	free(jsonString);
}

void HandleCommandFromTcp(void)
{
	if(Serial_GetRxFlag() == 1)//检查蹿接收缓冲区是否已经收到一个完整的json字符串命令
	{	
		uint16_t i,j;
		char json[100] = {0};  //单个服务调用命令长度不超过100
		//收到的json字符串命令的格式，想要在""里面表示"，需要使用\"
		//char json_str[] = "{\"identifier\":\"light_control\", \"switch\": 1}"; 
		OLED_Clear();
		OLED_ShowString(1,1,"Start");
//		Delay_s(2);
		for(i = 0,j = 0;i < 500;i ++)//遍历接收缓冲区
		{
			if(Serial_RxFromTcpBuf[i] == '\n')//'\n'为每条json字符串命令之间的间隔
			{
//				OLED_Clear();
//				OLED_ShowString(1,1,"recognize a n");
//				Delay_s(2);
				if(Serial_RxFromTcpBuf[i+1] == '\0')//如果在代表json字符串间隔的'\n'后的是'\0',则代表后面没有数据了
				{
//					OLED_Clear();
//					OLED_ShowString(1,1,"complete!");
//					Delay_s(2);
					Serial_RxFromTcpBuf_Clear();//刷新整个接收缓存区
					break;//跳出循环，本次处理服务器下发命令流程结束
				}
				else
				{
//					OLED_Clear();
//					OLED_ShowString(1,1,"Continue");
//					Delay_s(2);
					continue;//如果在代表json字符串间隔的'\n'后的不是\0',则代表后面还有数据
				}
			}
			else if(Serial_RxFromTcpBuf[i] == '}')//}为json字符串结束的标志
			{
				//解析json字符串的内容，执行相应操作
				json[j] = Serial_RxFromTcpBuf[i];
//				OLED_Clear();
//				Serial_OledShowString(json);
//				Delay_s(2);
				//解析json，未测试
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
				else
				{
					//printf("root format error:%d-%s\r\n", error.line, error.text);
					OLED_ShowString(1,1,"jiexiERROR");
				}
				json_decref(root);
				OLED_Clear();
				OLED_ShowString(1,1,identifier);
				OLED_ShowNum(2,1,Switch,1);
//				Delay_s(2);
				memset(json, 0, sizeof json);//刷新接收单个json字符串命令的缓存区
				j = 0;
				if(strcmp(identifier,"lightControl") == 0) {LED_Control(Switch);}
				else if(strcmp(identifier,"fanControl") == 0) {Motor_Control(Switch);}
				else if(strcmp(identifier,"pumpControl") == 0) {Pump_Control(Switch);}
				else if(strcmp(identifier,"valve1Control") == 0) {Valve1_Control(Switch);}
				else if(strcmp(identifier,"valve2Control") == 0) {Valve2_Control(Switch);}
				else if(strcmp(identifier,"hcControl") == 0) {HeatingCore_Control(Switch);}
				else if(strcmp(identifier,"smotorControl") == 0) {SteeperMotor_Control(Switch);}
				
			}
			else if(Serial_RxFromTcpBuf[i] == '\0')//说明接收缓存区有未接受完整的json字符串
			{
				OLED_ShowString(1,1,"cmd not complete");
				Delay_s(2);
				uint8_t Count = 5;
				do
				{
					Count --;
					Delay_us(100);//等待串口接收中断刷新接收缓存区
				}
				while(Serial_RxFromTcpBuf[i] != '\0' || Count == 0);//接收缓存区已被刷新
				
				if(Count == 0)
				{
					OLED_ShowString(1,1,"ERROR");
					break;
				}
				i --; continue;//重新开始本次循环
			}
			else
			{	
				json[j] = Serial_RxFromTcpBuf[i];
				j ++;
			}
		}
	}
}

