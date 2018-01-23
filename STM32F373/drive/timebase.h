#ifndef TIME_TEST_H
#define TIME_TEST_H
 
#include "../prosses/tasks.h"
#define CounterNum 4
 

 
void TIM4_Configuration(unsigned int Prescaler);
void TIM4_Init(void);
void TIM4_Set_Prescaler(unsigned int Prescaler);
#endif	/* TIME_TEST_H */
