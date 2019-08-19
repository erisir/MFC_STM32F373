/**
  ******************************************************************************
  * @file    pwm.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   pwm
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 	
#include "pwm.h"
 
#define     PWM_DUTY        65535 

#define DitherBit   32
uint8_t aDitherTable[32][32]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}; 
//uint8_t aDitherTable[8][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,1},{0,0,0,1,0,0,0,1},{0,0,0,1,0,1,0,1},{0,1,0,1,0,1,0,1},{0,1,0,1,1,0,1,1},{0,1,1,1,0,1,1,1},{0,1,1,1,1,1,1,1}};

uint32_t vDitherTemp[DitherBit];
uint32_t vDither[DitherBit*2];
uint8_t BufferSize = DitherBit*2;

uint16_t duty_Cycle ;//取整
uint8_t Dither_Index;//余数
uint8_t PWM_Value_FirstPart_Changed=0;
uint8_t PWM_Value_SecondPart_Changed=0;

void PWM_Mode_Config(uint16_t Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
  TIM_Cmd(TIM2, DISABLE);  
	/* 设置TIM2CLK 时钟为72MHZ */
	

	/* 基本定时器配置 */		 
	TIM_TimeBaseStructure.TIM_Period = Period;   				        //当定时器从0计数到255，即为266次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    				//设置预分频：
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM模式配置 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
	TIM_OCInitStructure.TIM_Pulse = 0;					  			//设置初始PWM脉冲宽度为0	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//;TIM_OCPolarity_Low  	  //当定时器计数值小于CCR1_Val时为低电平

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);	 									 //使能通道 -CH2
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);						 //使能预装载 	-CH2

	TIM_ARRPreloadConfig(TIM2, ENABLE);			 										 //使能TIM2重载寄存器ARR
	
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);                   										 //使能定时器2	

	//TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);									 //使能update中断
	
  //dma
	
	#ifdef __PWM_DITHER_MODE_
	TIM_DMACmd( TIM2, TIM_DMA_Update, ENABLE );    							 //产生DMA请求信号  
  TIM_DMACmd(TIM2, TIM_DMA_CC2, ENABLE);  	
	#endif
	
  TIM_CtrlPWMOutputs(TIM2, ENABLE );  
}

 
void PWM_Init(void)
{
 
	#ifdef __PWM_DITHER_MODE_
	PWW_DMA_Init();	
	#endif
	PWM_Mode_Config(PWM_DUTY);	//TIMER 相关
}
 
void PWW_DMA_Init(void){
            
	DMA_InitTypeDef DMA_InitStructure;
        
  DMA_DeInit( DMA1_Channel7 );   // DMA复位
  DMA_StructInit( &DMA_InitStructure );   // DMA缺省的参数
    
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &TIM2->CCR2; //外设地址0x40000040;//
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &vDither;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //dma传输方向,单向
  DMA_InitStructure.DMA_BufferSize = BufferSize; //设置DMA在传输时缓冲区的长度
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //设置DMA的外设递增模式，一个外设
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //设置DMA的内存递增模式，
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //外设数据字长
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;   //内存数据字长
  //循环模式开启，Buffer写满后，自动回到初始地址开始传输
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //设置DMA的传输模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; //设置DMA的优先级别
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //设置DMA的2个memory中的变量互相访问
  DMA_Init( DMA1_Channel7, &DMA_InitStructure );
  
  DMA_ClearFlag( DMA1_IT_TC7 );
	DMA_ClearFlag( DMA1_IT_HT7 );
	DMA_ClearFlag( DMA1_IT_TE7 );
  DMA_ITConfig( DMA1_Channel7, DMA_IT_TC, ENABLE );
	DMA_ITConfig( DMA1_Channel7, DMA_IT_HT, ENABLE );
	DMA_ITConfig( DMA1_Channel7, DMA_IT_TE, ENABLE );
  
  DMA_Cmd( DMA1_Channel7, ENABLE );
//=======================================================================
}
/**************** 计算PWM重装值函数 *******************/
//注意：TIM_SetCompare4的4为ch4的寄存器（PA3）-TIM2 CH4
void    LoadPWM(uint32_t pwmval)
{	
	uint8_t i;
 
	#ifdef __PWM_DITHER_MODE_
	duty_Cycle = (uint16_t)(pwmval/DitherBit);//取整
	Dither_Index = (uint16_t)(pwmval%DitherBit) ;//余数
	for(i = 0;i<DitherBit;i++){		
		vDitherTemp[i] =  (uint16_t)(duty_Cycle+aDitherTable[Dither_Index][i]);
	}
	PWM_Value_FirstPart_Changed =0;
	PWM_Value_SecondPart_Changed =0;
	#endif
	
	#ifdef __PWM_NONMAL_MODE_
	TIM_SetCompare2(TIM2,pwmval);	
	#endif
}
void DMA_Half_Transfer_interrupt(void){
	uint8_t i= 0;
	for(i = 0;i<DitherBit;i++){
		vDither[i] = vDitherTemp[i];
	}
	PWM_Value_FirstPart_Changed = 1;
}
void DMA_Total_Transfer_interrupt(void){
	uint8_t i= 0;
	for(i = 0;i<DitherBit;i++){
		vDither[i+DitherBit] = vDitherTemp[i];
	}
	PWM_Value_SecondPart_Changed =1;
}
 
void DMA1_Channel7_IRQHandler(void)
{	
	if(DMA_GetITStatus(DMA1_IT_TC7) !=RESET){
		DMA_ClearITPendingBit( DMA1_IT_TC7 );
		if(PWM_Value_SecondPart_Changed ==0)
			DMA_Total_Transfer_interrupt();
	}
	if(DMA_GetITStatus(DMA1_IT_HT7) !=RESET)
	{
		DMA_ClearITPendingBit( DMA1_IT_HT7 );
		if(PWM_Value_FirstPart_Changed ==0)
			DMA_Half_Transfer_interrupt();		
	}
	if(DMA_GetITStatus(DMA1_IT_TE7) !=RESET)	
	{
		DMA_ClearITPendingBit( DMA1_IT_TE7 );
	}
  
}
/*********************************************END OF FILE**********************/
