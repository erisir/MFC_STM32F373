#include "pid.h"
#include "sdadc.h"
#include "delay.h" 
#include <stdio.h>
#include "pwm.h"
#include "eeprom.h"
#include "rs485.h"
#include "tasks.h"
#include "string.h"
#include "math.h"
#include "protocol.h"
 

//xdata struct PID spid; // PID Control Structure
struct _PID spid;
 
uint16_t data_x,data_y,data_z;

float  responce_time=0.5;
float  freq_Cutoff=50;

uint32_t PWM_Output;

uint16_t Voltage_Set_Point;
uint16_t Voltage_Set_Point_temp;
uint16_t feedbackTime = 50;

uint8_t PID_Votage_Chanel = 1;
uint8_t PID_Ctrl_Votage_Chanel = 1;
uint8_t isRunning=0;
uint8_t set_point_changed= 0;

uint8_t PID_Ctrl_Mode = 0;//0 soft 1:Ext FlowSet

uint16_t getFeedBackTime(void){
	return feedbackTime;
}
void PID_Setpoint_Change(){
  static  int16_t counter=0;
	static char setPointIncreasing=0;
	static int16_t voltageStart = 0;
	int16_t error = 0;
	int16_t currVoltage = GetADCVoltage(PID_Votage_Chanel);
	int16_t CtrlVoltage = GetADCVoltage(PID_Ctrl_Votage_Chanel);
	
	if(PID_Ctrl_Mode ==0){
		error=Voltage_Set_Point-currVoltage;
	}
	if(PID_Ctrl_Mode ==1){
		error=CtrlVoltage-currVoltage;
		Voltage_Set_Point = CtrlVoltage;
	}
	
	if(abs(error)>spid.error_Low_Threadhold && setPointIncreasing==0){//从头计时
		setPointIncreasing = 1;
		counter = 0;
		voltageStart = currVoltage+error/4;//从三分之一开始走起
	}else{//差值小
		Voltage_Set_Point_temp= Voltage_Set_Point;
	}
	if(setPointIncreasing == 1){
		Voltage_Set_Point_temp = voltageStart+(Voltage_Set_Point-voltageStart)*(1-exp(-1*counter*0.01f/responce_time));	 
		counter++;
	}
	if(setPointIncreasing == 1 && counter==500)//abs(error)<spid.error_Low_Threadhold-spid.error_Low_Threadhold/2){
	{
		Voltage_Set_Point_temp = Voltage_Set_Point;	 
		counter=0;
		setPointIncreasing = 0;
	}
}
uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06,0x9A07,0x9A08,0x9A09,
	0x9A0A,0x9A0B,0x9A0C,0x9A0D,0x9A0F,0x9A10,0x9A11,0x9A12,0x9A13,0x9A14
};//VirtAddVarTab


