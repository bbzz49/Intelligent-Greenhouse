#ifndef __VALVE2_H__
#define __VALVE2_H__

extern uint8_t valve2_status;
extern uint8_t valve2_workmode;
void Valve2_Init(void);

void Valve2_Control(uint8_t Switch);

#endif
