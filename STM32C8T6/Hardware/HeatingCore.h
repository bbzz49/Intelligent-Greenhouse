#ifndef __HEATINGCORE_H__
#define __HEATINGCORE_H__

extern uint8_t hc_status;
extern uint8_t hc_workmode;

void HeatingCore_Init(void);

void HeatingCore_Control(uint8_t Switch);

#endif
