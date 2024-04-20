#ifndef __BH1750_H
#define __BH1750_H

extern uint32_t light;
extern uint8_t BH1750_WorkStatus;

void Single_Write_BH1750(uint8_t REG_Address);

void BH1750_Init(void);

void Multiple_read_BH1750(void);

#endif
