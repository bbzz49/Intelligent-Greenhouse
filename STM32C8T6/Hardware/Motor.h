#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"
extern uint8_t fan_status;
extern uint8_t fan_workmode;
#define Motor_IN1 PAout(0)
#define Motor_IN2 PAout(1)

#define Motor_IN1_GPIO_CLK			RCC_APB2Periph_GPIOA
#define Motor_IN1_GPIO_PIN			GPIO_Pin_0
#define Motor_IN1_GPIO_PORT			GPIOA

#define Motor_IN2_GPIO_CLK			RCC_APB2Periph_GPIOA
#define Motor_IN2_GPIO_PIN			GPIO_Pin_1
#define Motor_IN2_GPIO_PORT			GPIOA

void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);
void Motor_Control(uint8_t Switch);

#endif
