#include "stm32f10x.h"                  // Device header
#include "AD.h"

void PH_AD_Init(void)
{
	RCC_APB2PeriphClockCmd(PH_ADC_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(PH_GPIO_CLK, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = PH_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PH_GPIO_PORT, &GPIO_InitStructure);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(PH_ADC_ADCx, &ADC_InitStructure);
	
	ADC_Cmd(PH_ADC_ADCx, ENABLE);

	ADC_ResetCalibration(PH_ADC_ADCx);
	while (ADC_GetResetCalibrationStatus(PH_ADC_ADCx) == SET);
	ADC_StartCalibration(PH_ADC_ADCx);
	while (ADC_GetCalibrationStatus(PH_ADC_ADCx) == SET);
}

uint16_t PH_AD_GetValue(void)
{
	static uint16_t AD_VALUE_ARRAY[10],i;
	uint16_t AD_VALUE,sum = 0,j;
	ADC_RegularChannelConfig(PH_ADC_ADCx, PH_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(PH_ADC_ADCx, ENABLE);
	while (ADC_GetFlagStatus(PH_ADC_ADCx, ADC_FLAG_EOC) == RESET);
	AD_VALUE_ARRAY[i] = ADC_GetConversionValue(PH_ADC_ADCx);
	i ++;
	if(i >= 10) i = 0;
	for(j = 0;j < 10;j ++) sum+=AD_VALUE_ARRAY[j];
	AD_VALUE = sum / 10;
	return AD_VALUE;
}

void SOIL_AD_Init(void)
{
	RCC_APB2PeriphClockCmd(SOIL_ADC_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(SOIL_GPIO_CLK, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = SOIL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SOIL_GPIO_PORT, &GPIO_InitStructure);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(SOIL_ADC_ADCx, &ADC_InitStructure);
	
	ADC_Cmd(SOIL_ADC_ADCx, ENABLE);

	ADC_ResetCalibration(SOIL_ADC_ADCx);
	while (ADC_GetResetCalibrationStatus(SOIL_ADC_ADCx) == SET);
	ADC_StartCalibration(SOIL_ADC_ADCx);
	while (ADC_GetCalibrationStatus(SOIL_ADC_ADCx) == SET);
}

uint16_t SOIL_AD_GetValue(void)
{
	static uint16_t AD_VALUE_ARRAY[10],i;
	uint16_t AD_VALUE,sum = 0,j;
	ADC_RegularChannelConfig(SOIL_ADC_ADCx, SOIL_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(SOIL_ADC_ADCx, ENABLE);
	while (ADC_GetFlagStatus(SOIL_ADC_ADCx, ADC_FLAG_EOC) == RESET);
	AD_VALUE_ARRAY[i] = ADC_GetConversionValue(SOIL_ADC_ADCx);
	i ++;
	if(i >= 10) i = 0;
	for(j = 0;j < 10;j ++) sum+=AD_VALUE_ARRAY[j];
	AD_VALUE = sum / 10;
	return AD_VALUE;
}
