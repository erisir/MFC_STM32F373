#ifndef __TIMER_H
#define __TIMER_H
#include "port.h"

#define     PWM_DUTY        65535          
 
#define __PWM_DITHER_MODE_
//#define __PWM_NONMAL_MODE_
 
 
 
void PWM_Init(void);
 
void LoadPWM(uint32_t high);
void TIM2_Set_Prescaler(unsigned int Prescaler);
void DMA_Total_Transfer_interrupt(void);
void PWM_DMA1_Channel7_IRQHandler(void);
 
#endif
