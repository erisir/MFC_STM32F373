#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f37x.h"

#define     PWM_DUTY        12000          

#define     PWM_HIGH_MAX    690000 
#define     PWM_HIGH_MIN    10000   
#define __PWM_DITHER_MODE_
//#define __PWM_NONMAL_MODE_
 
void PWM_GPIO_Config(void) ;
void PWM_NVIC_Config(void);
void PWM_Mode_Config(void);

void PWM_Init(void);

void PWW_DMA_Init(void);
void LoadPWM(uint32_t high);
void TIM2_Set_Prescaler(unsigned int Prescaler);
void DMA_Total_Transfer_interrupt(void);
void DMA_Half_Transfer_interrupt(void);
 
 
#endif
