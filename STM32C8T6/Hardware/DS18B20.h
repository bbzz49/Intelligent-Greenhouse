#ifndef __DS18B20_H__
#define __DS18B20_H__

extern float soil_temperature;
extern uint8_t DS18B20_WorkStatus;
//DS18B20指令
#define DS18B20_SKIP_ROM			0xCC
#define DS18B20_CONVERT_T			0x44
#define DS18B20_READ_SCRATCHPAD 	0xBE


void DS18B20_ConvertT(void);
uint8_t DS18B20_ReadT(void);
void DS18B20_Init(void);
uint8_t DS18B20_Detect(void);
void DS18B20_SendDataToTcp(void);

#endif
