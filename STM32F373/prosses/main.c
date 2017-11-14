
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "led.h"
#include "sdadc.h"
#include "rs485.h" 
#include "pwm.h" 
#include "tasks.h" 
#include "timebase.h" 
#include "eeprom.h" 
#include <stdio.h>
#include "protocol.h"
#include "pid.h" 

uint8_t Bsp_Int_Ok=0;
extern uint8_t Count_1ms,Count_2ms,Count_5ms,Count_10ms,Count_100ms;
 

extern u8 RS485_RX_BUF[64]; 
extern u8 RS485_TX_BUF[64];
void Bsp_Int(void ){
	
	RCC_ClocksTypeDef RCC_Clocks;
 
	delay_init();
	
	LED_Init();		  
	LED_ON_OFF();
	RS485_Init(194000);
	RS485_PrintString("1:\tRS485_Init\n");
	KEY_Init();
	RS485_PrintString("2:\tKEY_Init\n");
	TIM4_Init();
	RS485_PrintString("3:\tTIM4_Init\n");
	PWM_Init();
	RS485_PrintString("4:\tPWM_Init\n");
	PID_Init() ;
	RS485_PrintString("5:\tPID_Init\n");
	ADC1_Init();
	Calculate_FilteringCoefficient(50);//sampel time,unit second,F_cutoff
	RS485_PrintString("6:\tADC_Init\n");
 
	
	RCC_GetClocksFreq(&RCC_Clocks);
	sprintf((char *)RS485_TX_BUF,"\n***ClocksFreq***[%d]\n",RCC_Clocks.HCLK_Frequency);
	RS485_PrintString(RS485_TX_BUF);
	
	Bsp_Int_Ok = 1;
}
int main(void)
{ 

	Bsp_Int();	
	 
	while(1)
	{					
		if(Count_1ms>=1)//1000Hz
		{			
			Count_1ms = 0;		
			Task_1000HZ();			
		}
		if(Count_2ms>=getFeedBackTime())//500Hz
		{	
			
			Count_2ms = 0;		
			Task_500HZ();
		}
		
		if(Count_5ms>=5)//200Hz
		{
			Count_5ms = 0;
			Task_200HZ();
		}
		if(Count_10ms>=10)//100Hz
		{
			Count_10ms = 0;
			Task_100HZ();		
		}
		if(Count_100ms>=100)//10Hz
		{
			Count_100ms = 0;
			Task_10HZ();		
		}
		 
	}
}
