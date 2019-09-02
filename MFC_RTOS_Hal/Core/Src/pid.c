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
 
#include "pid.h"

struct _PID * spid;
struct _CALVALUE * scalValue;
struct _CALVALUE *  scalibrateTable;
struct _FuzzyCtrlRuleMap *FuzzyCtrlRuleMap;//int8_t *FuzzyCtrlRuleMap[7][7][3];
uint16_t *Voltage_Set_PointCur;
uint16_t Voltage_Set_Point;
uint16_t data_x,data_y,data_z;
uint32_t PWM_Output;
int16_t LastError; // Error[-1]
int16_t PrevError; // Error[-2]
int32_t SumError;



uint8_t PID_Votage_Chanel = 0;
uint8_t PID_Ctrl_Votage_Chanel = 1;
uint8_t isRunning=0;
 
uint16_t VirtAddVarTab[NB_OF_VARIABLES];//VirtAddVarTab

float  eFuzzyRule[7]={-3000,-2000,-1000,0,1000,2000,3000};   
float  ecFuzzyRule[7]={-300,-200,-100,0,100,200,300};
float Kpid_calcu[3]={0.0,0.0,0.0};
float eFuzzy[]={0.0,0.0};       
float ecFuzzy[]={0.0,0.0};  
float deFuzzyFactor[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
uint16_t calibrateTable[]={0,10*50,20*50,30*50,40*50,50*50,60*50,70*50,80*50,90*50,100*50};

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
/*NL*/        {{PL,NL,PS },{PL,NL,PS },{PM,NL,ZE },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PS,NL}} ,
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


void EEPROM_INIT(void)// 
{
	/* Unlock the Flash Program Erase controller */
  for (int VarValue = 0; VarValue < NB_OF_VARIABLES; VarValue++)
  {
    VirtAddVarTab[VarValue] = (uint16_t)(VarValue + 1);
  }

	HAL_FLASH_Unlock();
  EE_Init(VirtAddVarTab, EE_FORCED_ERASE);
	HAL_FLASH_Lock();
}
/************************************************
				PID函数初始化
 *************************************************/
				
void PID_Init() 
{ 

	 SetContrlResource(1);//默认设定信号为数字控制
	//初始化holdingaddress	
	 spid  = (struct _PID *)REG__HOLDINGssAddr->PIDparam;
	 FuzzyCtrlRuleMap = (struct _FuzzyCtrlRuleMap*)REG__HOLDINGssAddr->FuzzyCtrlRuleMap;
	 scalValue = (struct _CALVALUE*)REG__HOLDINGssAddr->stdFlowValue;
	 scalibrateTable = (struct _CALVALUE*)calibrateTable;
	
	 FuzzyMapInit(2);
	 EEPROM_INIT();
	 HAL_FLASH_Unlock();
	
	 EE_ReadVariable16bits(VirtAddVarTab[EEPROM_SUM], &data_x);
	 if(data_x != EEPROM_SUM)//第一次开机，使用默认的PID参数而不是用户自定义的，自定义的会保存在Rom内
	 {
			EE_WriteVariable16bits(VirtAddVarTab[EEPROM_SUM], EEPROM_SUM);
			PIDStructInit();
			EEPROM_SAVE_PID();
			CalValueStructInit();
			EEPROM_SAVE_Correct();
	 }else{//否则，从ROM中读取pid数据
		 	EEPROM_READ_PID();
		 	EEPROM_READ_Correct();
	 }
	 HAL_FLASH_Lock();

	 FuzzyRuleInit();//依赖PID
}
void FuzzyRuleInit(void)
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
void FuzzyMapInit(uint8_t mapId)
{
	memset((uint8_t *)FuzzyCtrlRuleMap->data,0,147); 	// Initialize Structure 
	switch(mapId){
		case 0:
			memcpy(FuzzyCtrlRuleMap->data,FuzzyCtrlRuleMap0,147); 
			break;
		case 1:
			memcpy(FuzzyCtrlRuleMap->data,FuzzyCtrlRuleMap1,147); 
			break;
		case 2:
			memcpy(FuzzyCtrlRuleMap->data,FuzzyCtrlRuleMap2,147); 
			break;
		case 3:
			memcpy(FuzzyCtrlRuleMap->data,FuzzyCtrlRuleMap3,147); 
			break;
		default:
			memcpy(FuzzyCtrlRuleMap->data,FuzzyCtrlRuleMap2,147); 
			break;
	}
}

