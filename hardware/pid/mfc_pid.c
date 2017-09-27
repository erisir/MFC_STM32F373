#include "../pid/mfc_pid.h"
#include "../adc/mfc_adc.h"
#include "../../user/main.h"

#include "../pwm/mfc_pwm.h"
#include "../usart/mfc_usart1.h"
#include "string.h"
#include "math.h"

//xdata struct PID spid; // PID Control Structure
struct PID spid; // PID Control Structure
struct PWMVotageFitPara sPVFP;
unsigned int PIDVotageChanel = 1;
unsigned int PIDMode = 0;
unsigned char PIDEnable=0;
float PWMDelayRate = 0.1;
void SetPIDMode(float mode){
	PIDMode = mode;
}
void SetPIDVotageChanel(float ch){
	//PIDVotageChanel = ch;
	PWMDelayRate = ch;
}
uint32_t GetPIDOutput(){
	return spid.Output;
}

void GetPIDStatu(){
	printf("%.3f,%.3f,%.3f,%d,%d,%d,%d,%d,%d,%d\n",spid.Proportion  ,spid.Integral ,spid.Derivative  ,spid.DeadZone ,spid.SetPoint ,spid.Output,spid.LastError,spid.PrevError,spid.SetPoint-spid.LastError,spid.SumError );	
	printf("FA:%.3f,FB:%.3f,FC:%.3f,BA:%.3f,BB:%.3f,BC:%.3f\n",sPVFP.ForwardA,sPVFP.ForwardB,sPVFP.ForwardC,sPVFP.BackwardA,sPVFP.BackwardB,sPVFP.BackwardC );	
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
	Delay(abs(iIncpid)*PWMDelayRate);

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
/************************************************
				PID函数初始化
 *************************************************/
void PIDInit() 
{ 

	memset (&spid,0,sizeof(struct PID)); 	// Initialize Structure 
	memset (&sPVFP,0,sizeof(struct PWMVotageFitPara)); 	// Initialize Structure 

	spid.Proportion = 5;  
	spid.Integral =   0.00; 
	spid.Derivative =3; 
	spid.Output = 0;
	spid.SetPoint = 4000;
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
}
unsigned int getPeriod(){
return spid.Period;
}

void SetPIDPeriod(float v_data){
	spid.Period = v_data;
}
void SetPIDThredHold(float v_data){
	spid.Thredhold = v_data;
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
} 
void SetPIDparam_I_inc(float v_data)
{

		spid.Integral   = 	 v_data;

} 
void SetPIDparam_D_inc(float v_data)
{

		spid.Derivative  = 	 v_data;

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


