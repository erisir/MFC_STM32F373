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
struct _FuzzyCtrlRuleMap *sFuzzyCtrlRuleMap;//int8_t *FuzzyCtrlRuleMap[7][7][3];

int16_t LastError; // Error[-1]
int16_t PrevError; // Error[-2]
int32_t SumError;

int32_t PWM_Output;
uint16_t Voltage_Set_Point;

float Kpid_calcu[3]={0.0,0.0,0.0};
float eFuzzy[]={0.0,0.0};       
float ecFuzzy[]={0.0,0.0};  
float deFuzzyFactor[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float  eFuzzyRule[7]={-3000,-2000,-1000,0,1000,2000,3000};   
float  ecFuzzyRule[7]={-300,-200,-100,0,100,200,300};

int8_t  DefuzzyRuleMap[3][7]={
								{-3,-2,-1,0.0,1,2,3}, 
								{-3,-2,-1,0.0,1,2,3},
								{-3,-2,-1,0.0,1,2,3}}; 

//int8_t  FuzzyCtrlRuleMap0[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->电磁阀*/
///*Row[ec]          NL 	        NM 	        NS 	        ZE        	PS       	PM 	        PL    */	
///*NL*/    {{ZE,ZE,ZE },{ZE,ZE,ZE },{ZE,ZE,ZE }/**/,{NL,NM,NL },{NS,NS,NL },{NM,PM,NM },{NL,PL,NL}} ,
///*NM*/    {{ZE,ZE,ZE },{ZE,ZE,ZE },{ZE,ZE,ZE }/**/,{NL,NM,NL },{NS,NS,NL },{NM,PM,ZE },{NL,PL,NS}} ,
///*NS*/    {{ZE,ZE,ZE },{ZE,ZE,ZE },{ZE,ZE,ZE }/**/,{NL,NM,NL },{NS,NS,NL },{NM,PM,ZE },{NL,PL,PS}} ,
///*ZE*/    		{{NL,PL,NL },{NS,NM,NL },{NS,PS,NS },{NL,NM,NL },{NS,NS,NL },{NM,PM,PM },{NL,PL,PL}} ,
///*PS*/    		{{NL,PL,NS },{NS,NM,NL },{NL,NL,NL },{ZE,ZE,ZE },/**/{ZE,ZE,ZE },{ZE,ZE,ZE },{ZE,ZE,ZE}} ,
///*PM*/    		{{NL,PL,PS },{NS,NM,NL },{NL,NL,NL },{ZE,ZE,ZE },/**/{ZE,ZE,ZE },{ZE,ZE,ZE },{ZE,ZE,ZE}} ,
///*PL*/    		{{NL,PL,PL },{NS,NM,NL },{NL,NL,NL },{ZE,ZE,ZE },/**/{ZE,ZE,ZE },{ZE,ZE,ZE },{ZE,ZE,ZE}}
//    		};
int8_t  FuzzyCtrlRuleMap0[7][7][3] = {/*Column[e]----Cell[?Kp/?KI/?KD]--------->电磁阀*/
/*Row[ec]          NL 	        NM 	        NS 	        ZE        	PS       	PM 	        PL    */	
/*NL*/        {{PL,NL,PS },{PL,NL,PS },{PM,NL,ZE },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PS,NL}} ,
/*NM*/    		{{PL,NL,NS },{PL,NL,NS },{PM,NM,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PM,NL }} ,
/*NS*/    		{{PM,NM,NL },{PM,NM,NL },{PM,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NL,PL,NL }} ,
/*ZE*/    		{{PM,NM,NL },{PS,NS,NM },{PS,NS,NM },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL },{NL,PL,NL }} ,
/*PS*/    		{{PS,NS,NL },{PS,NS,NM },{ZE,ZE,NS },{NL,NL,NL  },{NL,NL,NL  },{NL,NL,NL  },{NM,PM,PS}} ,
/*PM*/    		{{ZE,ZE,NM },{ZE,ZE,NS },{NS,PS,NS },{NM,PM,NS },{NM,PM,ZE },{NM,PL,PS },{NL,PL,PS}} ,
/*PL*/    		{{ZE,ZE,PS },{NS,ZE,PS },{NS,PS,ZE },{NM,PM,ZE },{NM,PL,ZE },{NL,PL,PL },{NL,PL,PL}}
    		};



/************************************************
				PID函数初始化
 *************************************************/
				
void PIDInit(void) 
{ 	
	spid->kpid[0] = 33;//for 500sccm
	spid->kpid[1] = 9;
	spid->kpid[2] = 33;
	
	spid->kpidF[0] = 11;
	spid->kpidF[1] = 2;
	spid->kpidF[2] = 11;

  spid->eFuzzyRule[0] = 3000;//
	spid->eFuzzyRule[1] = 1200;
	spid->eFuzzyRule[2] = 600;
	
	spid->ecFuzzyRule[0] = 1000;
	spid->ecFuzzyRule[1] = 500;
	spid->ecFuzzyRule[2] = 100;
 
 	
	spid->PID_Cutoff=3000;
	spid->PID_ControlCycle=15;
	spid->PID_DeadZone=2;
 
	spid->PWM_MAX=1000000;
	spid->PWM_MIN=860000;
	spid->PWM_STEP=600;
}
void FuzzyCtrlRuleMapInit(void)
{
		memcpy(sFuzzyCtrlRuleMap->data,FuzzyCtrlRuleMap0,147); 
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
void setVoltageSetPoint(uint16_t voltage)
{
	Voltage_Set_Point= voltage;
}
uint16_t getVoltageSetPoint(void)
{
	return Voltage_Set_Point;
}
/*********************************************************** 
              PID计算函数
 ***********************************************************/ 
void Inc_PID_Calc(void)
{
	register int iError;
 	register int32_t iIncpid;
	int NextPoint = GetADCVoltage(0);//max(NextPoint)=5000/5
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
	
	PWM_Output += iIncpid/100;
}
 
/*********************************************************** 
              kp,ki,kd计算函数,模糊PID
 ***********************************************************/  

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

		num =sFuzzyCtrlRuleMap->data[pec][pe][KpidSelect];   //主值 可取 NL,NM,LS...PS,PM,PL[0~6]	 	pec 行 pe 列		
		deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[0]; 

		num =sFuzzyCtrlRuleMap->data[pec+1][pe][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[0]*ecFuzzy[1];  

		num =sFuzzyCtrlRuleMap->data[pec][pe+1][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[0]; 

		num =sFuzzyCtrlRuleMap->data[pec+1][pe+1][KpidSelect];   
		deFuzzyFactor[num] += eFuzzy[1]*ecFuzzy[1];  

		/********加权平均法解模糊********///DefuzzyRuleMap = [-3,-2,-1,0,1,2,3]
		temp=0;
		for(i=0;i<7;i++){
			temp+=deFuzzyFactor[i]*DefuzzyRuleMap[KpidSelect][i];      
		}

		Kpid_calcu[KpidSelect]=temp;

	}

}
 

uint16_t myabs( int val){
	return val>0?val:(-1*val);
}
//**********************************end of file**************************************
