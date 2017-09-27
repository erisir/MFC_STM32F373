#include "mfc_pwm.h"

/**
 * @brief  配置TIM2复用输出PWM时用到的I/O
 * @param  无
 * @retval 无
 */
void TIM2_GPIO_Config(void) 
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
 /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PC0 and PC1 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
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
	TIM_OCInitStructure.TIM_Pulse = 0;										  			//设置初始PWM脉冲宽度为0	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为低电平

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);	 									//使能通道2


	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);						//使能预装载2	

	TIM_ARRPreloadConfig(TIM2, ENABLE);			 										//使能TIM2重载寄存器ARR
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1);
	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);                   										//使能定时器2	

	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);										//使能update中断

																			

}

/**
 * @brief  TIM2 呼吸灯初始化
 *         配置PWM模式和GPIO
 * @param  无
 * @retval 无
 */
void TIM2_PWM_Init(void)
{
	TIM2_GPIO_Config();//引脚
	TIM2_Mode_Config(0);	//TIMER 相关
	NVIC_Config_PWM();//中断优先级					
}
void TIM2_Set_Prescaler(unsigned int Prescaler){
	TIM2_Mode_Config(Prescaler);	//TIMER 相关
}
/**************** 计算PWM重装值函数 *******************/
//注意：TIM_SetCompare2的2为ch2的寄存器（PA1）
void    LoadPWM(unsigned int pwmval)
{
	TIM_SetCompare2(TIM2,pwmval);	
}
/*********************************************END OF FILE**********************/
