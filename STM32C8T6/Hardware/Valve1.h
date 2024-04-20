#ifndef __VALVE1_H__
#define __VALVE1_H__

extern uint8_t valve1_status;
extern uint8_t valve1_workmode;
void Valve1_Init(void);

void Valve1_Control(uint8_t Switch);

#endif
