#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"
 

#define     PWM_DUTY        65535  
#define DitherBit   32

#define     PWM_MAX_VALUE PWM_DUTY*32
#define     PWM_MIN_MIN PWM_DUTY

void StartTimPwmDMA(void); 

void LoadPWM(uint32_t high);



#endif
