/********************************************************************
������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
����ṹ�ο� ����ʦ����ѧ  Lyzhangxiang��EasyHW OS�ṹ���
datacomm.h
���ߣ�bg8wj
��������: 2012.12.23
�汾��V1.0

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
PID����

 *************************************************/ 
/*************PID**********************************/
//�ڴ������32bitΪ��λ��֮�����ǰ��uint16Ϊ�����������ڴ˻��Զ����һ��0��uint16����,������modbus�ж������ľ���0



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
