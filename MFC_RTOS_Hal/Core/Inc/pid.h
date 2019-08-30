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

#include "main.h"

///////////////////////////////////////////////////////////
 


typedef enum{
	EEPROM_PID_P = 1,
	EEPROM_PID_I,
	EEPROM_PID_D,
	EEPROM_PID_PF,
	EEPROM_PID_IF,
	EEPROM_PID_DF,
	EEPROM_PID_EFRL,
	EEPROM_PID_EFRM,
	EEPROM_PID_EFRS,
	EEPROM_PID_ECFRL,
	EEPROM_PID_ECFRM,
	EEPROM_PID_ECFRS,
	EEPROM_PID_CUTOFF_FREQ,
	EEPROM_PID_CONTROL_CYCLE,
	EEPROM_PID_DEADZONE,
	EEPROM_PWM_MAX_HIGH,
	EEPROM_PWM_MIN_HIGH,
	EEPROM_PWM_STEP_HIGH,
	EEPROM_PWM_MAX_LOW,
	EEPROM_PWM_MIN_LOW,
	EEPROM_PWM_STEP_LOW,
	EEPROM_CAL_VAL_0,
	EEPROM_CAL_VAL_10,
	EEPROM_CAL_VAL_20,
	EEPROM_CAL_VAL_30,
	EEPROM_CAL_VAL_40,
	EEPROM_CAL_VAL_50,
	EEPROM_CAL_VAL_60,
	EEPROM_CAL_VAL_70,
	EEPROM_CAL_VAL_80,
	EEPROM_CAL_VAL_90,
	EEPROM_CAL_VAL_100,
	EEPROM_SUM,
}EEPROM_SAVE_INDEX;

typedef enum{
 NL=0 ,
 NM  ,
 NS  ,
 ZE  ,
 PS  ,
 PM  ,
 PL  ,
}FUZZY_MAP_FLAG;

/************************************************
PID函数

 *************************************************/ 
/*************PID**********************************/
struct _PID{//21 ints
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
}  ;

struct _FuzzyCtrlRuleMap {
	int8_t data[7][7][3];
};
struct _CALVALUE {
	int16_t value[11];
};

extern struct _PID *  spid;

void PID_Init (void);  
void EEPROM_INIT(void);

void EEPROM_READ_PID(void);
void EEPROM_SAVE_PID(void);

void EEPROM_READ_Correct(void);
void EEPROM_SAVE_Correct(void);


void FuzzyRuleInit(void);
void PIDStructInit(void);
void CalValueStructInit(void);

void FuzzyMapInit(uint8_t mapId);

void PID_Start(void); 		
void Inc_PID_Calc(void);

void SetValveMode(uint8_t mode);
void SetContrlResource(uint8_t mode);
void Set_PID_Param(void); 
void PIDSetPointChange(void);
void Set_Correct_Param(void); 

void Valve_Close(void);
void Valve_Open(void);
void Valve_PID_Ctrl(void);
 

uint8_t PID_isRunning(void);
uint16_t myabs( int val);
uint16_t Get_ControlCycle(void);

uint16_t piecewiselinearinterp(struct _CALVALUE * xDict,struct _CALVALUE * yDict,uint16_t DictSize,uint16_t xInput);//电压层面的分段线性插值
uint16_t VoltageToFlow(uint16_t voltage);
uint16_t FlowToVoltage(uint16_t flow);
void VoltageOutLinerFix(void);
 
void Fuzzy_Kpid(int16_t e, int16_t ec) ;
 
#endif
/*********************************************END OF FILE**********************/
