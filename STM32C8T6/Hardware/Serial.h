#ifndef __SERIAL_H__
#define __SERIAL_H__

extern char Serial_RxFromEspBuf[500];
extern char Serial_RxFromTcpBuf[500];
#define Serial_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define Serial_TX_GPIO_PIN			GPIO_Pin_9
#define Serial_TX_GPIO_PORT			GPIOA

#define Serial_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define Serial_RX_GPIO_PIN			GPIO_Pin_10
#define Serial_RX_GPIO_PORT			GPIOA	

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length); 
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number,uint8_t Length);
uint8_t Serial_GetRxFlag(void);

void Serial_SendPacket(void);
void Serial_OledShowString(char *String);
void Serial_RxFromEspBuf_Clear(void);
void Serial_RxFromTcpBuf_Clear(void);


#endif
