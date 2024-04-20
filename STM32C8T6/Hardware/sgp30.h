#ifndef __SGP30_H
#define __SGP30_H

extern uint16_t CO2Data;
extern uint8_t SGP30_WorkStatus;

#define SGP30_ADDRESS 		0x58           

#define SGP30_read  0xb1  //SGP30的读地址
#define SGP30_write 0xb0  //SGP30的写地址

//初始化IIC接口
void SGP30_Init(void);
uint8_t SGP30_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
uint8_t SGP30_Write(u8 HCmd, u8 LCmd);
uint8_t SGP30_Read(void);



#endif

