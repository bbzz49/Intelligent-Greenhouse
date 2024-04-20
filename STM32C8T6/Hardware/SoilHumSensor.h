#ifndef __SOIL_HUM_H__
#define __SOIL_HUM_H__

extern float soil_moisture;
extern uint8_t soilHumSensor_WorkStatus;

void SoilHumSensor_Init(void);
float SoilHumValue_Get(void);
void SoilHumSensor_SendDataToTcp(void);

#endif
