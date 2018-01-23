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
	/* TIM4 ��ʱ���� */	
  TIM4_Configuration(71);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
}
void TIM4_Set_Prescaler(unsigned int Prescaler){
	TIM4_Configuration(Prescaler);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
}
/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
void TIM4_Configuration(unsigned int Prescaler)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* ����TIM2CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    //TIM_DeInit(TIM4);
	
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	  /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler= Prescaler;
	
		/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM4, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
}

void TIM4_IRQHandler(void)//Timer4�ж�
{	
	static uint8_t i;
	if(TIM4->SR & TIM_IT_Update)
	{     
		TIM4->SR = ~TIM_FLAG_Update;//����жϱ�־
		
		if( Bsp_Int_Ok == 0 )	return;//Ӳ��δ��ʼ����ɣ��򷵻�
		for(i=0;i<CounterNum;i++){
			Counters[i]+=1;
		}	
	}
}
/*********************************************END OF FILE**********************/
