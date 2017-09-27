#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f37x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
#define     PWM_DUTY        32768            //定义PWM的周期，数值为时钟周期数，假如使用24.576MHZ的主频，则PWM频率为6000HZ。

#define     PWM_HIGH_MIN    10000              //限制PWM输出的最小占空比。用户请勿修改。
#define     PWM_HIGH_MAX    32758 					//限制PWM输出的最大占空比。用户请勿修改。
void TIM2_GPIO_Config(void) ;
void NVIC_Config_PWM(void);
void TIM2_Mode_Config(unsigned int Prescaler);
void TIM2_PWM_Init(void);
void LoadPWM(unsigned int high);
void TIM2_Set_Prescaler(unsigned int Prescaler);
#endif
