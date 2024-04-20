#ifndef __IC_H
#define __IC_H

#define IC_GPIO_CLK			RCC_APB2Periph_GPIOA
#define IC_GPIO_PIN			GPIO_Pin_6
#define IC_GPIO_PORT		GPIOA

void IC_Init(void);
uint32_t IC_GetFreq(void);
uint32_t IC_GetDuty(void);

#endif
