#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__
#include "sys.h"

#define OneWire_DQ_OUT				PBout(12)
#define OneWire_DQ_IN				PBin(12)

#define OneWire_DQ_GPIO_CLK			RCC_APB2Periph_GPIOB
#define OneWire_DQ_GPIO_PIN			GPIO_Pin_12
#define OneWire_DQ_GPIO_PORT		GPIOB

void OneWire_Init(void);
void OneWire_Set_Out(void);
void OneWire_Set_In(void);
void OneWire_SendBit(uint8_t Bit);
uint8_t OneWire_ReceiveBit(void);
void OneWire_SendByte(uint8_t Byte);
uint8_t OneWire_ReceiveByte(void);


#endif
