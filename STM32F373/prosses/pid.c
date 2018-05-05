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
struct _PID spid;
 
uint16_t data_x,data_y,data_z;

uint32_t PWM_Output;
uint16_t Voltage_Set_Point;
extern uint32_t tocTimers[4];

uint16_t Debug[6]={0};
uint8_t debugFlag = 0;
uint8_t PID_Votage_Chanel = 0;
uint8_t PID_Ctrl_Votage_Chanel = 1;
uint8_t isRunning=0;
uint32_t pwmTemp[10];
uint8_t pwmDebugCounter = 0;
uint8_t pwmDebugIndex=0;
uint8_t pwmReveser = 0;
uint8_t pwmStateForward = 0;
uint8_t pwmStateBackward = 0;

float  eFuzzyRule[7]={-3000,-2000,-1000,0,1000,2000,3000};   
float  ecFuzzyRule[7]={-300,-200,-100,0,100,200,300};

float Kpid_calcu[3]={0.0,0.0,0.0};

float eFuzzy[]={0.0,0.0};       
float ecFuzzy[]={0.0,0.0};  

float deFuzzyFactor[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
int8_t hysteresis = 0;
int8_t hysteresisCounter = 0;
uint8_t reactHysteresis = 0;
uint8_t PWMReverse = 0;

int8_t  DefuzzyRuleMap[3][7]={
								{-3,-2,-1,0.0,1,2,3}, 
								{-3,-2,-1,0.0,1,2,3},
								{-3,-2,-1,0.0,1,2,3}}; 
int8_t  FuzzyCtrlRuleMap[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->步进电机*/
/*Row[ec]          NL 	        NM 	        NS 	        ZE        	PS       	PM 	        PL    */	
/*NL*/        {{PL,PL,PL },{PL,PL,PM },{PM,PL,PL },{PM,NM,PL },{PM,NM,PL },{PS,ZE,ZE },{ZE,ZE,NL}} ,
/*NM*/    		{{PL,PL,PM },{PL,PL,PS },{PM,PM,PM },{PM,NS,PM },{PM,NS,PM },{ZE,ZE,NS },{ZE,PS,NL}} ,
/*NS*/    		{{PL,PL,PS },{PM,PL,ZE },{PS,PS,PS },{PM,ZE,PS },{ZE,ZE,ZE },{NS,PS,NM },{NS,PS,NL}} ,
/*ZE*/    		{{PM,PL,NL },{PS,PM,NM },{PS,PL,NS },{ZE,PL,NS },{NS,PL,ZE },{NS,PM,PS },{NM,PL,PM}} ,
/*PS*/    		{{PS,PM,NL },{PS,PS,NL },{PS,PL,PS },{NS,PL,PS },{NS,PL,PS },{NS,PM,ZE },{NM,PL,PS}} ,
/*PM*/    		{{ZE,PS,NL },{ZE,ZE,NS },{ZE,NS,PM },{NS,NS,PM },{NM,PM,PM },{NM,PL,PS },{NM,PL,PM}} ,
/*PL*/    		{{ZE,ZE,NL },{NS,ZE,ZE },{NS,NM,PL },{NS,NM,PL },{NM,PL,PL },{NM,PL,PM },{NL,PL,PL}}
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
//int8_t  FuzzyCtrlRuleMap[7][7][3] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
void Set_FuzzyMap_Param(uint8_t *buf)
{
	uint8_t i = 0;
	for(i=0;i<7;i++){
		FuzzyCtrlRuleMap[buf[5]][i][0]=buf[6+3*i];
		FuzzyCtrlRuleMap[buf[5]][i][1]=buf[6+3*i+1];
		FuzzyCtrlRuleMap[buf[5]][i][2]=buf[6+3*i+2];
	}
}
void Get_FuzzyMap_Param(uint8_t *buf,uint8_t row)
{
	int offset = 5;
	uint8_t i = 0;
	buf[0] = '$';
	buf[1] = 'N';
	buf[2] = '>';//发给上位机
	buf[4] = _CMD_ReadFuzzyMap;
	for(i=0;i<7;i++){
		buf[offset+i*3] = FuzzyCtrlRuleMap[row][i][0];
		buf[offset+i*3+1] = FuzzyCtrlRuleMap[row][i][1];
		buf[offset+i*3+2] = FuzzyCtrlRuleMap[row][i][2];
		
	}
	offset+=3*7;
	buf[offset] = Get_Checksum(buf);offset+=1;
	buf[3] = offset+1;
	
}
uint16_t Get_ControlCycle(void){
	return spid.PID_ControlCycle;
}

uint16_t VirtAddVarTab[NumbOfVar] = {
	0x9A00,0x9A01,0x9A02,0x9A03,0x9A04,0x9A05,0x9A06,0x9A07,0x9A08,0x9A09,
	0x9A0A,0x9A0B,0x9A0C,0x9A0D,0x9A0F,0x9A10,0x9A11,0x9A12,0x9A13,0x9A14,
	0x9A15,0x9A16,0x9A17,0x9A18,0x9A19,0x9A1A
};//VirtAddVarTab

uint32_t Bytes2Int32_t(uint8_t * buf,uint8_t offset)
{
	return ((uint32_t)(buf[offset+3])  << 24)|((uint32_t)(buf[offset+2])  << 16)|((uint32_t)(buf[offset+1])  << 8)|buf[offset];
}
uint16_t Bytes2Int16_t(uint8_t * buf,uint8_t offset)
{
	return ( (uint16_t)(buf[offset+1])  << 8 ) | buf[offset];
}
void Int32_t2Bytes(uint32_t val,uint8_t * buf,uint8_t offset)
{
		buf[offset] =  val & 0xFF ;
		buf[offset+1] = (val >> 8) & 0xFF;
		buf[offset+2] = (val >> 16)  & 0xFF ;
		buf[offset+3] = (val >> 24) & 0xFF;
}
void Int16_t2Bytes(uint16_t val,uint8_t * buf,uint8_t offset)
{
		buf[offset] =  val & 0xFF ;
		buf[offset+1] = (val >> 8) & 0xFF;
}
void Set_PID_Param(uint8_t *buf)
{
	int offset = 5;
	spid.kpid[0]=Bytes2Int16_t(buf,offset);  offset+=2;
	spid.kpid[1]=Bytes2Int16_t(buf,offset);  offset+=2;
	spid.kpid[2]=Bytes2Int16_t(buf,offset);  offset+=2;
	
	spid.kpidF[0] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.kpidF[1] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.kpidF[2] = Bytes2Int16_t(buf,offset); offset+=2;
	
	spid.eFuzzyRule[0] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.eFuzzyRule[1] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.eFuzzyRule[2] = Bytes2Int16_t(buf,offset); offset+=2;
	
	spid.ecFuzzyRule[0] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.ecFuzzyRule[1] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.ecFuzzyRule[2] = Bytes2Int16_t(buf,offset); offset+=2;
	
	spid.PID_Cutoff = Bytes2Int16_t(buf,offset); offset+=2;
	spid.PID_ControlCycle = Bytes2Int16_t(buf,offset); offset+=2;
	spid.PID_DeadZone = Bytes2Int16_t(buf,offset); offset+=2;
	
	spid.PWM_MAX = Bytes2Int32_t(buf,offset); offset+=4;
	spid.PWM_MIN = Bytes2Int32_t(buf,offset); offset+=4;
	spid.PWM_STEP = Bytes2Int32_t(buf,offset); offset+=4;
 	
	Init_FuzzyMap();	
	//Calculate_FilteringCoefficient(spid.PID_Cutoff );
	 
	EEPROM_SAVE_PID();
}
void Init_FuzzyMap(void)
{
	  // l m s
	eFuzzyRule[0] = -1*spid.eFuzzyRule[0];
	eFuzzyRule[1] = -1*spid.eFuzzyRule[1];
	eFuzzyRule[2] = -1*spid.eFuzzyRule[2];
	eFuzzyRule[3]=0;
	eFuzzyRule[4] =  spid.eFuzzyRule[2];
	eFuzzyRule[5] =  spid.eFuzzyRule[1];
	eFuzzyRule[6] =  spid.eFuzzyRule[0];
	
	ecFuzzyRule[0] = -1*spid.ecFuzzyRule[0];
	ecFuzzyRule[1] = -1*spid.ecFuzzyRule[1];
	ecFuzzyRule[2] = -1*spid.ecFuzzyRule[2];
	ecFuzzyRule[3]=0;
	ecFuzzyRule[4] =  spid.ecFuzzyRule[2];
	ecFuzzyRule[5] =  spid.ecFuzzyRule[1];
	ecFuzzyRule[6] =  spid.ecFuzzyRule[0];
}
uint8_t Get_PID_Param(uint8_t *buf){
	int offset = 5;
	buf[0] = '$';
	buf[1] = 'N';
	buf[2] = '>';//发给上位机
	buf[4] = _CMD_GetPIDParam;
	 
  Int16_t2Bytes(spid.kpid[0], buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpid[1], buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpid[2], buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.kpidF[0], buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpidF[1], buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpidF[2], buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.eFuzzyRule[0], buf,offset);offset+=2;
	Int16_t2Bytes(spid.eFuzzyRule[1], buf,offset);offset+=2;
	Int16_t2Bytes(spid.eFuzzyRule[2], buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.ecFuzzyRule[0], buf,offset);offset+=2;
	Int16_t2Bytes(spid.ecFuzzyRule[1], buf,offset);offset+=2;
	Int16_t2Bytes(spid.ecFuzzyRule[2], buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.PID_Cutoff , buf,offset);offset+=2;
	Int16_t2Bytes(spid.PID_ControlCycle, buf,offset);offset+=2;
	Int16_t2Bytes(spid.PID_DeadZone, buf,offset);offset+=2;
	
	Int32_t2Bytes(spid.PWM_MAX  , buf,offset);offset+=4;
	Int32_t2Bytes(spid.PWM_MIN , buf,offset);offset+=4;
	Int32_t2Bytes(spid.PWM_STEP , buf,offset);offset+=4;
	
	buf[offset] = Get_Checksum(buf);offset+=1;
	buf[3] = offset+1;
	return offset+1;
} 

void Set_Running_Param(uint8_t *buf)
{
	int offset = 5;
	Voltage_Set_Point=Bytes2Int16_t(buf,offset);offset+=2;
	PWM_Mode_Config(Bytes2Int16_t(buf,offset));offset+=2;
	if (PID_isRunning()==0){
		LoadPWM(Bytes2Int32_t(buf,offset));
	}
	
}
uint8_t Get_Running_Param(uint8_t *buf)
{
	int offset = 5;
	buf[0] = '$';
	buf[1] = 'N';
	buf[2] = '>';//发给上位机
	buf[4] = _CMD_GetRunParam;

	Int16_t2Bytes(Voltage_Set_Point , buf,offset);offset+=2;
	Int32_t2Bytes(PWM_Output , buf,offset);offset+=4;
	Int16_t2Bytes(Voltage_Set_Point-spid.LastError , buf,offset);offset+=2;
	
	Int16_t2Bytes(GetADCVoltage(0)*10.f , buf,offset);offset+=2;
	Int16_t2Bytes(GetADCVoltage(1)*10.f , buf,offset);offset+=2;
	 

	
	Int16_t2Bytes(Kpid_calcu[0]*1000 +10000, buf,offset);offset+=2;
	Int16_t2Bytes(Kpid_calcu[1]*1000 +10000 , buf,offset);offset+=2;
  Int16_t2Bytes(Kpid_calcu[2]*1000 +10000 , buf,offset);offset+=2;
	
	Int32_t2Bytes(tocTimers[0] , buf,offset);offset+=4;
	Int32_t2Bytes(tocTimers[1] , buf,offset);offset+=4;
	Int32_t2Bytes(tocTimers[2] , buf,offset);offset+=4;
	Int32_t2Bytes(tocTimers[3] , buf,offset);offset+=4;
	
	Int16_t2Bytes(Debug[0], buf,offset);offset+=2;
	Int16_t2Bytes(Debug[1], buf,offset);offset+=2;
	Int16_t2Bytes(Debug[2], buf,offset);offset+=2;
	Int16_t2Bytes(Debug[3], buf,offset);offset+=2;
	Int16_t2Bytes(Debug[4], buf,offset);offset+=2;
	Int16_t2Bytes(Debug[5], buf,offset);offset+=2;
	
	buf[offset] = Get_Checksum(buf);offset+=1;
	buf[3] = offset+1;
	return offset+1;
}
/*********************************************************** 
              PID控制动作函数
 ***********************************************************/ 
void PID_Start() 		 
{ 
	Inc_PID_Calc();
	if(PWM_Output >spid.PWM_MAX)
		PWM_Output = spid.PWM_MAX; 
	if(PWM_Output <spid.PWM_MIN)
		PWM_Output = spid.PWM_MIN;  
	LoadPWM(PWM_Output);	
}
void LoadPWMTemp(uint32_t value)
{
	uint8_t i = pwmDebugIndex%10;
	int32_t delta = 0;
	pwmTemp[i]=value;
	pwmDebugCounter ++;
	if(i==0){
		delta = value-pwmTemp[9];
		pwmDebugCounter=10;
	}else{
		delta = value-pwmTemp[i-1];
	}
	if(delta<0 ){ 			
		pwmTemp[i]=value-5000;
	}
	PWM_Output=pwmTemp[i-3<=0?(i-3):(i+6)];
	LoadPWM(PWM_Output) ;
}

//增量式PID控制设计
void Inc_PID_Calc(void)
{
	register int iError;
 	register int32_t iIncpid;
	int NextPoint = GetADCVoltage(PID_Votage_Chanel);
	//当前误差
	iError = Voltage_Set_Point - NextPoint;
	Fuzzy_Kpid(iError, iError-spid.LastError) ;
	if(myabs(iError)<spid.PID_DeadZone){
	iIncpid = 0;
	}else{//在中间
		Debug[0]=10000+hysteresis;
		Debug[1]=10000+iError-spid.LastError;
		Debug[2]=10000+iError;
		Debug[3]=10000+iError-2*spid.LastError+spid.PrevError;
		
		iIncpid = (spid.kpid[0]+Kpid_calcu[0]*spid.kpidF[0]) * (iError-spid.LastError) //E[k]项
						+ (spid.kpid[1]+Kpid_calcu[1]*spid.kpidF[1]) * iError //E[k－1]项
						+ (spid.kpid[2]+Kpid_calcu[2]*spid.kpidF[2]) * (iError-2*spid.LastError+spid.PrevError); //E[k－2]项
						 						
	}
  tocTimers[3]= (100000+(iIncpid));
	//存储误差，用于下次计算
	spid.PrevError = spid.LastError;
	spid.LastError = iError;
	if(iIncpid>0){
		pwmStateForward=1;
		if(pwmStateBackward ==1){
			PWMReverse = 1;
		}	
		pwmStateBackward=0;
	}
	if(iIncpid<0){
		pwmStateBackward=1;
		if(pwmStateForward==1){
			PWMReverse = 1;
		}	
		pwmStateForward=0;
	}
 
	PWM_Output += (iIncpid/100+ hysteresis*spid.PID_Cutoff);
}
void SetHysteresis(uint8_t pe,uint8_t pec){
	hysteresis = 0; 
	if((pec == 2 || pec == 3)&&reactHysteresis==0){//ec 不变 且-eLow <e< eLow 且没响应过磁滞		
		if(pe == 0 || pe == 1 || pe == 2){//ec <0
			hysteresisCounter++;
			if(hysteresisCounter>=10){
				hysteresis= -1;
				reactHysteresis = 1;
			}
		}
		if(pe == 3 || pe == 4 || pe == 5){//ec >0
			hysteresisCounter++;
			if(hysteresisCounter>=10){
				hysteresis= 1;
				reactHysteresis = 1;
			}
		}
		
	}else{
	hysteresisCounter=0;
	}
	
	if(pec != 2 && pec != 3){                    //ec动了 为下次磁滞准备
			reactHysteresis=0;	
	}
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
	data_x = (int16_t) (spid.kpid[0]);
	data_y = (int16_t) (spid.kpid[1]);
	data_z = (int16_t) (spid.kpid[2]);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_D], data_z);
	
	data_x = (int16_t) (spid.kpidF[0]);
	data_y = (int16_t) (spid.kpidF[1]);
	data_z = (int16_t) (spid.kpidF[2]);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_PF], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_IF], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_DF], data_z);
	
	data_x = (int16_t) (spid.eFuzzyRule[0]);
	data_y = (int16_t) (spid.eFuzzyRule[1]);
	data_z = (int16_t) (spid.eFuzzyRule[2]);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_EFRL], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_EFRM], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_EFRS], data_z);
	
	data_x = (int16_t) (spid.ecFuzzyRule[0]);
	data_y = (int16_t) (spid.ecFuzzyRule[1]);
	data_z = (int16_t) (spid.ecFuzzyRule[2]);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ECFRL], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ECFRM], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_ECFRS], data_z);
	
	data_x = (int16_t) (spid.PID_Cutoff);
	data_y = (int16_t) (spid.PID_ControlCycle);
	data_z = (int16_t) (spid.PID_DeadZone);
	
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_CUTOFF_FREQ], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_CONTROL_CYCLE], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_DEADZONE], data_z);
	
	data_x = (int16_t) (spid.PWM_MAX/65535);
	data_y = (int16_t) (spid.PWM_MIN/65535);
	data_z = (int16_t) (spid.PWM_STEP/65535);
	
	EE_WriteVariable(VirtAddVarTab[EEPROM_PWM_MAX_HIGH], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PWM_MIN_HIGH], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PWM_STEP_HIGH], data_z);
	
	data_x = (int16_t) (spid.PWM_MAX%65535);
	data_y = (int16_t) (spid.PWM_MIN%65535);
	data_z = (int16_t) (spid.PWM_STEP%65535);
	
	EE_WriteVariable(VirtAddVarTab[EEPROM_PWM_MAX_LOW], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PWM_MIN_LOW], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PWM_STEP_LOW], data_z);
	
 
	
	__set_PRIMASK(0); 
}
void EEPROM_READ_PID(void)	// 
{
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_P], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_I], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_D], &data_z);
	spid.kpid[0] = (float) data_x;
	spid.kpid[1] = (float) data_y;
	spid.kpid[2] = (float) data_z;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PF], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_IF], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DF], &data_z);
	spid.kpidF[0] = (float) data_x;
	spid.kpidF[1] = (float) data_y;
	spid.kpidF[2] = (float) data_z;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_EFRL], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_EFRM], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_EFRS], &data_z);
	spid.eFuzzyRule[0] = (float) data_x;
	spid.eFuzzyRule[1] = (float) data_y;
	spid.eFuzzyRule[2] = (float) data_z;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ECFRL], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ECFRM], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_ECFRS], &data_z);
	spid.ecFuzzyRule[0] = (float) data_x;
	spid.ecFuzzyRule[1] = (float) data_y;
	spid.ecFuzzyRule[2] = (float) data_z;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_CUTOFF_FREQ], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_CONTROL_CYCLE], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DEADZONE], &data_z);
	
	spid.PID_Cutoff =   data_x;
	spid.PID_ControlCycle =   data_y;
	spid.PID_DeadZone =   data_z;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PWM_MAX_HIGH], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PWM_MIN_HIGH], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PWM_STEP_HIGH], &data_z);
	
	spid.PWM_MAX =   data_x*65535;
	spid.PWM_MIN =   data_y*65535;
	spid.PWM_STEP =   data_z*65535;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PWM_MAX_LOW], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PWM_MIN_LOW], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PWM_STEP_LOW], &data_z);
	
	spid.PWM_MAX +=   data_x;
	spid.PWM_MIN +=   data_y;
	spid.PWM_STEP +=   data_z;
 
	  
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

		num =FuzzyCtrlRuleMap[pec][pe][KpidSelect];   //主值 可取 NL,NM,LS...PS,PM,PL[0~6]	 	pec 行 pe 列		
		deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[0]; 

		num =FuzzyCtrlRuleMap[pec+1][pe][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[1];  

		num =FuzzyCtrlRuleMap[pec][pe+1][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[0]; 

		num =FuzzyCtrlRuleMap[pec+1][pe+1][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[1];  

		/********加权平均法解模糊********///DefuzzyRuleMap = [-3,-2,-1,0,1,2,3]
		temp=0;
		for(i=0;i<7;i++){
			temp+=deFuzzyFactor[i]*DefuzzyRuleMap[KpidSelect][i];      
		}

		Kpid_calcu[KpidSelect]=temp;

		//Kpid_calcu
	}
	Debug[4]=pe;
	Debug[5]=pec;
	SetHysteresis( pe, pec);

}

/************************************************
				PID函数初始化
 *************************************************/
void PID_Init() 
{ 
	 FLASH_Unlock();
	 EEPROM_INIT();// 
	 Init_FuzzyMap();
}

void PID_Param_Reset(void) 
{
	memset (&spid,0,sizeof(struct _PID)); 	// Initialize Structure 
	spid.kpid[0] = 100;
	spid.kpid[1] = 150;
	spid.kpid[2] = 50;
	
	spid.kpidF[0] = 33;
	spid.kpidF[1] = 50;
	spid.kpidF[2] = 17;
	
  spid.eFuzzyRule[0] = 2000;
	spid.eFuzzyRule[1] = 800;
	spid.eFuzzyRule[2] = 20;
	
	spid.ecFuzzyRule[0] = 300;
	spid.ecFuzzyRule[1] = 100;
	spid.ecFuzzyRule[2] = 10;
 
 	
	spid.PID_Cutoff=30000;
	spid.PID_ControlCycle=100;
	spid.PID_DeadZone=5;
	 
 
	spid.PWM_MAX=5000000;
	spid.PWM_MIN=100000;
	spid.PWM_STEP=5000;
	
	EEPROM_SAVE_PID();
}
 
void  Valve_Close(void)
{
	isRunning = 0;
	PWM_Output = spid.PWM_MIN;
	LoadPWM(PWM_Output) ;
	reactHysteresis=0;	
	//LoadPWMTemp(PWM_Output);
}
void Valve_Open(void)
{
	isRunning = 0;
	PWM_Output = spid.PWM_MAX;
	reactHysteresis=0;	
	LoadPWM(PWM_Output) ;
	//LoadPWMTemp(PWM_Output);
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
