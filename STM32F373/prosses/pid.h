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
 

#define EEPROM_PID_P		0
#define EEPROM_PID_I		1
#define EEPROM_PID_D		2

#define EEPROM_PID_PF		3
#define EEPROM_PID_IF		4
#define EEPROM_PID_DF		5

#define EEPROM_PID_EFRL		6
#define EEPROM_PID_EFRM		7
#define EEPROM_PID_EFRS		8

#define EEPROM_PID_ECFRL		9
#define EEPROM_PID_ECFRM		10
#define EEPROM_PID_ECFRS		11


#define EEPROM_PID_CUTOFF_FREQ		12
#define EEPROM_PID_CONTROL_CYCLE 13
#define EEPROM_PID_DEADZONE		14
 
#define EEPROM_PWM_MAX_HIGH 15
#define EEPROM_PWM_MIN_HIGH 16
#define EEPROM_PWM_STEP_HIGH 17

#define EEPROM_PWM_MAX_LOW 18
#define EEPROM_PWM_MIN_LOW 19
#define EEPROM_PWM_STEP_LOW 20

#define EEPROM_SUM				21

#define NL 0 
#define NM 1 
#define NS 2 
#define ZE 3 
#define PS 4 
#define PM 5 
#define PL 6 

/************************************************
PID函数

 *************************************************/ 
/*************PID**********************************/
struct _PID {
	int kpid[3]; // kp ki kd
	int kpidF[3]; // kp ki kd factor	
	uint16_t eFuzzyRule[3]; //  high middle low
	uint16_t ecFuzzyRule[3]; //  high middle low
 	
	int PID_Cutoff;
	uint16_t PID_ControlCycle;
	int PID_DeadZone;
	 

	int LastError; // Error[-1]
	int PrevError; // Error[-2]
	int32_t SumError;
	
	uint32_t PWM_MAX;
	uint32_t PWM_MIN;
	uint32_t PWM_STEP;
	
};
extern struct _PID spid;

void SetHysteresis(uint8_t pe,uint8_t pec);

void PID_Init (void);  
void EEPROM_INIT(void);
void EEPROM_READ_PID(void);
void EEPROM_SAVE_PID(void);

void PID_Param_Reset(void);
void PID_Start(void); 		

void Set_Running_Param(uint8_t *buf);
uint8_t Get_Running_Param(uint8_t *buf);

void Set_PID_Param(uint8_t *buf); 
uint8_t Get_PID_Param(uint8_t *buf); 

void Set_FuzzyMap_Param(uint8_t *buf);
void Get_FuzzyMap_Param(uint8_t *buf,uint8_t row);
void Valve_Close(void);
void Valve_Open(void);
void Valve_PID_Ctrl(void);
void LoadPWMTemp(uint32_t value);

uint8_t PID_isRunning(void);
uint16_t abs( int val);
uint16_t Get_ControlCycle(void);

void Inc_PID_Calc(void);
 
void Fuzzy_Kpid(int16_t e, int16_t ec) ;
void Init_FuzzyMap(void);
uint32_t Bytes2Int32_t(uint8_t * buf,uint8_t offset);
uint16_t Bytes2Int16_t(uint8_t * buf,uint8_t offset);

void Int32_t2Bytes(uint32_t val,uint8_t * buf,uint8_t offset);
void Int16_t2Bytes(uint16_t val,uint8_t * buf,uint8_t offset);
#endif
/*********************************************END OF FILE**********************/
