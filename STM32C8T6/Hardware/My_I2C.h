#ifndef __MY_I2C_H__
#define __MY_I2C_H__

#define My_I2C_SCL_GPIO_CLK 		RCC_APB2Periph_GPIOB
#define My_I2C_SCL_GPIO_PIN 		GPIO_Pin_10
#define My_I2C_SCL_GPIO_PORT 		GPIOB

#define My_I2C_SDA_GPIO_CLK 		RCC_APB2Periph_GPIOB
#define My_I2C_SDA_GPIO_PIN 		GPIO_Pin_11
#define My_I2C_SDA_GPIO_PORT 		GPIOB

void My_I2C_Init(void);				//发送IIC开始信号

#endif
