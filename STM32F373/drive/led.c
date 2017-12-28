#include "led.h"
#include "delay.h"
void LED_Init(void)
{	
	LED_Close();
}

void LED_Open(void)
{
	 GPIO_SetBits(LED_PORT, LED_PIN );
}

void LED_Close(void)
{		
		GPIO_ResetBits(LED_PORT, LED_PIN );
}

 
/******************************************************************************
函数原型：	void LED_ON_OFF(void)
功    能：	LED开机闪烁
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LED_Open();

		delay_ms(200);
		LED_Close();

		delay_ms(200);	
	}
	for(i=0;i<3;i++)
	{
		LED_Open();

		delay_ms(100);
		LED_Close();

		delay_ms(100);	
	}	
	LED_Close();

	delay_ms(100);
}
