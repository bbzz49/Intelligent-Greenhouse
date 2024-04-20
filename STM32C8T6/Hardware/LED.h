#ifndef __LED_H
#define __LED_H
#include "sys.h"
extern uint8_t light_status;
extern uint8_t light_workmode;
#define  LED_IN1   PAout(5)
#define  LED_IN2   PAout(4)

#define LED_IN1_GPIO_CLK			RCC_APB2Periph_GPIOA
#define LED_IN1_GPIO_PIN			GPIO_Pin_5
#define LED_IN1_GPIO_PORT			GPIOA

#define LED_IN2_GPIO_CLK			RCC_APB2Periph_GPIOA
#define LED_IN2_GPIO_PIN			GPIO_Pin_4
#define LED_IN2_GPIO_PORT			GPIOA


void LED_Init(void);
void LED_SetStrength(int8_t Strength);
void LED_Control(uint8_t Switch);

#endif
