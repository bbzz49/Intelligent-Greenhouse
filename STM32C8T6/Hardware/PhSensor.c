#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Serial.h"
#include "String.h"
#include "jansson.h"
#include <math.h>
float ph_value;
uint8_t PhSensor_ErrorCount;
uint8_t PhSensor_WorkStatus;
void PhSensor_Init(void)
{
	PH_AD_Init();
}

void PhSensorValue_Get(void)
{
	uint16_t ADValue;
	float Voltage;
	ADValue = PH_AD_GetValue();
	Voltage = (float)ADValue / 4095 * 3.3;
	ph_value = -5.7541 *  Voltage +16.654;
	if(ph_value<=0.0)
	{
		ph_value=0.0;
		PhSensor_ErrorCount += 1;
		if(PhSensor_ErrorCount == 20) PhSensor_WorkStatus = 1;
	}
	if(ph_value>=14.0)
	{
		ph_value=14.0;
		PhSensor_ErrorCount += 1;
		if(PhSensor_ErrorCount == 20) PhSensor_WorkStatus = 1;
	}
	if(fabs(ph_value - 14) > 2.0 || fabs(ph_value - 0) > 2.0)
	{
		PhSensor_WorkStatus = 0;
		PhSensor_ErrorCount = 0;
	}
}

void PhSensor_SendDataToTcp(void)
{
	float ph_value;
	ph_value = (uint16_t)(ph_value * 10) / 10.0;
	json_t *root;
	char *jsonString;
	
	root = json_pack("{sf}","ph_value",ph_value);
	jsonString = json_dumps(root, JSON_COMPACT);
	Serial_SendString(jsonString);
	json_delete(root);
	free(jsonString);
}
