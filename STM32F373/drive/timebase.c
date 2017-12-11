/**
  ******************************************************************************
  * @file    bsp_TimBase.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   TIM2 1ms 定时应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "timebase.h" 

uint16_t Count_1ms,Count_2ms,Count_5ms,Count_10ms,Count_100ms;
uint32_t  Timer4_Count;//记录Timer3中断次数
extern uint8_t Bsp_Int_Ok;
void TIM4_Init(void){
	/* TIM4 定时配置 */	
  TIM4_Configuration(71);
	
	/* 实战定时器的中断优先级 */
	TIM4_NVIC_Configuration();

	/* TIM4 重新开时钟，开始计时 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
}
void TIM4_Set_Prescaler(unsigned int Prescaler){
	TIM4_Configuration(Prescaler);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
}
/// TIM4中断优先级配置
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //2bit 抢占，2bit响应 抢占优先，值小则优先 													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
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
	if(TIM4->SR & TIM_IT_Update)
	{     
		TIM4->SR = ~TIM_FLAG_Update;//清除中断标志
		
		if( Bsp_Int_Ok == 0 )	return;//硬件未初始化完成，则返回
		Timer4_Count++;
		/*if(Timer4_Count%2 ==0)
			GPIO_SetBits(GPIOA, GPIO_Pin_15);
		else
			GPIO_ResetBits(GPIOA, GPIO_Pin_15);*/
		Count_1ms++;
		Count_2ms++;
		Count_5ms++;
		Count_10ms++;
		Count_100ms++;
	}
}
/*********************************************END OF FILE**********************/
