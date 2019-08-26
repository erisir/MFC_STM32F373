/**
  ******************************************************************************
  * @file    pid.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   pid
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#define NumbOfVar 26
#include "pid.h"
#include "pwm.h"
#include "sdadc.h"
#include "string.h"
#include <stdio.h>
#include "user_mb_app.h"

struct _PID * spid;
struct _FuzzyCtrlRuleMap *FuzzyCtrlRuleMap;//int8_t *FuzzyCtrlRuleMap[7][7][3];
uint16_t *Voltage_Set_PointCur;

uint16_t data_x,data_y,data_z;
uint32_t PWM_Output;
int16_t LastError; // Error[-1]
int16_t PrevError; // Error[-2]
int32_t SumError;



uint8_t PID_Votage_Chanel = 0;
uint8_t PID_Ctrl_Votage_Chanel = 1;
uint8_t isRunning=0;
 


float  eFuzzyRule[7]={-3000,-2000,-1000,0,1000,2000,3000};   
float  ecFuzzyRule[7]={-300,-200,-100,0,100,200,300};
float Kpid_calcu[3]={0.0,0.0,0.0};
float eFuzzy[]={0.0,0.0};       
float ecFuzzy[]={0.0,0.0};  
float deFuzzyFactor[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};


int8_t  DefuzzyRuleMap[3][7]={
								{-3,-2,-1,0.0,1,2,3}, 
								{-3,-2,-1,0.0,1,2,3},
								{-3,-2,-1,0.0,1,2,3}}; 

int8_t  FuzzyCtrlRuleMap0[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->步进电机*/
/*Row[ec]          NL 	        NM 	        NS 	        ZE        	PS       	PM 	        PL    */	
/*NL*/        {{PL,PL,PL },{PL,PL,PM },{PM,PL,PL },{PM,NM,PL },{PM,NM,PL },{PS,ZE,ZE },{ZE,ZE,NL}} ,
/*NM*/    		{{PL,PL,PM },{PL,PL,PS },{PM,PM,PM },{PM,NS,PM },{PM,NS,PM },{ZE,ZE,NS },{ZE,PS,NL}} ,
/*NS*/    		{{PL,PL,PS },{PM,PL,ZE },{PS,PS,PS },{PM,ZE,PS },{ZE,ZE,ZE },{NS,PS,NM },{NS,PS,NL}} ,
/*ZE*/    		{{PM,PL,NL },{PS,PM,NM },{PS,PL,NS },{ZE,PL,NS },{NS,PL,ZE },{NS,PM,PS },{NM,PL,PM}} ,
/*PS*/    		{{PS,PM,NL },{PS,PS,NL },{PS,PL,PS },{NS,PL,PS },{NS,PL,PS },{NS,PM,ZE },{NM,PL,PS}} ,
/*PM*/    		{{ZE,PS,NL },{ZE,ZE,NS },{ZE,NS,PM },{NS,NS,PM },{NM,PM,PM },{NM,PL,PS },{NM,PL,PM}} ,
/*PL*/    		{{ZE,ZE,NL },{NS,ZE,ZE },{NS,NM,PL },{NS,NM,PL },{NM,PL,PL },{NM,PL,PM },{NL,PL,PL}}
    		};
int8_t  FuzzyCtrlRuleMap1[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->电磁阀*/
/*Row[ec]          NL 	        NM 	        NS 	        ZE        	PS       	PM 	        PL    */	
/*NL*/        {{PL,NL,PS },{PL,NL,PS },{PM,NL,ZE },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PS,NL}} ,
/*NM*/    		{{PL,NL,NS },{PL,NL,NS },{PM,NM,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PM,NL }} ,
/*NS*/    		{{PM,NM,NL },{PM,NM,NL },{PM,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NL,PL,NL }} ,
/*ZE*/    		{{PM,NM,NL },{PS,NS,NM },{PS,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PL,NL }} ,
/*PS*/    		{{PS,NS,NL },{PS,NS,NM },{ZE,ZE,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NM,PM,PS}} ,
/*PM*/    		{{ZE,ZE,NM },{ZE,ZE,NS },{NS,PS,NS },{NM,PM,NS },{NM,PM,ZE },{NM,PL,PS },{NL,PL,PS}} ,
/*PL*/    		{{ZE,ZE,PS },{NS,ZE,PS },{NS,PS,ZE },{NM,PM,ZE },{NM,PL,ZE },{NL,PL,PL },{NL,PL,PL}}
    		};