void CalValueStructInit(void) 
{	
	scalValue->value[0] = 0;
	/*scalValue->value[1] = 4.2*50;
	scalValue->value[2] = 16.2*50;
	scalValue->value[3] = 27.4*50;
	scalValue->value[4] = 39.3*50;
	scalValue->value[5] = 49.9*50;
	scalValue->value[6] = 61.3*50;
	scalValue->value[7] = 71.3*50;
	scalValue->value[8] = 82*50;
	scalValue->value[9] = 93*50;
	scalValue->value[10] = 104*50;*/
	scalValue->value[1] = 5.4*50;//500sccm
	scalValue->value[2] = 16.6*50;
	scalValue->value[3] = 27.8*50;
	scalValue->value[4] = 38.8*50;
	scalValue->value[5] = 49.6*50;
	scalValue->value[6] = 60.2*50;
	scalValue->value[7] = 70.4*50;
	scalValue->value[8] = 80.4*50;
	scalValue->value[9] = 90*50;
	scalValue->value[10] = 99.6*50;
}
void PIDStructInit(void) 
{	
	spid->Voltage_Set_Point=1500;

	spid->kpid[0] = 20;//for 500sccm
	spid->kpid[1] = 30;
	spid->kpid[2] = 20;
	
	spid->kpidF[0] = 2;
	spid->kpidF[1] = 2;
	spid->kpidF[2] = 2;

  spid->eFuzzyRule[0] = 2000;//
	spid->eFuzzyRule[1] = 800;
	spid->eFuzzyRule[2] = 20;
	
	spid->ecFuzzyRule[0] = 300;
	spid->ecFuzzyRule[1] = 100;
	spid->ecFuzzyRule[2] = 10;
 
 	
	spid->PID_Cutoff=3000;
	spid->PID_ControlCycle=20;
	spid->PID_DeadZone=2;
	
 
	spid->PWM_MAX=1000000;
	spid->PWM_MIN=900000;
	spid->PWM_STEP=5000;

}

void Set_PID_Param()//42bit 被modbus调用
{ 	
	FuzzyRuleInit();	
	//Calculate_FilteringCoefficient(spid->PID_Cutoff ); 
	EEPROM_SAVE_PID();
}
void Set_Correct_Param()//42bit 被modbus调用
{ 	
	//Calculate_FilteringCoefficient(spid->PID_Cutoff ); 
	EEPROM_SAVE_Correct();
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
	REG_INPUTsAddr->DEBUG32[0] = PWM_Output;
	LoadPWM(PWM_Output);	
}

//增量式PID控制设计
void Inc_PID_Calc(void)
{
	register int iError;
 	register int32_t iIncpid;
	int NextPoint = GetADCVoltage(PID_Votage_Chanel);//max(NextPoint)=5000/5
	//当前误差
	iError =  Voltage_Set_Point- NextPoint;
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

	}

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


