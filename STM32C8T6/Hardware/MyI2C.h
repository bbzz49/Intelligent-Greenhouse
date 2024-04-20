#ifndef __MYI2C_H
#define __MYI2C_H

#define My_I2C_SOFT_SCL_GPIO_CLK 		RCC_APB2Periph_GPIOA
#define My_I2C_SOFT_SCL_GPIO_PIN 		GPIO_Pin_12
#define My_I2C_SOFT_SCL_GPIO_PORT 		GPIOA

#define My_I2C_SOFT_SDA_GPIO_CLK 		RCC_APB2Periph_GPIOA
#define My_I2C_SOFT_SDA_GPIO_PIN 		GPIO_Pin_11
#define My_I2C_SOFT_SDA_GPIO_PORT 		GPIOA


void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
uint8_t MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

#endif
