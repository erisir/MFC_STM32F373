#ifndef _TASKS_H_
#define	_TASKS_H_
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "led.h"
#include "sdadc.h"
#include "pwm.h" 
#include <stdio.h>

/******************************************************************************
							宏定义
*******************************************************************************/ 
//用于测量程序运行速率
#define	Debug1_H   	GPIO_SetBits(GPIOF, GPIO_Pin_6 )  //高电平
#define	Debug1_L   	GPIO_ResetBits(GPIOF, GPIO_Pin_6 )//低电平

	
#define	Debug2_H   	GPIO_SetBits(GPIOF, GPIO_Pin_7 )
#define	Debug2_L   	GPIO_ResetBits(GPIOF, GPIO_Pin_7 )//低电平


#define	Debug3_H   	GPIO_SetBits(GPIOA, GPIO_Pin_14 )
#define	Debug3_L   	GPIO_ResetBits(GPIOA, GPIO_Pin_14 )//低电平


#define	Debug4_H   	GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define	Debug4_L   	GPIO_ResetBits(GPIOA, GPIO_Pin_15 )//低电平

/******************************************************************************
							全局变量声明
*******************************************************************************/ 

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void OnResetFlowDown(void);
void Task_1000HZ(void);
void Task_500HZ(void);
void Task_200HZ(void);
void Task_100HZ(void);
void Task_10HZ(void);
 
#endif

