#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "AD.h"
#include "jansson.h"
#include "Serial.h"

float soil_moisture;
uint8_t soilHumSensor_ErrorCount;
uint8_t soilHumSensor_WorkStatus;

void SoilHumSensor_Init(void)
{
	SOIL_AD_Init();
}

void SoilHumValue_Get(void)
{
	uint16_t ADValue;
	float SoilHum_value;
	ADValue = SOIL_AD_GetValue();
	SoilHum_value = (float)(4092-ADValue) / 4092 * 100;
	if(SoilHum_value >=100) SoilHum_value = 100;
	soil_moisture =SoilHum_value;
}

void SoilHumSensor_SendDataToTcp(void)
{
	float SoilHum_value;
	SoilHum_value = soil_moisture;
	SoilHum_value = (uint16_t)(SoilHum_value * 10) / 10.0;
	json_t *root;
	char *jsonString;
	
	root = json_pack("{sf}","soil_moisture",SoilHum_value);
	jsonString = json_dumps(root, JSON_COMPACT);
	Serial_SendString(jsonString);
	json_delete(root);
	free(jsonString);
}
