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
//内存分配以32bit为单位。之间如果前面uint16为奇数个，则在此会自动添加一个0的uint16补齐,这样在modbus中读出来的就是0



extern int32_t PWM_Output;

void PIDInit (void);  
void setVoltageSetPoint(uint16_t voltage);
uint16_t getVoltageSetPoint(void);
void FuzzyCtrlRuleMapInit(void);
void FuzzyRuleInit(void);

void PID_Start(void); 		
void Inc_PID_Calc(void);
void Fuzzy_Kpid(int16_t e, int16_t ec) ;

uint16_t myabs( int val);



#endif
/*********************************************END OF FILE**********************/