int8_t  FuzzyCtrlRuleMap2[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->电磁阀*/
/*Row[ec]          NL 	        NM 	        NS 	        ZE        	PS       	PM 	        PL    */	
/*NL*/        {{1,2,3 },{4,5,6 },{7,8,9 },{10,11,12  },{13,14,15 },{NL,NL,NL },{NL,PS,NL}} ,
/*NM*/    		{{PL,NL,NS },{PL,NL,NS },{PM,NM,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PM,NL }} ,
/*NS*/    		{{PM,NM,NL },{PM,NM,NL },{PM,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NL,PL,NL }} ,
/*ZE*/    		{{PM,NM,NL },{PS,NS,NM },{PS,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PL,NL }} ,
/*PS*/    		{{PS,NS,NL },{PS,NS,NM },{ZE,ZE,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NM,PM,PS}} ,
/*PM*/    		{{ZE,ZE,NM },{ZE,ZE,NS },{NS,PS,NS },{NM,PM,NS },{NM,PM,ZE },{NM,PL,PS },{NL,PL,PS}} ,
/*PL*/    		{{ZE,ZE,PS },{NS,ZE,PS },{NS,PS,ZE },{NM,PM,ZE },{NM,PL,ZE },{NL,PL,PL },{NL,PL,PL}}
    		};
int8_t  FuzzyCtrlRuleMap3[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->电磁阀*/
/*Row[ec]          NL 	        NM 	        NS 	        ZE        	PS       	PM 	        PL    */	
/*NL*/        {{PL,NL,PS },{PL,NL,PS },{PM,NL,ZE },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PS,NL}} ,
/*NM*/    		{{PL,NL,NS },{PL,NL,NS },{PM,NM,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PM,NL }} ,
/*NS*/    		{{PM,NM,NL },{PM,NM,NL },{PM,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NL,PL,NL }} ,
/*ZE*/    		{{PM,NM,NL },{PS,NS,NM },{PS,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PL,NL }} ,
/*PS*/    		{{PS,NS,NL },{PS,NS,NM },{ZE,ZE,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NM,PM,PS}} ,
/*PM*/    		{{ZE,ZE,NM },{ZE,ZE,NS },{NS,PS,NS },{NM,PM,NS },{NM,PM,ZE },{NM,PL,PS },{NL,PL,PS}} ,
/*PL*/    		{{ZE,ZE,PS },{NS,ZE,PS },{NS,PS,ZE },{NM,PM,ZE },{NM,PL,ZE },{NL,PL,PL },{NL,PL,PL}}
    		};
void SetValveMode(uint8_t mode)
{
	switch(mode){
		case 1:
			Valve_Close();			
		break;
		case 2:
			Valve_Open();
		break;
		case 4:
			Valve_PID_Ctrl();
		break;
		default:
			Valve_Close();			
		break;
			
	}
}
			 
void SetContrlResource(uint8_t mode)
{
		switch(mode){
		case 1:
			Voltage_Set_PointCur=(uint16_t *)REG__HOLDINGssAddr->PIDparam;
					
		break;
		case 2:
			Voltage_Set_PointCur = &REG_INPUTsAddr->voltageCh1;	
		break;
		
		default:
			Voltage_Set_PointCur=(uint16_t *)REG__HOLDINGssAddr->PIDparam;
		break;
	}
}
uint16_t Get_ControlCycle(void){
	return spid->PID_ControlCycle;
}

uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06,0x9A07,0x9A08,0x9A09,
	0x9A0A,0x9A0B,0x9A0C,0x9A0D,0x9A0F,0x9A10,0x9A11,0x9A12,0x9A13,0x9A14,
	0x9A15,0x9A16,0x9A17,0x9A18,0x9A19,0x9A1A
};//VirtAddVarTab

 
void Set_PID_Param()//42bit
{ 	
	Init_FuzzyMap();	
	//Calculate_FilteringCoefficient(spid->PID_Cutoff ); 
	//EEPROM_SAVE_PID();
}

/*********************************************************** 
              PID控制动作函数
 ***********************************************************/ 
void PID_Start() 		 
{ 
	Inc_PID_Calc();
	if(PWM_Output >spid->PWM_MAX)
		PWM_Output = spid->PWM_MAX; 
	if(PWM_Output <spid->PWM_MIN)
		PWM_Output = spid->PWM_MIN;  
	LoadPWM(PWM_Output);	
}

