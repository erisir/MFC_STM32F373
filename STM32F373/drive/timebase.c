/**
  ******************************************************************************
  * @file    timebase.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   TIM4 1ms  
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "timebase.h" 

uint16_t Counters[CounterNum];
extern uint8_t Bsp_Int_Ok;
 
void TIM4_Init(void){
	/* TIM4 定时配置 */	
  TIM4_Configuration(71);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
}
void TIM4_Set_Prescaler(unsigned int Prescaler){
	TIM4_Configuration(Prescaler);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
void TIM4_Configuration(unsigned int Prescaler)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* 设置TIM2CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    //TIM_DeInit(TIM4);
	
	/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
	  /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler= Prescaler;
	
		/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM4, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);		/*先关闭等待使用*/    
}

void TIM4_IRQHandler(void)//Timer4中断
{	
	static uint8_t i;
	if(TIM4->SR & TIM_IT_Update)
	{     
		TIM4->SR = ~TIM_FLAG_Update;//清除中断标志
		
		if( Bsp_Int_Ok == 0 )	return;//硬件未初始化完成，则返回
		for(i=0;i<CounterNum;i++){
			Counters[i]+=1;
		}	
	}
}
/*********************************************END OF FILE**********************/
