#include "pwm.h"
#include <stdio.h>


uint8_t aDitherTable[32][32]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}; 
uint32_t vDitherTemp[32];
uint32_t vDither[64];
uint16_t duty_Cycle ;//取整
uint8_t Dither_Index;//余数
uint8_t PWM_Value_FirstPart_Changed=0;
uint8_t PWM_Value_SecondPart_Changed=0;
 
void TIM2_GPIO_Config(void) 
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
 /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PC0 and PC1 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
void NVIC_Config_PWM(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* 配置TIM2_IRQ中断为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
}

/**
 * @brief  配置TIM2输出的PWM信号的模式，如周期、极性
 * @param  无
 * @retval 无
 */

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> 中断 & TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 *
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 
 */

/*    _______    ______     _____      ____       ___        __         _
 * |_|       |__|      |___|     |____|    |_____|   |______|  |_______| |________|
 */
void TIM2_Mode_Config(unsigned int Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				

	/* 设置TIM2CLK 时钟为72MHZ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 					//使能TIM2时钟

	/* 基本定时器配置 */		 
	TIM_TimeBaseStructure.TIM_Period = PWM_DUTY;   				  //当定时器从0计数到255，即为266次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;	    							//设置预分频：
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//设置时钟分频系数：不分频(这里用不到)
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM模式配置 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
	TIM_OCInitStructure.TIM_Pulse = 64;										  			//设置初始PWM脉冲宽度为0	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为低电平

	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	 									//使能通道 -CH4


	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);						//使能预装载 	-CH4

	TIM_ARRPreloadConfig(TIM2, ENABLE);			 										//使能TIM2重载寄存器ARR
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);                   										//使能定时器2	

	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);										//使能update中断
  //dma
	
	//TIM_DMAConfig(TIM2,TIM_DMABase_CCR4,TIM_DMABurstLength_4Bytes);  
	TIM_DMACmd( TIM2, TIM_DMA_Update, ENABLE );
    //产生DMA请求信号  
  TIM_DMACmd(TIM2, TIM_DMA_CC4, ENABLE);  	
  TIM_CtrlPWMOutputs(TIM2, ENABLE ); /* Main Output Enable */	
}

/**
 * @brief  TIM2 呼吸灯初始化
 *         配置PWM模式和GPIO
 * @param  无
 * @retval 无
 */
void PWM_Init(void)
{
	TIM2_GPIO_Config();//引脚
	PWW_DMA_Init();	
	TIM2_Mode_Config(0);	//TIMER 相关
	NVIC_Config_PWM();//中断优先级		

}
void TIM2_Set_Prescaler(unsigned int Prescaler){
	TIM2_Mode_Config(Prescaler);	//TIMER 相关
}

void PWW_DMA_Init(void){
            
	DMA_InitTypeDef DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );  // dma1时钟使能
    
  DMA_DeInit( DMA1_Channel7 );   // DMA复位
  DMA_StructInit( &DMA_InitStructure );   // DMA缺省的参数
    
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &TIM2->CCR4; //外设地址0x40000040;//
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &vDither;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //dma传输方向,单向
  DMA_InitStructure.DMA_BufferSize = 64; //设置DMA在传输时缓冲区的长度
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
	
	uint8_t i= 0;
	duty_Cycle = (uint16_t)(pwmval/32);//取整
	Dither_Index = (uint16_t)(pwmval%32) ;//余数
	for(i = 0;i<32;i++){
		vDitherTemp[i] = (uint16_t)(duty_Cycle+aDitherTable[Dither_Index][i]);
	}
	PWM_Value_FirstPart_Changed =0;
	PWM_Value_SecondPart_Changed =0;
}
void DMA_Half_Transfer_interrupt(void){
	uint8_t i= 0;
	for(i = 0;i<32;i++){
		vDither[i] = vDitherTemp[i];
		}
	PWM_Value_FirstPart_Changed = 1;
}
void DMA_Total_Transfer_interrupt(void){
	uint8_t i= 0;
	for(i = 0;i<32;i++){
		vDither[i+32] = vDitherTemp[i];
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
