#ifndef __DTH11_H
#define __DTH11_H

extern float air_temperature,air_humidity;
extern uint8_t DTH11_WorkStatus;
void Dth11_Init(void);
void Dth11_Read(void);
void Dth11_OLED_Show(void);
void Dth11_SendDataToTcp(void);


#endif
