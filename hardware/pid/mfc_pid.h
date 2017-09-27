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
/************************************************
PID函数
 *************************************************/ 
/*************PID**********************************/
struct PID {
	double Proportion; // 比例常数 Proportional Const
	double Integral; // 积分常数 Integral Const
	double Derivative; // 微分常数 Derivative Const

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

void PIDInit (void);  
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
void GetPIDStatu(void); 
unsigned int getPeriod(void);
unsigned char isPIDEnable(void);
unsigned int abs( int val);
unsigned int LocPIDCalc(struct PID *spid,int NextPoint);
int IncPIDCalc(struct PID *spid,int NextPoint);
int IncAutoPIDCalc(struct PID *spid,int NextPoint);
#endif
/*********************************************END OF FILE**********************/
