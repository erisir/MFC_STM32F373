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


uint8_t PID_Votage_Chanel = 0;
uint8_t PID_Ctrl_Votage_Chanel = 1;
uint8_t isRunning=0;

int16_t  eFuzzyRule[7]={-3000,-2000,-1000,0.0,1000,2000,3000};   
int16_t  ecFuzzyRule[7]={-300,-200,-100,0.0,100,200,300};

float Kpid_calcu[3]={0,0,0};

float eFuzzy[]={0.0,0.0};       
float ecFuzzy[]={0.0,0.0};  

float deFuzzyFactor[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	
int8_t  DefuzzyRuleMap[3][7]={
								{-3,-2,-1,0.0,1.0,2.0,3.0}, 
								{-3,-2,-1,0.0,1.0,2.0,3.0},
								{-3,-2,-1,0.0,1.0,2.0,3.0}}; 
int8_t  FuzzyCtrlRuleMap0[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->*/
/*Row[ec]*/	
        {{PL,PM,PS },{PL,PL,PS },{PM,PL,ZE },{PM,PL,ZE },{PS,PL,ZE },{ZE,PL,PS },{ZE,PM,PS}} ,
    		{{PL,PL,NL },{PL,PS,NL },{PM,PS,NM },{PS,PS,NS },{PS,PS,NS },{ZE,PS,NM },{ZE,PS,NM}} ,
    		{{PL,ZE,NL },{PL,ZE,NL },{PM,ZE,NM },{ZE,ZE,NS },{ZE,ZE,NM },{NS,NS,NM },{NS,NS,NM}} ,
    		{{PL,NS,NS },{PM,NS,NS },{PS,NS,NS },{NS,NS,NS },{NM,NS,NS },{NM,NS,ZE },{NM,ZE,NM}} ,
    		{{PM,ZE,NL },{PS,ZE,NL },{ZE,ZE,NM },{NM,ZE,NS },{NM,ZE,NM },{NM,NS,ZE },{NM,ZE,NM}} ,
    		{{ZE,PL,NL },{ZE,PS,NL },{NS,PL,ZE },{NM,PS,NS },{NL,PS,NM },{NM,ZE,ZE },{NM,PS,NL}} ,
    		{{ZE,PM,PS },{ZE,PM,PS },{NS,PL,ZE },{NM,PL,ZE },{NM,PL,ZE },{NM,PS,PS },{NM,PS,NL}}
    		};
int8_t  FuzzyCtrlRuleMap[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->*/
/*Row[ec]*/	
        {{PL,PM,PS },{PL,PL,PS },{PM,PL,ZE },{PM,PL,ZE },{PS,PL,ZE },{ZE,PL,PS },{ZE,PM,PS}} ,
    		{{PL,PL,NL },{PL,PS,NL },{PM,PS,NM },{PS,PS,NS },{PS,PS,NS },{ZE,PS,NM },{ZE,PS,NM}} ,
    		{{PL,ZE,NL },{PL,ZE,NL },{PM,ZE,NM },{ZE,ZE,NS },{ZE,ZE,NM },{NS,NS,NM },{NS,NS,NM}} ,
    		{{PL,NS,NS },{PM,NS,NS },{PS,NS,NS },{NS,NS,NS },{NM,NS,NS },{NM,NS,ZE },{NM,ZE,NM}} ,
    		{{PM,ZE,NM },{PS,ZE,NM },{ZE,ZE,NM },{NM,ZE,NS },{NM,ZE,NM },{NM,NS,ZE },{NM,ZE,NM}} ,
    		{{ZE,PS,NM },{ZE,PS,NM },{NS,PS,ZE },{NM,PS,NS },{NL,PS,NM },{NM,ZE,ZE },{NM,PS,NL}} ,
    		{{ZE,PM,PS },{ZE,PM,PS },{NS,PM,ZE },{NM,PL,ZE },{NM,PL,ZE },{NM,PS,PS },{NM,PS,NL}}
    		};

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
	spid.kpid[0]=Bytes2Int16_t(buf,offset)/100.0f;  offset+=2;
	spid.kpid[1]=Bytes2Int16_t(buf,offset)/100.0f;  offset+=2;
	spid.kpid[2]=Bytes2Int16_t(buf,offset)/100.0f;  offset+=2;
	
	spid.kpidF[0] = Bytes2Int16_t(buf,offset)/100.0f; offset+=2;
	spid.kpidF[1] = Bytes2Int16_t(buf,offset)/100.0f; offset+=2;
	spid.kpidF[2] = Bytes2Int16_t(buf,offset)/100.0f; offset+=2;
	
	spid.eFuzzyRule[0] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.eFuzzyRule[1] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.eFuzzyRule[2] = Bytes2Int16_t(buf,offset); offset+=2;
	
	spid.ecFuzzyRule[0] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.ecFuzzyRule[1] = Bytes2Int16_t(buf,offset); offset+=2;
	spid.ecFuzzyRule[2] = Bytes2Int16_t(buf,offset); offset+=2;
	
	spid.PID_Cutoff = Bytes2Int16_t(buf,offset)/10.0f; offset+=2;
	spid.PID_ControlCycle = Bytes2Int16_t(buf,offset)/10.0f; offset+=2;
	spid.PID_DeadZone = Bytes2Int16_t(buf,offset)/10.0f; offset+=2;
	
	spid.PWM_MAX = Bytes2Int32_t(buf,offset); offset+=4;
	spid.PWM_MIN = Bytes2Int32_t(buf,offset); offset+=4;
	spid.PWM_STEP = Bytes2Int32_t(buf,offset); offset+=4;
 	
	Init_FuzzyMap();	
	Calculate_FilteringCoefficient(spid.PID_Cutoff );
	 
	EEPROM_SAVE_PID();
}
void Init_FuzzyMap(void)
{
	  // l m s
	eFuzzyRule[0] = -1*spid.eFuzzyRule[0];
	eFuzzyRule[1] = -1*spid.eFuzzyRule[1];
	eFuzzyRule[2] = -1*spid.eFuzzyRule[2];
	eFuzzyRule[4] =  spid.eFuzzyRule[2];
	eFuzzyRule[5] =  spid.eFuzzyRule[1];
	eFuzzyRule[6] =  spid.eFuzzyRule[0];
	
	ecFuzzyRule[0] = -1*spid.ecFuzzyRule[0];
	ecFuzzyRule[1] = -1*spid.ecFuzzyRule[1];
	ecFuzzyRule[2] = -1*spid.ecFuzzyRule[2];
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
	 
  Int16_t2Bytes(spid.kpid[0]*100.f, buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpid[1]*100.f, buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpid[2]*100.f, buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.kpidF[0]*100.f, buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpidF[1]*100.f, buf,offset);offset+=2;
	Int16_t2Bytes(spid.kpidF[2]*100.f, buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.eFuzzyRule[0], buf,offset);offset+=2;
	Int16_t2Bytes(spid.eFuzzyRule[1], buf,offset);offset+=2;
	Int16_t2Bytes(spid.eFuzzyRule[2], buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.ecFuzzyRule[0], buf,offset);offset+=2;
	Int16_t2Bytes(spid.ecFuzzyRule[1], buf,offset);offset+=2;
	Int16_t2Bytes(spid.ecFuzzyRule[2], buf,offset);offset+=2;
	
	Int16_t2Bytes(spid.PID_Cutoff*10 , buf,offset);offset+=2;
	Int16_t2Bytes(spid.PID_ControlCycle*10, buf,offset);offset+=2;
	Int16_t2Bytes(spid.PID_DeadZone*10, buf,offset);offset+=2;
	
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
	Voltage_Set_Point=Bytes2Int16_t(buf,offset);
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
	 	
	Int16_t2Bytes((spid.kpid[0]+Kpid_calcu[0]*spid.kpidF[0])*100 , buf,offset);offset+=2;
	Int16_t2Bytes((spid.kpid[1]+Kpid_calcu[1]*spid.kpidF[1])*100 , buf,offset);offset+=2;
	Int16_t2Bytes((spid.kpid[2]+Kpid_calcu[2]*spid.kpidF[2])*100 , buf,offset);offset+=2;
	
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
		
	LoadPWM(PWM_Output) ;
}
 