void EEPROM_SAVE_Correct(void)	 
{
	 
	HAL_FLASH_Unlock();

	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_0], scalValue->value[0]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_10], scalValue->value[1]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_20], scalValue->value[2]);
	

	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_30], scalValue->value[3]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_40], scalValue->value[4]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_50], scalValue->value[5]);

	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_60], scalValue->value[6]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_70], scalValue->value[7]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_80], scalValue->value[8]);
	
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_90], scalValue->value[9]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_100], scalValue->value[10]);
		
	HAL_FLASH_Lock();
}
void EEPROM_READ_Correct(void)	// 
{
 
	HAL_FLASH_Unlock();
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_0], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_10], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_20], &data_z);
	
	scalValue->value[0] = (float) data_x;
	scalValue->value[1] = (float) data_y;
	scalValue->value[2] = (float) data_z;
	 
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_30], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_40], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_50], &data_z);
	
	scalValue->value[3] = (float) data_x;
	scalValue->value[4] = (float) data_y;
	scalValue->value[5] = (float) data_z;
	
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_60], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_70], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_80], &data_z);
	scalValue->value[6] = (float) data_x;
	scalValue->value[7] = (float) data_y;
	scalValue->value[8] = (float) data_z;
	
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_90], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_CAL_VAL_100], &data_y);
	scalValue->value[9] = (float) data_x;
	scalValue->value[10] = (float) data_y;
	
  HAL_FLASH_Lock();
	  
}
void EEPROM_SAVE_PID(void)	 
{
	 
	HAL_FLASH_Unlock();
	data_x = (int16_t) (spid->kpid[0]);
	data_y = (int16_t) (spid->kpid[1]);
	data_z = (int16_t) (spid->kpid[2]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_P], data_x);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_I], data_y);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_D], data_z);
	
	data_x = (int16_t) (spid->kpidF[0]);
	data_y = (int16_t) (spid->kpidF[1]);
	data_z = (int16_t) (spid->kpidF[2]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_PF], data_x);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_IF], data_y);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_DF], data_z);
	
	data_x = (int16_t) (spid->eFuzzyRule[0]);
	data_y = (int16_t) (spid->eFuzzyRule[1]);
	data_z = (int16_t) (spid->eFuzzyRule[2]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_EFRL], data_x);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_EFRM], data_y);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_EFRS], data_z);
	
	data_x = (int16_t) (spid->ecFuzzyRule[0]);
	data_y = (int16_t) (spid->ecFuzzyRule[1]);
	data_z = (int16_t) (spid->ecFuzzyRule[2]);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_ECFRL], data_x);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_ECFRM], data_y);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_ECFRS], data_z);
	
	data_x = (int16_t) (spid->PID_Cutoff);
	data_y = (int16_t) (spid->PID_ControlCycle);
	data_z = (int16_t) (spid->PID_DeadZone);
	
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_CUTOFF_FREQ], data_x);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_CONTROL_CYCLE], data_y);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PID_DEADZONE], data_z);
	
	data_x = (int16_t) (spid->PWM_MAX/65535);
	data_y = (int16_t) (spid->PWM_MIN/65535);
	data_z = (int16_t) (spid->PWM_STEP/65535);
	
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PWM_MAX_HIGH], data_x);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PWM_MIN_HIGH], data_y);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PWM_STEP_HIGH], data_z);
	
	data_x = (int16_t) (spid->PWM_MAX%65535);
	data_y = (int16_t) (spid->PWM_MIN%65535);
	data_z = (int16_t) (spid->PWM_STEP%65535);
	
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PWM_MAX_LOW], data_x);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PWM_MIN_LOW], data_y);
	EE_WriteVariable16bits(VirtAddVarTab[EEPROM_PWM_STEP_LOW], data_z);
	
	HAL_FLASH_Lock();
}
void EEPROM_READ_PID(void)	// 
{
 
	HAL_FLASH_Unlock();
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_P], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_I], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_D], &data_z);
	
	spid->kpid[0] = (float) data_x;
	spid->kpid[1] = (float) data_y;
	spid->kpid[2] = (float) data_z;
	 
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_PF], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_IF], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_DF], &data_z);
	spid->kpidF[0] = (float) data_x;
	spid->kpidF[1] = (float) data_y;
	spid->kpidF[2] = (float) data_z;
	
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_EFRL], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_EFRM], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_EFRS], &data_z);
	spid->eFuzzyRule[0] = (float) data_x;
	spid->eFuzzyRule[1] = (float) data_y;
	spid->eFuzzyRule[2] = (float) data_z;
	
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_ECFRL], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_ECFRM], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_ECFRS], &data_z);
	spid->ecFuzzyRule[0] = (float) data_x;
	spid->ecFuzzyRule[1] = (float) data_y;
	spid->ecFuzzyRule[2] = (float) data_z;
	
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_CUTOFF_FREQ], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_CONTROL_CYCLE], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PID_DEADZONE], &data_z);
	
	spid->PID_Cutoff =   data_x;
	spid->PID_ControlCycle =   data_y;
	spid->PID_DeadZone =   data_z;
	
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PWM_MAX_HIGH], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PWM_MIN_HIGH], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PWM_STEP_HIGH], &data_z);
	
	spid->PWM_MAX =   data_x*65535;
	spid->PWM_MIN =   data_y*65535;
	spid->PWM_STEP =   data_z*65535;
	
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PWM_MAX_LOW], &data_x);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PWM_MIN_LOW], &data_y);
	EE_ReadVariable16bits(VirtAddVarTab[EEPROM_PWM_STEP_LOW], &data_z);
	
	spid->PWM_MAX +=   data_x;
	spid->PWM_MIN +=   data_y;
	spid->PWM_STEP +=   data_z;
  HAL_FLASH_Lock();
	  
}

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

