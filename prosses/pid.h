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

#include "stdint.h"

///////////////////////////////////////////////////////////
#define EEPROM_offset_acc_x		0
#define EEPROM_offset_acc_y		1
#define EEPROM_offset_acc_z		2
#define EEPROM_offset_gyro_x	3
#define EEPROM_offset_gyro_y	4
#define EEPROM_offset_gyro_z	5

#define EEPROM_PID_P		6
#define EEPROM_PID_I		7
#define EEPROM_PID_D		8
#define EEPROM_PID_sumMax		9
#define EEPROM_PID_sumMin		10
#define EEPROM_PID_Thredhold		11

#define EEPROM_PID_DeadZone		12
#define EEPROM_PID_Period		13
#define EEPROM_PID_YAW_D		14
#define EEPROM_SUM				15
/************************************************
PID函数
 *************************************************/ 
/*************PID**********************************/
struct PID {
	float Proportion; // 比例常数 Proportional Const
	float Integral; // 积分常数 Integral Const
	float Derivative; // 微分常数 Derivative Const

	int LastError; // Error[-1]
	int PrevError; // Error[-2]
	int SumError;
	
	unsigned int sumMax;
	unsigned int sumMin;

	int Output;
	unsigned int Thredhold;
	unsigned int SetPoint;
	unsigned int DeadZone;
	unsigned int Period;
};
struct PWMVotageFitPara {
	double ForwardA; // 比例常数 Proportional Const
	double ForwardB; // 积分常数 Integral Const
	double ForwardC; // 微分常数 Derivative Const
	double BackwardA; // 比例常数 Proportional Const
	double BackwardB; // 积分常数 Integral Const
	double BackwardC; // 微分常数 Derivative Const
};

void PID_Init (void);  
void PID_Reset(void);
void PIDStart(void); 		 

void SetSetPoint(float v_data); 
void SetPWMValue(float v_data);
void SetPIDPeriod(float v_data);
void SetPIDparam_P_inc(float v_data); 
void SetPIDparam_I_inc(float v_data); 
void SetPIDparam_D_inc(float v_data); 
void SetTClose(void);
void SetTOpen(void);
void SetTPID(void);
void SetPIDMode(float mode);
void SetPIDVotageChanel(float ch);
void SetPIDThredHold(float v_data);
void SetForwardA(float v_data); 
void SetForwardB(float v_data); 
void SetForwardC(float v_data);
void SetBackwardA(float v_data);
void SetBackwardB(float v_data);
void SetBackwardC(float v_data);
unsigned int getPWMByVotage(unsigned int votage,char forBackward);
uint32_t GetPIDOutput(void);
void GetPIDStatu(char *buf); 
unsigned int getPeriod(void);
unsigned char isPIDEnable(void);
unsigned int abs( int val);
unsigned int LocPIDCalc(struct PID *spid,int NextPoint);
int IncPIDCalc(struct PID *spid,int NextPoint);
int IncAutoPIDCalc(struct PID *spid,int NextPoint);

void EEPROM_INIT(void);
void EEPROM_READ_PID(void);
void EEPROM_SAVE_PID(void);
#endif
/*********************************************END OF FILE**********************/
