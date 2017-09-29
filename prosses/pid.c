#include "pid.h"
#include "sdadc.h"
#include "delay.h" 
#include <stdio.h>
#include "pwm.h"
#include "eeprom.h"
#include "rs485.h"
#include "string.h"
#include "math.h"

//xdata struct PID spid; // PID Control Structure
struct PID spid; // PID Control Structure
struct PWMVotageFitPara sPVFP;
unsigned int PIDVotageChanel = 0;
unsigned int PIDMode = 0;
unsigned char PIDEnable=0;
float PWMDelayRate = 0.1;

uint16_t data_x,data_y,data_z;

uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06,0x9A07,0x9A08,0x9A09,
	0x9A10,0x9A12,0x9A13,0x9A14,0x9A15
};//VirtAddVarTab

void SetPIDMode(float mode){
	PIDMode = mode;
}
void SetPIDVotageChanel(float ch){
	PIDVotageChanel = ch;	 
}
uint32_t GetPIDOutput(void){
	return spid.Output;
}

void GetPIDStatu(char *buf){
	sprintf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",spid.Proportion  ,spid.Integral ,spid.Derivative  ,spid.DeadZone ,spid.SetPoint ,spid.Output,spid.LastError,spid.PrevError,spid.SetPoint-spid.LastError,spid.SumError );	
	//printf("FA:%.3f,FB:%.3f,FC:%.3f,BA:%.3f,BB:%.3f,BC:%.3f\n",sPVFP.ForwardA,sPVFP.ForwardB,sPVFP.ForwardC,sPVFP.BackwardA,sPVFP.BackwardB,sPVFP.BackwardC );	
}  
/*********************************************************** 
              PID温度控制动作函数
 ***********************************************************/ 
void PIDStart() 		 
{ 
	int iIncpid = 0;
	switch(PIDMode){
	case 0:
		iIncpid=IncAutoPIDCalc ( &spid,GetADCVoltage(PIDVotageChanel) );
		break;
	case 1:
		iIncpid=IncPIDCalc ( &spid,GetADCVoltage(PIDVotageChanel) );
		break;
	default:
		iIncpid=IncPIDCalc ( &spid,GetADCVoltage(PIDVotageChanel) );
		break;
	}		
	if(spid.Output >PWM_HIGH_MAX)
		spid.Output = PWM_HIGH_MAX; 
	if(spid.Output <PWM_HIGH_MIN)
		spid.Output = PWM_HIGH_MIN;  
	LoadPWM(spid.Output) ;
	//delay_ms(abs(iIncpid)*PWMDelayRate);

}

//增量式PID控制设计
int IncPIDCalc(struct PID *spid,int NextPoint)
{
	register int iError, iIncpid;
	//当前误差
	iError = spid->SetPoint - NextPoint;
	/*if(abs(iError) >spid->Thredhold){
		spid->Output = getPWMByVotage(spid->SetPoint,iError);
		return 0;
	}*/
	//增量计算
	iIncpid = spid->Proportion * iError //E[k]项
			- spid->Integral * spid->LastError //E[k－1]项
			+ spid->Derivative * spid->PrevError; //E[k－2]项
	//存储误差，用于下次计算
	spid->PrevError = spid->LastError;
	spid->LastError = iError;
	//返回增量值
		if(abs(iIncpid) >spid->Thredhold){
		iIncpid = iIncpid>0?spid->Thredhold:(-1*spid->Thredhold);
	}
	spid->Output += iIncpid;
	return iIncpid;
}

//增量式自适应PID控制设计
int IncAutoPIDCalc(struct PID *spid,int NextPoint)
{
	register int iError, iIncpid;
	//当前误差
	iError = spid->SetPoint - NextPoint;

	//增量计算
	iIncpid = spid->Proportion * (2.45*iError //E[k]项
			- 3.5*spid->LastError //E[k－1]项
			+ 1.25*spid->PrevError); //E[k－2]项
	//存储误差，用于下次计算
	spid->PrevError = spid->LastError;
	spid->LastError = iError;
	//返回增量值
	
	if(abs(iIncpid) >spid->Thredhold){
		iIncpid = iIncpid>0?spid->Thredhold:(-1*spid->Thredhold);
	}
	spid->Output += iIncpid;
	return iIncpid;
}
void EEPROM_INIT(void)// 
{
	EE_Init();	
	EEPROM_READ_PID();
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_SUM], &data_x);
	if(data_x != EEPROM_SUM)//
	{
		__set_PRIMASK(1);// 
		EE_WriteVariable(VirtAddVarTab[EEPROM_SUM], EEPROM_SUM);
		 __set_PRIMASK(0);// 
		PID_Reset();// 
	}
}
void EEPROM_SAVE_PID(void)	 
{
	 __set_PRIMASK(1); 
	data_x = (int16_t) (spid.Proportion*1000);
	data_y = (int16_t) (spid.Integral*1000);
	data_z = (int16_t) (spid.Derivative*1000);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_D], data_z);

	data_x = (int16_t) (spid.sumMax);
	data_y = (int16_t) (spid.sumMin);
	data_z = (int16_t) (spid.Thredhold);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_sumMax], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_sumMin], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_Thredhold], data_z);

	data_x = (int16_t) (spid.DeadZone);
	data_y = (int16_t) (spid.Period);
 
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_DeadZone], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_Period], data_y);

	__set_PRIMASK(0); 
}
void EEPROM_READ_PID(void)	// 
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_D], &data_z);
	spid.Proportion = (float) data_x/1000.0f;
	spid.Integral = (float) data_y/1000.0f;
	spid.Derivative = (float) data_z/1000.0f;
 
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_sumMax], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_sumMin], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_Thredhold], &data_z);
	spid.sumMax = (float) data_x;
	spid.sumMin = (float) data_y;
	spid.Thredhold = (float) data_z;
 
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DeadZone], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_Period], &data_y);
 
	spid.DeadZone = (float) data_x;
	spid.Period = (float) data_y;
 
}
/************************************************
				PID函数初始化
 *************************************************/
