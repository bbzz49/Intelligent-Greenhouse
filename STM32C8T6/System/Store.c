#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"
#include "Store.h"
#define STORE_START_ADDRESS		0x08007C00		//存储的起始地址
#define STORE_COUNT				29					//存储数据的个数
//float airT_Max = Store_Data1[0].value, airT_Min = 10, airH_Max = 80, airH_Min = 50, 
//	ph_Max = 9, ph_Min = 7, soilT_Max = 25, soilT_Min = 10, soilH_Max = 80, soilH_Min = 50; 
//uint16_t co2_Max = 2000, co2_Min = 500, light_Max = 2000 , light_Min = 100;
//uint8_t Led_Mode, Fan_Mode, Valve1_Mode, Valve2_Mode, Pump_Mode, HeatingCore_Mode, Curtain_Mode;
//uint16_t Led_WorkingTime[] = {480, 1080}, Fan_WorkingTime[] = {480, 1080}, Pump_WorkingTime[] = {480, 1080}, Curtain_WorkingTime[] = {480, 1080};
//0：airT_Max 1: airT_Min 2: airH_Max 3: airH_Min 4: ph_Max 5: ph_Min 6: soilT_Max 7: soilT_Min 8: soilH_Max 9: soilH_Min
//0: co2_Max 1: co2_Min 2: light_Max 3: light_Min 4: Led_WorkingTime[0] 5: Led_WorkingTime[1] 6: Fan_WorkingTime[0]
//7: Fan_WorkingTime[1] 8:  Pump_WorkingTime[0] 9: Pump_WorkingTime[1] 10: Curtain_WorkingTime[0] 11:Curtain_WorkingTime[1]
//12: Led_Mode 13: 	Fan_Mode 14： Valve1_Mode 15： Valve2_Mode 16： Pump_Mode 17： HeatingCore_Mode 18：Curtain_Mode
Flash Store_Data1[10] = {{25.0}, {10.0}, {80.0}, {50.0}, {9.0}, {7.0}, {25.0}, {10.0}, {80.0}, {50.0}};
uint16_t Store_Data2[STORE_COUNT - 10] = {2000, 500, 2000, 100, 480, 1080, 480, 1080, 480, 1080, 480, 1080, 0, 0, 0, 0, 0, 0, 0};				//定义SRAM数组	
/**
  * 函    数：参数存储模块保存数据到闪存
  * 参    数：无
  * 返 回 值：无
  */
void Store_Save(void)
{
	MyFLASH_ErasePage(STORE_START_ADDRESS);				//擦除指定页
	for (uint16_t i = 0; i < STORE_COUNT; i ++)			//循环STORE_COUNT次，包括第一个标志位
	{
		if(i < 10) MyFLASH_ProgramWord(STORE_START_ADDRESS + i * 2, Store_Data1[i].value1);
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data2[i - 10]);	//将SRAM数组的数据备份保存到闪存
	}
}	

/**
  * 函    数：参数存储模块初始化
  * 参    数：无
  * 返 回 值：无
  */
void Store_Init(void)
{
	/*判断是不是第一次使用*/
	if (MyFLASH_ReadHalfWord(STORE_START_ADDRESS) == 0xFFFF)	//读取第一个半字的标志位，if成立，则执行第一次使用的初始化
	{
		Store_Save();
	}
	
	/*上电时，将闪存数据加载回SRAM数组，实现SRAM数组的掉电不丢失*/
	for (uint16_t i = 0; i < STORE_COUNT; i ++)					//循环STORE_COUNT次，包括第一个标志位
	{
		if(i < 10) Store_Data1[i].value1 = MyFLASH_ReadWord(STORE_START_ADDRESS + i * 2);		//将闪存的数据加载回SRAM数组
		Store_Data2[i-10] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);
	}
}


/**
  * 函    数：参数存储模块将所有有效数据清0
  * 参    数：无
  * 返 回 值：无
  */
//void Store_Clear(void)
//{
//	for (uint16_t i = 1; i < STORE_COUNT; i ++)			//循环STORE_COUNT次，除了第一个标志位
//	{
//		Store_Data[i] = 0x0000;							//SRAM数组有效数据清0
//	}
//	Store_Save();										//保存数据到闪存
//}