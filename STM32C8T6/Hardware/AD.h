#ifndef __AD_H
#define __AD_H

#define PH_GPIO_CLK				RCC_APB2Periph_GPIOB
#define PH_GPIO_PIN				GPIO_Pin_0
#define PH_GPIO_PORT			GPIOB

#define PH_ADC_CLK				RCC_APB2Periph_ADC1
#define PH_ADC_CHANNEL			ADC_Channel_8
#define PH_ADC_ADCx				ADC1

#define SOIL_GPIO_CLK			RCC_APB2Periph_GPIOB
#define SOIL_GPIO_PIN			GPIO_Pin_1
#define SOIL_GPIO_PORT			GPIOB

#define SOIL_ADC_CLK			RCC_APB2Periph_ADC1
#define SOIL_ADC_CHANNEL		ADC_Channel_9
#define SOIL_ADC_ADCx			ADC1

void PH_AD_Init(void);
uint16_t PH_AD_GetValue(void);
void SOIL_AD_Init(void);
uint16_t SOIL_AD_GetValue(void);

#endif