void Set_PID_Param(uint8_t *buf)
{
	int offset = 5;
	data_x = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_y = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_z = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];	
	spid.kpH=data_x/1000.0f; 
	spid.kiH=data_y/1000.0f; 
	spid.kdH=data_z/1000.0f; 
	
	offset+=2;
	data_x = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_y = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_z = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];	
 	
	spid.kpM=data_x/1000.0f; 
	spid.kiM=data_y/1000.0f; 
	spid.kdM=data_z/1000.0f; 
	
	offset+=2;
	data_x = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_y = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_z = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];	
 	
	spid.kpL=data_x/1000.0f; 
	spid.kiL=data_y/1000.0f; 
	spid.kdL=data_z/1000.0f; 
	
	offset+=2;
	data_x = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_y = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_z = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];	
	
	spid.error_High_Threadhold=data_x;
	spid.error_Low_Threadhold=data_y;
	spid.PWM_Change_Threadhold=data_z;
	
	offset+=2;
	data_x = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_y = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_z = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];	
	
	spid.deadzone = data_x;		
	responce_time = (float)(data_y/1000.f);
	freq_Cutoff = (float)(data_z/1000.f);
	
	offset+=2;
	data_x = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];

	PID_Ctrl_Mode = data_x;
	
	Calculate_FilteringCoefficient(freq_Cutoff);
	 
	EEPROM_SAVE_PID();
}
void Get_PID_Param(uint8_t *buf){
	int offset = 5;
	buf[0] = '$';
	buf[1] = 'N';
	buf[2] = '>';//发给上位机
	
	buf[4] = _CMD_GetPIDParam;
	
	data_x = (int16_t)(spid.kpH*1000);
	data_y = (int16_t)(spid.kiH*1000);
	data_z = (int16_t)(spid.kdH*1000);
	
	buf[offset] =  data_x & 0xFF ;
	buf[offset+1] = (data_x >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_y & 0xFF ;
	buf[offset+1] = (data_y >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_z & 0xFF ;
	buf[offset+1] = (data_z >> 8) & 0xFF;
	offset+=2;
	
	data_x = (int16_t)(spid.kpM*1000);
	data_y = (int16_t)(spid.kiM*1000);
	data_z = (int16_t)(spid.kdM*1000);
	
	buf[offset] =  data_x & 0xFF ;
	buf[offset+1] = (data_x >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_y & 0xFF ;
	buf[offset+1] = (data_y >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_z & 0xFF ;
	buf[offset+1] = (data_z >> 8) & 0xFF;
	offset+=2;
	
		
	data_x = (int16_t)(spid.kpL*1000);
	data_y = (int16_t)(spid.kiL*1000);
	data_z = (int16_t)(spid.kdL*1000);
	
	buf[offset] =  data_x & 0xFF ;
	buf[offset+1] = (data_x >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_y & 0xFF ;
	buf[offset+1] = (data_y >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_z & 0xFF ;
	buf[offset+1] = (data_z >> 8) & 0xFF;
	offset+=2;
	
	data_x = (int16_t)spid.error_High_Threadhold;
	data_y = (int16_t)spid.error_Low_Threadhold;
	data_z = (int16_t)spid.PWM_Change_Threadhold;
	
	buf[offset] =  data_x & 0xFF ;
	buf[offset+1] = (data_x >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_y & 0xFF ;
	buf[offset+1] = (data_y >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_z & 0xFF ;
	buf[offset+1] = (data_z >> 8) & 0xFF;
	offset+=2;
	
	data_x = (int16_t)spid.deadzone;
	data_y = (int16_t)(responce_time*1000);
	data_z = (int16_t)(freq_Cutoff*1000);		
	
	buf[offset] =  data_x & 0xFF ;
	buf[offset+1] = (data_x >> 8) & 0xFF;
	offset+=2;	
	buf[offset] =  data_y & 0xFF ;
	buf[offset+1] = (data_y >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_z & 0xFF ;
	buf[offset+1] = (data_z >> 8) & 0xFF;
	offset+=2;
	
	buf[offset+1] = Get_Checksum(buf);
	buf[3] = offset;
} 

void Set_Running_Param(uint8_t *buf)
{
	int offset = 5;
	data_x = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	offset+=2;
	data_y = ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
	
 
	feedbackTime = data_x; 
	if(isRunning==0){
		PWM_Output=data_x; 
		LoadPWM(PWM_Output);
	}

	if(PID_Ctrl_Mode==0){
		Voltage_Set_Point=data_y; 	
	}
}
void Get_Running_Param(uint8_t *buf)
{
	int offset = 5;
  uint32_t pwm ;
	buf[0] = '$';
	buf[1] = 'N';
	buf[2] = '>';//发给上位机
	
	buf[4] = _CMD_GetRunParam;
	
	data_x = (int16_t)(Voltage_Set_Point);
	pwm = (uint32_t)(PWM_Output);
	data_z = (int16_t)(Voltage_Set_Point-spid.LastError);//lastVoltage
	
	buf[offset] =  data_x & 0xFF ;
	buf[offset+1] = (data_x >> 8) & 0xFF;
	offset+=2;
	
	buf[offset] =  pwm & 0xFF ;
	buf[offset+1] = (pwm >> 8) & 0xFF;
	buf[offset+2] = (pwm >> 16)  & 0xFF ;
	buf[offset+3] = (pwm >> 24) & 0xFF;
	
	offset+=4;
	buf[offset] =  data_z & 0xFF ;
	buf[offset+1] = (data_z >> 8) & 0xFF;
	offset+=2;
			
	data_x = (int16_t)(GetADCVoltage(0)*10);
	data_y = (int16_t)(GetADCVoltage(1)*10);		 
	data_z = Voltage_Set_Point_temp;
	 
	
 	buf[offset] =  data_x & 0xFF ;
	buf[offset+1] = (data_x >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_y & 0xFF ;
	buf[offset+1] = (data_y >> 8) & 0xFF;
	offset+=2;
	buf[offset] =  data_z & 0xFF ;
	buf[offset+1] = (data_z >> 8) & 0xFF;
	offset+=2;
	
	//buf[offset+1] = Get_Checksum(buf);
	buf[3] = offset;
}
/*********************************************************** 
              PID温度控制动作函数
 ***********************************************************/ 
void PID_Start() 		 
{ 
  Inc_PID_Calc();
	
	if(PWM_Output >PWM_HIGH_MAX)
		PWM_Output = PWM_HIGH_MAX; 
	if(PWM_Output <PWM_HIGH_MIN)
		PWM_Output = PWM_HIGH_MIN;  
		
	LoadPWM(PWM_Output) ;
}

//增量式PID控制设计
void Inc_PID_Calc(void)
{
	register float iError;
 	register int iIncpid;
	float NextPoint = GetADCVoltage(PID_Votage_Chanel);
	//当前误差
	iError = Voltage_Set_Point_temp - NextPoint;
	if(abs(iError)<spid.deadzone){
	iIncpid = 0;
	}else if(abs(iError) >spid.error_High_Threadhold){
	 //增量计算
	iIncpid = spid.kpH * iError //E[k]项
					- spid.kiH * spid.LastError //E[k－1]项
					+ spid.kdH * spid.PrevError; //E[k－2]项
	
	}else if(abs(iError) <spid.error_High_Threadhold){
	 	iIncpid = spid.kpL * iError //E[k]项
						- spid.kiL * spid.LastError //E[k－1]项
						+ spid.kdL * spid.PrevError; //E[k－2]项
	}else if(abs(iError) >spid.error_Low_Threadhold && abs(iError) <spid.error_High_Threadhold){
	 	iIncpid = spid.kpM * iError //E[k]项
						- spid.kiM * spid.LastError //E[k－1]项
						+ spid.kdM * spid.PrevError; //E[k－2]项
	}
	//存储误差，用于下次计算
	spid.PrevError = spid.LastError;
	spid.LastError = iError;
	//返回增量值
		if(abs(iIncpid) >spid.PWM_Change_Threadhold){
		iIncpid = iIncpid>0?spid.PWM_Change_Threadhold:(-1*spid.PWM_Change_Threadhold);
	}
	PWM_Output += iIncpid;
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
		PID_Param_Reset();// 
	}
}
void EEPROM_SAVE_PID(void)	 
{
	 __set_PRIMASK(1); 
	data_x = (int16_t) (spid.kpH*1000);
	data_y = (int16_t) (spid.kiH*1000);
	data_z = (int16_t) (spid.kdH*1000);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PH], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_IH], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_DH], data_z);
	
	data_x = (int16_t) (spid.kpM*1000);
	data_y = (int16_t) (spid.kiM*1000);
	data_z = (int16_t) (spid.kdM*1000);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PM], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_IM], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_DM], data_z);
	
	data_x = (int16_t) (spid.kpL*1000);
	data_y = (int16_t) (spid.kiL*1000);
	data_z = (int16_t) (spid.kdL*1000);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PL], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_IL], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_DL], data_z);
	
	data_x = (int16_t) (spid.error_High_Threadhold);
	data_y = (int16_t) (spid.error_Low_Threadhold);
	data_z = (int16_t) (spid.PWM_Change_Threadhold);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_THD_H], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_THD_L], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_THD_PWM], data_z);
	
	data_x = (int16_t) (spid.deadzone);
	data_y = (int16_t) (responce_time*1000);
	data_z = (int16_t) (freq_Cutoff*1000);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_DEADZONE], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_RUN_RES_TIME], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_RUN_FREQ_CUTOFF], data_z);
 

	__set_PRIMASK(0); 
}
void EEPROM_READ_PID(void)	// 
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PH], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_IH], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DH], &data_z);
	spid.kpH = (float) data_x/1000.0f;
	spid.kiH = (float) data_y/1000.0f;
	spid.kdH = (float) data_z/1000.0f;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PM], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_IM], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DM], &data_z);
	spid.kpM = (float) data_x/1000.0f;
	spid.kiM = (float) data_y/1000.0f;
	spid.kdM = (float) data_z/1000.0f;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PL], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_IL], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DL], &data_z);
	spid.kpL = (float) data_x/1000.0f;
	spid.kiL = (float) data_y/1000.0f;
	spid.kdL = (float) data_z/1000.0f;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_THD_H], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_THD_L], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_THD_PWM], &data_z);
	spid.error_High_Threadhold =   data_x;
	spid.error_Low_Threadhold =   data_y;
	spid.PWM_Change_Threadhold =   data_z;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DEADZONE], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_RUN_RES_TIME], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_RUN_FREQ_CUTOFF], &data_z);
	spid.deadzone =   data_x;
	responce_time =   data_y/1000.f;
	freq_Cutoff =   data_z/1000.f;
	  
}
/************************************************
				PID函数初始化
 *************************************************/
