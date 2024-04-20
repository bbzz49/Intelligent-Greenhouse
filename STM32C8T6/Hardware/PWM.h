#ifndef __PWM_H
#define __PWM_H

#define PWMA_GPIO_CLK 			RCC_APB2Periph_GPIOA
#define PWMA_GPIO_PORT 			GPIOA
#define PWMA_GPIO_PIN			GPIO_Pin_3

#define PWMB_GPIO_CLK 			RCC_APB2Periph_GPIOA
#define PWMB_GPIO_PORT 			GPIOA
#define PWMB_GPIO_PIN			GPIO_Pin_2

void PWM_Init(void);
void PWM_SetCompare4(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);

#endif
