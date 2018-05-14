#ifndef _TASKS_H_
#define	_TASKS_H_

#include "stm32f37x.h"

#include "sys.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "sdadc.h"
#include "pwm.h" 
#include "timebase.h" 
#include "eeprom.h" 
#include "spi.h" 


#include "pid.h" 

#include <stdio.h>
#include "math.h"
#include "string.h"
#include "../../modbus/user_mb_app.h"


 

/******************************************************************************
							全局变量声明
*******************************************************************************/ 

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void RCC_Configuration(void);
void Bsp_Int(void );
void OnResetFlowDown(void); 
#endif

