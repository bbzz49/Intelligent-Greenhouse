#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OneWire.h"
#include "string.h"
#include "Serial.h"
#include "DS18B20.h"
#include "jansson.h"
uint8_t DS18B20_ErrorCount;
uint8_t DS18B20_WorkStatus;
float soil_temperature;

void DS18B20_Init(void)
{
	OneWire_Init();
	DS18B20_ConvertT();
}



uint8_t DS18B20_Detect(void)
{
	uint8_t AckBit;
	OneWire_Init();
	OneWire_DQ_OUT = 1;
	OneWire_DQ_OUT = 0;
	Delay_us(500);
	OneWire_DQ_OUT = 1;
	Delay_us(70);
	OneWire_Set_In();
	AckBit = OneWire_DQ_IN;
	Delay_us(500);
	if(AckBit == 1)
	{
		DS18B20_ErrorCount += 1;
		if(DS18B20_ErrorCount == 3)
		{
			DS18B20_WorkStatus = 1;
			soil_temperature = 0;
		}
	}
	else{DS18B20_ErrorCount = 0;DS18B20_WorkStatus = 0;}

	return AckBit;//0为响应，1为未响应
}

void DS18B20_ConvertT(void)
{
	DS18B20_Detect();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_CONVERT_T);
}

uint8_t DS18B20_ReadT(void)
{
	uint8_t TLSB,TMSB;
	uint16_t Temp;
	float T;
	DS18B20_ConvertT();
	Delay_ms(200);
	if(DS18B20_Detect() == 1) return 0;
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
	TLSB=OneWire_ReceiveByte();
	TMSB=OneWire_ReceiveByte();
	Temp=(TMSB<<8)|TLSB;
	T=Temp/16.0;
	soil_temperature = T;
	soil_temperature = (uint16_t)(soil_temperature * 10) / 10.0;
	return 1;
}

void DS18B20_SendDataToTcp(void)
{
	float soil_temperature;
	json_t *root;
	char *jsonString;
	DS18B20_ConvertT();
	Delay_ms(700);
	soil_temperature = (uint16_t)(soil_temperature * 10) / 10.0; //保留一位小数
	root = json_pack("{sf}","soil_temperature",soil_temperature);
	jsonString = json_dumps(root, JSON_DECODE_ANY);
	Serial_SendString(jsonString);
	json_delete(root);
	free(jsonString);
}