uint16_t piecewiselinearinterp(struct _CALVALUE * xDict,struct _CALVALUE * yDict,uint16_t DictSize,uint16_t xInput)
{
 
	uint16_t x1,x2,y1,y2,i=0,yOutput=0;
  if(xInput<=xDict->value[0]){
		yOutput= yDict->value[0];
	}else if(xInput>=xDict->value[DictSize-1]){
		yOutput= yDict->value[DictSize-1];
	}else{
		for(i=0;i<DictSize-1;i++){
			if( (xInput>=xDict->value[i]) && (xInput<xDict->value[i+1])){
				x1 = xDict->value[i];
				x2 = xDict->value[i+1];
				y1 = yDict->value[i];
				y2 = yDict->value[i+1];
				yOutput = (xInput-x2)*y1/(x1-x2)+(xInput-x1)*y2/(x2-x1);
				break;
			}
		}
	}

	return yOutput;
}
void VoltageOutLinerFix(void)
{
	uint16_t currVoltage,outputVoltage;
	currVoltage= GetADCVoltage(PID_Votage_Chanel);
	outputVoltage=VoltageToFlow(currVoltage);
	
	REG_INPUTsAddr->DEBUG16[0]= currVoltage;
	REG_INPUTsAddr->DEBUG16[1]= outputVoltage;
	
	AD5761_SetVoltage(outputVoltage);
}
void PIDSetPointChange()
{
	uint16_t temp = *Voltage_Set_PointCur;
	Voltage_Set_Point = FlowToVoltage(temp);
	REG_INPUTsAddr->DEBUG16[2]= temp;
	REG_INPUTsAddr->DEBUG16[3]= Voltage_Set_Point;
	
	REG_INPUTsAddr->DEBUG16[4]=scalValue->value[1];
	REG_INPUTsAddr->DEBUG16[5]=scalibrateTable->value[1];
}
uint16_t VoltageToFlow(uint16_t voltage) 
{
	return piecewiselinearinterp(scalibrateTable,scalValue,11,voltage);
	//AD5761_SetVotage(vout);
}
uint16_t FlowToVoltage(uint16_t flow)//real Target voltage cover to current MFC sensor Voltage
{
	//uint16_t voltage = 0;
	//scalValue->value[0];
	//voltage = (uint16_t)(13.0f*filter_voltage.ch0);
	return piecewiselinearinterp(scalValue,scalibrateTable,11,flow);
  //AD5761_SetVotage(voltage);
}
//**********************************end of file**************************************
