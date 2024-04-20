#ifndef __PHSENSOR_H__
#define __PHSENSOR_H__

extern float ph_value;
extern uint8_t PhSensor_WorkStatus;
void PhSensor_Init(void);
float PhSensorValue_Get(void);
void PhSensor_SendDataToTcp(void);


#endif
