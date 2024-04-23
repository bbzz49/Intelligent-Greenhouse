#ifndef __STORE_H
#define __STORE_H

#include <stdint.h>
typedef  union
{
    float value;
		uint32_t value1;
}Flash;

extern uint16_t Store_Data2[];
void Store_Init(void);
void Store_Save(void);
//void Store_Clear(void);

#endif