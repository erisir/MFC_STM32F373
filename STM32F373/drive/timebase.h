#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f37x.h"
void TIM4_NVIC_Configuration(void);
void TIM4_Configuration(unsigned int Prescaler);
void TIM4_Init(void);
void TIM4_Set_Prescaler(unsigned int Prescaler);
#endif	/* TIME_TEST_H */
