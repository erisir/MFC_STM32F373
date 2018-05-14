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
struct _PID {//21 ints
  uint16_t Voltage_Set_Point;
	uint16_t kpid[3]; // kp ki kd
	uint16_t kpidF[3]; // kp ki kd factor	
	uint16_t eFuzzyRule[3]; //  high middle low
	uint16_t ecFuzzyRule[3]; //  high middle low
 	
	uint16_t PID_Cutoff;
	uint16_t PID_ControlCycle;
	uint16_t PID_DeadZone;	
	
	//内存分配以32bit为单位。之间如果前面uint16为奇数个，则在此会自动添加一个0的uint16补齐
	uint32_t PWM_MAX;
	uint32_t PWM_MIN;
	uint32_t PWM_STEP;
};

struct _FuzzyCtrlRuleMap{
	int8_t data[7][7][3];
};
extern struct _PID *  spid;

void SetHysteresis(uint8_t pe,uint8_t pec);

void PID_Init (void);  
void Init_FuzzyMap(void);
void EEPROM_INIT(void);

void EEPROM_READ_PID(void);
void EEPROM_SAVE_PID(void);

void PID_Param_Reset(void);
void FuzzyMap_Param_Reset(void);

void PID_Start(void); 		
void Inc_PID_Calc(void);

void SetValveMode(uint8_t mode);
void SetContrlResource(uint8_t mode);
void Set_PID_Param(void); 

void Valve_Close(void);
void Valve_Open(void);
void Valve_PID_Ctrl(void);
 

uint8_t PID_isRunning(void);
uint16_t myabs( int val);
uint16_t Get_ControlCycle(void);


 
void Fuzzy_Kpid(int16_t e, int16_t ec) ;
 
#endif
/*********************************************END OF FILE**********************/
