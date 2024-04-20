#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "InteractWithTcp.h"
#include "Pump.h"
#include "LED.h"
#include "Valve1.h"
#include "Motor.h"
#include "LED.h"
#include "Pump.h"
#include "Valve1.h"
#include "HeatingCore.h"
#include "Serial.h"
#include "_28BYJ48.h"
int main(void)
{
	Module_Init();

	while (1)
	{
		SendPropertyToTcp();
		for(uint8_t i = 0;i < 8;i ++)
		{
			HandleCommandFromTcp();
			Delay_ms(500);
		}
	}

}

