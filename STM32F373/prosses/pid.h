/********************************************************************
本程序只供学习使用，未经作者许可，不得用于其它任何用途
程序结构参考 安徽师范大学  Lyzhangxiang的EasyHW OS结构设计
datacomm.h
作者：bg8wj
建立日期: 2012.12.23
版本：V1.0

Copyright(C) bg8wj
 ********************************************************************/
#ifndef  __PID_H__
#define  __PID_H__

#include "tasks.h"

///////////////////////////////////////////////////////////
 

#define EEPROM_PID_PH		0
#define EEPROM_PID_IH		1
#define EEPROM_PID_DH		2

#define EEPROM_PID_PM		3
#define EEPROM_PID_IM		4
#define EEPROM_PID_DM		5

#define EEPROM_PID_PL		6
#define EEPROM_PID_IL		7
#define EEPROM_PID_DL		8

#define EEPROM_PID_THD_H		9
#define EEPROM_PID_THD_L		10
#define EEPROM_PID_THD_PWM		11

#define EEPROM_PID_DEADZONE		12
#define EEPROM_RUN_RES_TIME		13
#define EEPROM_RUN_FREQ_CUTOFF 14
 

 
#define EEPROM_SUM				15
/************************************************
PID函数
 *************************************************/ 
/*************PID**********************************/
struct _PID {
	float kpH; // 比例常数 Proportional Const
	float kiH; // 积分常数 Integral Const
	float kdH; // 微分常数 Derivative Const
	
	float kpM; // 比例常数 Proportional Const
	float kiM; // 积分常数 Integral Const
	float kdM; // 微分常数 Derivative Const
	
	float kpL; // 比例常数 Proportional Const
	float kiL; // 积分常数 Integral Const
	float kdL; // 微分常数 Derivative Const
	
		
	float error_High_Threadhold;
	float error_Low_Threadhold;
	int16_t  PWM_Change_Threadhold;
	int16_t  deadzone;

	float LastError; // Error[-1]
	float PrevError; // Error[-2]
	int32_t SumError;
	
};
extern struct _PID spid;
uint16_t getFeedBackTime(void);

void PID_Init (void);  
void EEPROM_INIT(void);
void EEPROM_READ_PID(void);
void EEPROM_SAVE_PID(void);

void PID_Param_Reset(void);
void PID_Start(void); 		

void Set_Running_Param(uint8_t *buf);
void Get_Running_Param(uint8_t *buf);

void Set_PID_Param(uint8_t *buf); 
void Get_PID_Param(uint8_t *buf); 

void Valve_Close(void);
void Valve_Open(void);
void Valve_PID_Ctrl(void);


unsigned char is_PID_Running(void);
unsigned int abs( int val);
 
void Inc_PID_Calc(void);
void Inc_PID_Calc1(void);
void PID_Setpoint_Change(void);

#endif
/*********************************************END OF FILE**********************/
