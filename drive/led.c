#include "led.h"
#include "delay.h"
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
  GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_Init(LED_PORT, &GPIO_InitStruct);
	
	
	LED_Open();
	 
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
	LED_Open();

	delay_ms(100);
}
