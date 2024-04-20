#include "stm32f10x.h"                  // Device header
#include "_28BYJ48.h"
#include "Delay.h"
//八相驱动步进电机，经历八个相位后，步进电机转子转一圈，步进电机转子转64圈，外部驱动部件转一圈（步进电机内部有1：64的减速比）
//const uint16_t CW[8] = {0x0002,0x0006,0x0004,0x000C,0x0008,0x0018,0x0010,0x0030};//正转 A AB B BC C CD D DA 
//const uint16_t CCW[8] = {0x0010,0x0018,0x0008,0x000C,0x0004,0x0006,0x0002,0x0030};//反转 D DC C CB B BA A AD
const uint16_t CW[8] = {0x0020,0x0060,0x0040,0x00C0,0x0080,0x0180,0x0100,0x0120};//正转 A AB B BC C CD D DA 
const uint16_t CCW[8] = {0x0100,0x0180,0x0080,0x00C0,0x0040,0x0060,0x0020,0x0120};//反转 D DC C CB B BA A AD

uint8_t Mode = 0; //表示步进电机是要正转还是反转
uint16_t Count = 0;//记录进入定时中断的次数来实现步进电机加速的过程来实现更高的恒定转速
uint8_t arr = 100 - 1;//arr重载计数的值，控制进入定时中断的频率，即更新步进电机相位的频率
uint8_t curtain_status;
void _28BYJ48_Init(void)//
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(MOTOR_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = MOTOR_IN1_PIN | MOTOR_IN2_PIN | MOTOR_IN3_PIN | MOTOR_IN4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 150 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 -1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief	控制步进电机顺时针转动一段时间，具体值看定时器中断函数中Count的阈值
  * @param	无
  * @retval	无
  */
void _28BYJ48_Forward_Ration(void)
{
	Count = 0;
	arr = 150 - 1;
	TIM_Cmd(TIM1, ENABLE);
	Mode = 1;
}
/**
  * @brief	控制步进电机逆时针转动一段时间，具体值看定时器中断函数中Count的阈值
  * @param	无
  * @retval	无
  */
void _28BYJ48_Reverse_Rotation(void)
{
	Count = 0;
	arr = 150 - 1;
	TIM_Cmd(TIM1, ENABLE);
	Mode = 2;
}
/**
  * @brief	关闭步进电机
  * @param  无
  * @retval 无
  */
void _28BYJ48Stop(void)
{
	TIM_Cmd(TIM1, DISABLE);
	Mode = 0;
	GPIO_Write(MOTOR_PORT,0x0000);
}
/**
  * @brief	控制步进电机正转或反转
  * @param	Switch	0表示步进电机反转，1表示开启步进电机正转
  * @retval	无
  */
void SteeperMotor_Control(uint8_t Switch)
{
	if(Switch == 0 && curtain_status == 1) {curtain_status = 0; _28BYJ48_Forward_Ration();}
	else if(Switch == 1 && curtain_status == 0) {curtain_status = 1; _28BYJ48_Reverse_Rotation();}
}



/**
  * @brief	每隔2ms更新步进电机的相位来驱动步进电机进行转动
  * @param	无
  * @retval	无
  */
void TIM1_UP_IRQHandler(void)
{
	static	uint8_t i = 0;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
		Count ++;
		//表示步进电机未达到恒定极速时，每隔一段时间将arr减一，加快步进电机转速
		if(Count >= 100 && arr >= 60)
		{
			Count = 0;
			TIM_SetAutoreload(TIM1, --arr);
		}
		if(Count >= 8000) _28BYJ48Stop();//Count的阈值具体取决于多长时间把棚子关上
        if(Mode == 1) GPIO_Write(MOTOR_PORT,CW[(i++%8)]);
		else if(Mode == 2) GPIO_Write(MOTOR_PORT,CCW[(i++%8)]);
		
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

    }
}