//增量式PID控制设计
void Inc_PID_Calc(void)
{
	register int iError;
 	register int32_t iIncpid;
	int NextPoint = GetADCVoltage(PID_Votage_Chanel);
	//当前误差
	iError = *Voltage_Set_PointCur - NextPoint;
	Fuzzy_Kpid(iError, iError-LastError) ;
	if(myabs(iError)<spid->PID_DeadZone){
	iIncpid = 0;
	}else{//在中间
		iIncpid = (spid->kpid[0]+Kpid_calcu[0]*spid->kpidF[0]) * (iError-LastError) //E[k]项
						+ (spid->kpid[1]+Kpid_calcu[1]*spid->kpidF[1]) * iError //E[k－1]项
						+ (spid->kpid[2]+Kpid_calcu[2]*spid->kpidF[2]) * (iError-2*LastError+PrevError); //E[k－2]项
						 						
	}
	//存储误差，用于下次计算
	PrevError = LastError;
	LastError = iError;
	
	PWM_Output += (iIncpid/100);
}
 
	/***********************************************          
	 * 函数功能：PID参数Kp,Ki,Kd的计算   ********************/  

void Fuzzy_Kpid(int16_t e, int16_t ec)  
{  
	float temp;
	uint8_t num,pe,pec;
	uint8_t KpidSelect=0,i=0;		     
	/*****误差变化隶属函数描述*****/       
	if(ec<ecFuzzyRule[0]){   
		pec = 0;
		ecFuzzy[0] =1.0; //ecFuzzy[0] 属于 当前pec的程度
	}else if(ecFuzzyRule[0]<=ec && ec<ecFuzzyRule[1])  {
		pec = 0;
		ecFuzzy[0] = (ecFuzzyRule[1] - ec)/(ecFuzzyRule[1]-ecFuzzyRule[0]);   		
	}else if(ecFuzzyRule[1]<=ec && ec<ecFuzzyRule[2])  {   
		pec = 1;
		ecFuzzy[0] = (ecFuzzyRule[2] - ec)/(ecFuzzyRule[2]-ecFuzzyRule[1]);  
	}else if(ecFuzzyRule[2]<=ec && ec<ecFuzzyRule[3])  {  
		pec = 2 ; 
		ecFuzzy[0] = (ecFuzzyRule[3] - ec)/(ecFuzzyRule[3]-ecFuzzyRule[2]);   
	}else if(ecFuzzyRule[3]<=ec && ec<ecFuzzyRule[4])  {  
		pec = 3;			
		ecFuzzy[0] = (ecFuzzyRule[4] - ec)/(ecFuzzyRule[4]-ecFuzzyRule[3]);
	}else if(ecFuzzyRule[4]<=ec && ec<ecFuzzyRule[5])  {  
		pec = 4;
		ecFuzzy[0] = (ecFuzzyRule[5] - ec)/(ecFuzzyRule[5]-ecFuzzyRule[4]);         
	}else if(ecFuzzyRule[5]<=ec && ec<ecFuzzyRule[6])  {  
		pec = 5;
		ecFuzzy[0] = (ecFuzzyRule[6] - ec)/(ecFuzzyRule[6]-ecFuzzyRule[5]);         
	}else{   
		pec = 5;
		ecFuzzy[0] =0;	
	}   
	ecFuzzy[1] = (float)(1.0f - ecFuzzy[0]); 
	/*****误差隶属函数描述*****/  
	if(e<eFuzzyRule[0]) {
		pe = 0;			 						
		eFuzzy[0] =1;
	}else if(eFuzzyRule[0]<=e && e<eFuzzyRule[1])  {
		pe = 0; 
		eFuzzy[0] = (eFuzzyRule[1] - e)/(eFuzzyRule[1]-eFuzzyRule[0]);
	}else if(eFuzzyRule[1]<=e && e<eFuzzyRule[2])  {
		pe = 1; 
		eFuzzy[0] = (eFuzzyRule[2] - e)/(eFuzzyRule[2]-eFuzzyRule[1]);
	}else if(eFuzzyRule[2]<=e && e<eFuzzyRule[3])  {
		pe = 2;
		eFuzzy[0] = (eFuzzyRule[3] - e)/(eFuzzyRule[3]-eFuzzyRule[2]);
	}else if(eFuzzyRule[3]<=e && e<eFuzzyRule[4])  {
		pe = 3;
		eFuzzy[0] = (eFuzzyRule[4] - e)/(eFuzzyRule[4]-eFuzzyRule[3]);
	}else if(eFuzzyRule[4]<=e && e<eFuzzyRule[5])  {
		pe = 4; 
		eFuzzy[0] = (eFuzzyRule[5] - e)/(eFuzzyRule[5]-eFuzzyRule[4]);   
	}else if(eFuzzyRule[5]<=e && e<eFuzzyRule[6])  {
		pe = 5; 
		eFuzzy[0] = (eFuzzyRule[6] - e)/(eFuzzyRule[6]-eFuzzyRule[5]);   
	}else  { 
		pe =5;
		eFuzzy[0] =0;
	}      
	eFuzzy[1] =(float)(1.0f - eFuzzy[0]); 		
	/***********查询模糊规则表*************/   

	for(KpidSelect=0;KpidSelect<3;KpidSelect++){
		memset(deFuzzyFactor,0,7*sizeof(float));

		num =FuzzyCtrlRuleMap->data[pec][pe][KpidSelect];   //主值 可取 NL,NM,LS...PS,PM,PL[0~6]	 	pec 行 pe 列		
		deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[0]; 

		num =FuzzyCtrlRuleMap->data[pec+1][pe][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[1];  

		num =FuzzyCtrlRuleMap->data[pec][pe+1][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[0]; 

		num =FuzzyCtrlRuleMap->data[pec+1][pe+1][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[1];  

		/********加权平均法解模糊********///DefuzzyRuleMap = [-3,-2,-1,0,1,2,3]
		temp=0;
		for(i=0;i<7;i++){
			temp+=deFuzzyFactor[i]*DefuzzyRuleMap[KpidSelect][i];      
		}

		Kpid_calcu[KpidSelect]=temp;

		//Kpid_calcu
	}

}

/************************************************
				PID函数初始化
 *************************************************/
void PID_Init() 
{ 

	 SetContrlResource(1);//Digti
	 spid  = (struct _PID *)REG__HOLDINGssAddr->PIDparam;
	 FuzzyCtrlRuleMap = (struct _FuzzyCtrlRuleMap*)REG__HOLDINGssAddr->FuzzyCtrlRuleMap;
	 Init_FuzzyMap();
	 FuzzyMap_Param_Reset();
	 //PID_Param_Reset();
	 //Valve_Open();
}
void FuzzyMap_Param_Reset()
{
	memset((uint8_t *)FuzzyCtrlRuleMap->data,0,147); 	// Initialize Structure 
	memcpy(FuzzyCtrlRuleMap->data,FuzzyCtrlRuleMap2,147); 
}
void PID_Param_Reset(void) 
{
	memset ((uint8_t *)spid,0,sizeof(struct _PID)); 	// Initialize Structure 
	spid->Voltage_Set_Point=1500;
	spid->kpid[0] = 100;
	spid->kpid[1] = 150;
	spid->kpid[2] = 50;
	
	spid->kpidF[0] = 33;
	spid->kpidF[1] = 50;
	spid->kpidF[2] = 17;
	 
  spid->eFuzzyRule[0] = 2000;//
	spid->eFuzzyRule[1] = 800;
	spid->eFuzzyRule[2] = 20;
	
	spid->ecFuzzyRule[0] = 300;
	spid->ecFuzzyRule[1] = 100;
	spid->ecFuzzyRule[2] = 10;
 
 	
	spid->PID_Cutoff=30000;
	spid->PID_ControlCycle=100;
	spid->PID_DeadZone=5;

	 
	spid->PWM_MAX=5000000;
	spid->PWM_MIN=100;
	spid->PWM_STEP=5000;
	
	//EEPROM_SAVE_PID();
}
 
void Init_FuzzyMap(void)
  {
	  // l m s
	eFuzzyRule[0] = -1*spid->eFuzzyRule[0];
	eFuzzyRule[1] = -1*spid->eFuzzyRule[1];
	eFuzzyRule[2] = -1*spid->eFuzzyRule[2];
	eFuzzyRule[3]=0;
	eFuzzyRule[4] =  spid->eFuzzyRule[2];
	eFuzzyRule[5] =  spid->eFuzzyRule[1];
	eFuzzyRule[6] =  spid->eFuzzyRule[0];
	
	ecFuzzyRule[0] = -1*spid->ecFuzzyRule[0];
	ecFuzzyRule[1] = -1*spid->ecFuzzyRule[1];
	ecFuzzyRule[2] = -1*spid->ecFuzzyRule[2];
	ecFuzzyRule[3]=0;
	ecFuzzyRule[4] =  spid->ecFuzzyRule[2];
	ecFuzzyRule[5] =  spid->ecFuzzyRule[1];
	ecFuzzyRule[6] =  spid->ecFuzzyRule[0];
}

void  Valve_Close(void)
{
	isRunning = 0;
	PWM_Output = spid->PWM_MIN;
	LoadPWM(PWM_Output) ;
}
void Valve_Open(void)
{
	isRunning = 0;
	PWM_Output = spid->PWM_MAX;
	LoadPWM(PWM_Output) ;
}
void Valve_PID_Ctrl()
{
	isRunning = 1;
}
uint8_t PID_isRunning(void){
	return isRunning;
} 
uint16_t myabs( int val){
	return val>0?val:(-1*val);
}
//**********************************end of file**************************************