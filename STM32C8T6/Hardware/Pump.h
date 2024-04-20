#ifndef __PUMP_H__
#define __PUMP_H__

extern uint8_t pump_status;
extern uint8_t pump_workmode;

void Pump_Init(void);

void Pump_Control(uint8_t Switch);

#endif