void PID_Init() 
{ 
	 FLASH_Unlock();
	 EEPROM_INIT();// 
}

void PID_Param_Reset(void) 
{
	memset (&spid,0,sizeof(struct _PID)); 	// Initialize Structure 
	
	spid.kpH=10*2.45f; 
	spid.kiH=10*3.5f; 
	spid.kdH=10*1.25f; 
 	
	spid.kpM=5*2.45f;  
	spid.kiM=5*3.5f; 
	spid.kdM=5*1.25f; 
	
	spid.kpL=2*2.45f; 
	spid.kiL=2*3.5f; 
	spid.kdL=2*1.25f; 
	
	spid.error_High_Threadhold=3000;
	spid.error_Low_Threadhold=500;
	spid.PWM_Change_Threadhold=1000;
	
	spid.deadzone=50;

 
	EEPROM_SAVE_PID();
}
 
void  Valve_Close(void)
{
	isRunning = 0;
	PWM_Output = PWM_HIGH_MIN;
	LoadPWM(PWM_Output) ;
}
void Valve_Open(void)
{
	isRunning = 0;
	PWM_Output = PWM_HIGH_MAX;
	LoadPWM(PWM_HIGH_MAX) ;
}
void Valve_PID_Ctrl()
{
	isRunning = 1;
}
unsigned char is_PID_Running(void){
	return isRunning;
} 
unsigned int abs( int val){
	return val>0?val:(-1*val);
}
//**********************************end of file**************************************
