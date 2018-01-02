/**
  ******************************************************************************
  * @file    led.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   led
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "led.h"
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
void LED_Toggle(void)
{
GPIO_WriteBit(LED_PORT, LED_PIN, (BitAction)((1-GPIO_ReadOutputDataBit(LED_PORT, LED_PIN))));
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
