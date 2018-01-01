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
							�궨��
*******************************************************************************/ 
//���ڲ���������������
#define	Debug1_H   	GPIO_SetBits(GPIOF, GPIO_Pin_6 )  //�ߵ�ƽ
#define	Debug1_L   	GPIO_ResetBits(GPIOF, GPIO_Pin_6 )//�͵�ƽ

	
#define	Debug2_H   	GPIO_SetBits(GPIOF, GPIO_Pin_7 )
#define	Debug2_L   	GPIO_ResetBits(GPIOF, GPIO_Pin_7 )//�͵�ƽ


#define	Debug3_H   	GPIO_SetBits(GPIOA, GPIO_Pin_14 )
#define	Debug3_L   	GPIO_ResetBits(GPIOA, GPIO_Pin_14 )//�͵�ƽ


#define	Debug4_H   	GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define	Debug4_L   	GPIO_ResetBits(GPIOA, GPIO_Pin_15 )//�͵�ƽ

/******************************************************************************
							ȫ�ֱ�������
*******************************************************************************/ 

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void OnResetFlowDown(void);
void Task_1000HZ(void);
void Task_500HZ(void);
void Task_200HZ(void);
void Task_100HZ(void);
void Task_10HZ(void);
 
#endif