void PID_Init() 
{ 
	 FLASH_Unlock();
	 EEPROM_INIT();// 
}

void PID_Reset(void) 
{ 
	memset (&spid,0,sizeof(struct PID)); 	// Initialize Structure 
	memset (&sPVFP,0,sizeof(struct PWMVotageFitPara)); 	// Initialize Structure 

	spid.Proportion = 2;  
	spid.Integral =   3; 
	spid.Derivative =5; 
	spid.Output = 0;
	spid.SetPoint = 3000;
	spid.DeadZone = 0;
	spid.Period = 100;
	spid.sumMax=999999;
	spid.sumMin=10;
	spid.Thredhold = 5000;
	
	sPVFP.ForwardA = 0;
	sPVFP.ForwardB = 0;
	sPVFP.ForwardC = 0;
	sPVFP.BackwardA = 0;
	sPVFP.BackwardB = 0;
	sPVFP.BackwardC = 0;
	
	EEPROM_SAVE_PID();
}

unsigned int getPeriod(){
return spid.Period;
}

void SetPIDPeriod(float v_data){
	spid.Period = v_data;
		EEPROM_SAVE_PID();
}
void SetPIDThredHold(float v_data){
	spid.Thredhold = v_data;
		EEPROM_SAVE_PID();
}
void SetSetPoint(float v_data)
{
	spid.SetPoint =v_data; 	 
}
void SetForwardA(float v_data){
	sPVFP.ForwardA = v_data;
} 
void SetForwardB(float v_data){
	sPVFP.ForwardB = v_data;
} 
void SetForwardC(float v_data){
	sPVFP.ForwardC = v_data;
}
void SetBackwardA(float v_data){
	sPVFP.BackwardA = v_data;
}
void SetBackwardB(float v_data){
	sPVFP.BackwardB = v_data;
}
void SetBackwardC(float v_data){
	sPVFP.BackwardC = v_data;
}

void SetPWMValue(float v_data)
{
	unsigned int manuPWM =   v_data;	
	if(manuPWM >PWM_HIGH_MAX)
		manuPWM = PWM_HIGH_MAX; 
	if(manuPWM <PWM_HIGH_MIN)
		manuPWM = PWM_HIGH_MIN;  
	LoadPWM(manuPWM); 
	spid.Output = manuPWM;

}  
void SetPIDparam_P_inc(float v_data)
{
		spid.Proportion = v_data;
		//EEPROM_SAVE_PID();
} 
void SetPIDparam_I_inc(float v_data)
{

		spid.Integral   = 	 v_data;
		//EEPROM_SAVE_PID();

} 
void SetPIDparam_D_inc(float v_data)
{

		spid.Derivative  = 	 v_data;
		//EEPROM_SAVE_PID();

} 
void  SetTClose()
{
	PIDEnable = 0;
	spid.Output = PWM_HIGH_MIN;
	LoadPWM(PWM_HIGH_MIN) ;
}
void  SetTOpen()
{
	PIDEnable = 0;
	spid.Output = PWM_HIGH_MAX;
	LoadPWM(PWM_HIGH_MAX) ;
}
void  SetTPID()
{
	PIDEnable = 1;

}
unsigned char isPIDEnable(){
	return PIDEnable;
} 
unsigned int getPWMByVotage(unsigned int votage,char forBackward){
	if(forBackward>0)
		return (unsigned int)((float)sPVFP.ForwardA*votage*votage+(float)sPVFP.ForwardB*votage+(float)sPVFP.ForwardC);
	else
		return (unsigned int)((float)sPVFP.BackwardA*votage*votage+(float)sPVFP.BackwardB*votage+(float)sPVFP.BackwardC);
}

unsigned int abs( int val){
	return val>0?val:(-1*val);
}
//**********************************end of file**************************************


