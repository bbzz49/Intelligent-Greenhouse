#ifndef _28BYJ48_H__
#define _28BYJ48_H__
#include <stdio.h>
#include <string.h>

#define  MOTOR_CLK               (RCC_APB2Periph_GPIOB)
#define  MOTOR_PORT              (GPIOB)
#define  MOTOR_IN1_PIN           (GPIO_Pin_5)
#define  MOTOR_IN2_PIN           (GPIO_Pin_6)
#define  MOTOR_IN3_PIN           (GPIO_Pin_7)
#define  MOTOR_IN4_PIN           (GPIO_Pin_8)


void _28BYJ48_Init(void);
void _28BYJ48_Forward_Ration(void);  
void _28BYJ48_Reverse_Rotation(void);  
void _28BYJ48Stop(void); 
void SteeperMotor_Control(uint8_t Switch);


#endif


