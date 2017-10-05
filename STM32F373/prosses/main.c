
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

#include "pid.h" 

uint8_t Bsp_Int_Ok=0;
extern uint8_t Count_1ms,Count_2ms,Count_4ms;
 

extern u8 RS485_RX_BUF[64]; 
extern u8 RS485_TX_BUF[64];
void Bsp_Int(void ){
	
	RCC_ClocksTypeDef RCC_Clocks;
 
	delay_init();
	
	LED_Init();		  
	
	RS485_Init(115200);
	RS485_PrintString("1:\tRS485_Init\n");
	KEY_Init();
	RS485_PrintString("2:\tKEY_Init\n");
	TIM4_Init();
	RS485_PrintString("3:\tTIM4_Init\n");
	PWM_Init();
	RS485_PrintString("4:\tPWM_Init\n");
	PID_Init() ;
	RS485_PrintString("5:\tPID_Init\n");
	SDADC1_Config();
	Calculate_FilteringCoefficient(0.005f, 10.f);//sampel time,unit second,F_cutoff
	RS485_PrintString("6:\tSDADC1_Config\n");
 
	
	RCC_GetClocksFreq(&RCC_Clocks);
	sprintf((char *)RS485_TX_BUF,"\n***ClocksFreq***[%d]\n",RCC_Clocks.HCLK_Frequency);
	RS485_PrintString(RS485_TX_BUF);
	LED_ON_OFF();
	Bsp_Int_Ok = 1;
}
int main(void)
{ 
	Bsp_Int();	
	 
	while(1)
	{	
		
		if(Count_1ms>=5)//200Hz
		{	
			Count_1ms = 0;		
			Task_1000HZ();
		}
		if(Count_2ms>=10)//100Hz
		{
			Count_2ms = 0;
			Task_500HZ();
		}
		if(Count_4ms>=20)//50Hz
		{
			Count_4ms = 0;
			Task_250HZ();
			
		}
	}
}
