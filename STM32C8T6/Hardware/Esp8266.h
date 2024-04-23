#ifndef __ESP8266_H__
#define __ESP8266_H__

void Esp8266_Init(void);
uint8_t Esp8266_detect(void);
uint8_t Esp8266_ModeNetSet(void);
uint8_t Esp8266_Mqtt_Connect(void);	
uint8_t Esp8266_WriteCmd(char *Command, char *Response1, char *Response2, uint8_t Count, uint8_t Clean_buff);

#endif