//增量式PID控制设计
void Inc_PID_Calc(void)
{
	register float iError;
 	register int iIncpid;
	float NextPoint = GetADCVoltage(PID_Votage_Chanel);
	//当前误差
	iError = Voltage_Set_Point - NextPoint;
	Fuzzy_Kpid(iError, iError-spid.LastError) ;
	//Kpid_calcu[0];
	if(abs(iError)<spid.PID_DeadZone){
	iIncpid = 0;
	}else{//在中间
		iIncpid = (spid.kpid[0]+Kpid_calcu[0]*spid.kpidF[0]) * (iError-spid.LastError) //E[k]项
						+ (spid.kpid[1]+Kpid_calcu[1]*spid.kpidF[1]) * iError //E[k－1]项
						+ (spid.kpid[2]+Kpid_calcu[2]*spid.kpidF[2]) * (iError-2*spid.LastError+spid.PrevError); //E[k－2]项
	}
	//存储误差，用于下次计算
	spid.PrevError = spid.LastError;
	spid.LastError = iError;
 
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
	data_x = (int16_t) (spid.kpid[0]*100.f);
	data_y = (int16_t) (spid.kpid[1]*100.f);
	data_z = (int16_t) (spid.kpid[2]*100.f);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_P], data_x);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_I], data_y);
	EE_WriteVariable(VirtAddVarTab[EEPROM_PID_D], data_z);
	
	data_x = (int16_t) (spid.kpidF[0]*100.f);
	data_y = (int16_t) (spid.kpidF[1]*100.f);
	data_z = (int16_t) (spid.kpidF[2]*100.f);
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
	
	data_x = (int16_t) (spid.PID_Cutoff*10);
	data_y = (int16_t) (spid.PID_ControlCycle);
	data_z = (int16_t) (spid.PID_DeadZone*10);
	
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
	spid.kpid[0] = (float) data_x/100.0f;
	spid.kpid[1] = (float) data_y/100.0f;
	spid.kpid[2] = (float) data_z/100.0f;
	
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_PF], &data_x);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_IF], &data_y);
	EE_ReadVariable(VirtAddVarTab[EEPROM_PID_DF], &data_z);
	spid.kpidF[0] = (float) data_x/100.0f;
	spid.kpidF[1] = (float) data_y/100.0f;
	spid.kpidF[2] = (float) data_z/100.0f;
	
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
	
	spid.PID_Cutoff =   data_x/10.f;
	spid.PID_ControlCycle =   data_y;
	spid.PID_DeadZone =   data_z/10.f;
	
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
		/*****误差隶属函数描述*****/  
		if(e<eFuzzyRule[0]) {eFuzzy[0] =1;pe = 0;
		}   else if(eFuzzyRule[0]<=e && e<eFuzzyRule[1])  {eFuzzy[0] = (eFuzzyRule[1]-e)/(eFuzzyRule[1]-eFuzzyRule[0]);
			pe = 0;
		}   else if(eFuzzyRule[1]<=e && e<eFuzzyRule[2])  {eFuzzy[0] = (eFuzzyRule[2] -e)/(eFuzzyRule[2]-eFuzzyRule[1]);
			pe = 1;
		}   else if(eFuzzyRule[2]<=e && e<eFuzzyRule[3])  {eFuzzy[0] = (eFuzzyRule[3] -e)/(eFuzzyRule[3]-eFuzzyRule[2]);
			pe = 2;
		}   else if(eFuzzyRule[3]<=e && e<eFuzzyRule[4])  {eFuzzy[0] = (eFuzzyRule[4]-e)/(eFuzzyRule[4]-eFuzzyRule[3]);
			pe = 3;
		}   else if(eFuzzyRule[4]<=e && e<eFuzzyRule[5])  {eFuzzy[0] = (eFuzzyRule[5]-e)/(eFuzzyRule[5]-eFuzzyRule[4]);   
			pe = 4;
		}   else if(eFuzzyRule[5]<=e && e<eFuzzyRule[6])  {eFuzzy[0] = (eFuzzyRule[6]-e)/(eFuzzyRule[6]-eFuzzyRule[5]);   
			pe = 5;
		}   else  {   eFuzzy[0] =0;pe =5;
		}      
		eFuzzy[1] =(float)(1.0f - eFuzzy[0]);       
		/*****误差变化隶属函数描述*****/       
		if(ec<ecFuzzyRule[0]){   ecFuzzy[0] =1.0;pec = 0;
		}  else if(ecFuzzyRule[0]<=ec && ec<ecFuzzyRule[1])  {   ecFuzzy[0] = (ecFuzzyRule[1] - ec)/(ecFuzzyRule[1]-ecFuzzyRule[0]);   
			pec = 0   ;
		}  else if(ecFuzzyRule[1]<=ec && ec<ecFuzzyRule[2])  {   ecFuzzy[0] = (ecFuzzyRule[2] - ec)/(ecFuzzyRule[2]-ecFuzzyRule[1]);  
			pec = 1;
		}   else if(ecFuzzyRule[2]<=ec && ec<ecFuzzyRule[3]) {   ecFuzzy[0] = (ecFuzzyRule[3] - ec)/(ecFuzzyRule[3]-ecFuzzyRule[2]);   
			pec = 2 ;  
		}   else if(ecFuzzyRule[3]<=ec && ec<ecFuzzyRule[4]) {   ecFuzzy[0] = (ecFuzzyRule[4]-ec)/(ecFuzzyRule[4]-ecFuzzyRule[3]);
			pec=3;
		}   else if(ecFuzzyRule[4]<=ec && ec<ecFuzzyRule[5]) {   ecFuzzy[0] = (ecFuzzyRule[5]-ec)/(ecFuzzyRule[5]-ecFuzzyRule[4]);         
			pec=4;
		}   else if(ecFuzzyRule[5]<=ec && ec<ecFuzzyRule[6]) {   ecFuzzy[0] = (ecFuzzyRule[6]-ec)/(ecFuzzyRule[6]-ecFuzzyRule[5]);         
			pec=5;
		}   else  {   ecFuzzy[0] =0.0;		pec = 5;
		}   
		ecFuzzy[1] = (float)(1.0f - ecFuzzy[0]);    
		/***********查询模糊规则表*************/   
		
		for(KpidSelect=0;KpidSelect<3;KpidSelect++){
			memset(deFuzzyFactor,0,7*sizeof(float));
			num =FuzzyCtrlRuleMap[pec][pe][KpidSelect];   //主值
			deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[0]; 
			
			num =FuzzyCtrlRuleMap[pec][pe+1][KpidSelect];   
			deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[1];  
			
			num =FuzzyCtrlRuleMap[pec+1][pe][KpidSelect];   
			deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[0]; 
			
			num =FuzzyCtrlRuleMap[pec+1][pe+1][KpidSelect];   
			deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[1];  
			
			/********加权平均法解模糊********/
			temp=0;
			for(i=0;i<7;i++){
				temp+=deFuzzyFactor[i]*DefuzzyRuleMap[KpidSelect][i];      
			}
			 
			Kpid_calcu[KpidSelect]=temp;
		}
	 
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
	spid.kpid[0] = 4;
	spid.kpid[1] = 2;
	spid.kpid[2] = 2;
	
	spid.kpidF[0] = 0.5;
	spid.kpidF[1] = 0.5;
	spid.kpidF[2] = 0.5;
	
  spid.eFuzzyRule[0] = 3000;
	spid.eFuzzyRule[1] = 2000;
	spid.eFuzzyRule[2] = 1000;
	
	spid.ecFuzzyRule[0] = 300;
	spid.ecFuzzyRule[1] = 200;
	spid.ecFuzzyRule[2] = 100;
 
 	
	spid.PID_Cutoff=5;
	spid.PID_ControlCycle=50;
	spid.PID_DeadZone=1;
	 
 
	spid.PWM_MAX=200000;
	spid.PWM_MIN=100000;
	spid.PWM_STEP=5000;
	
	EEPROM_SAVE_PID();
}
 
void  Valve_Close(void)
{
	isRunning = 0;
	PWM_Output = spid.PWM_MIN;
	LoadPWM(PWM_Output) ;
}
void Valve_Open(void)
{
	isRunning = 0;
	PWM_Output = spid.PWM_MAX;
	LoadPWM(PWM_Output) ;
}
void Valve_PID_Ctrl()
{
	isRunning = 1;
}
uint8_t PID_isRunning(void){
	return isRunning;
} 
uint16_t abs( int val){
	return val>0?val:(-1*val);
}
//**********************************end of file**************************************
