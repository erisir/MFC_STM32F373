//file user_mb_app.h

#ifndef    USER_APP
#define USER_APP

#include "main.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START        0 //40000 等
#define S_DISCRETE_INPUT_NDISCRETES   7
#define S_COIL_START                  0 //40000 等
#define S_COIL_NCOILS                 3 // ON OFF pid
#define S_REG_INPUT_START             0 //40000 等
#define S_REG_INPUT_NREGS             100 // Ch0 Ch1
#define S_REG_HOLDING_START           0 //40000 等
#define S_REG_HOLDING_NREGS           260
 

 
struct REG_INPUTs{
	uint16_t voltageCh0;  
	uint16_t voltageCh1; 
	uint16_t voltageSetPoint;
	int32_t pwmOut;
	float flowRawCh0;//%
	float flowRawCh1;//%
	
	float flowRealCh0;//%
	float flowRealCh1;//%
	
	float flowIIRFilterCh0;
	float flowIIRFilterCh1;
	
	float flowOffsetCh0;
	float flowOffsetCh1;
 
//22 int
};
struct REG__HOLDINGs{//75*2= 150 int16t
	int32_t	 PIDparam[11];//start address: 0
	int32_t  FuzzyCtrlRuleMap[37];//start address: 22
	int32_t  LinearFittingY[6];//start address: 96
	int32_t  ControlMode[2];// 必须写成数组的形式天知道为神马108 16bits +12 
	int32_t  SetPoint[3];
	int32_t  ZeroAndReadFlow[4];
	int32_t  ValveCommand[2];
	int32_t  WarningsAlarms[1];
	int32_t  Product[37];
	int32_t  Calibrate[20];
	int32_t  MacBaudrate[2];	
};
extern struct REG_INPUTs * REG_INPUTsAddr;
extern struct REG__HOLDINGs * REG__HOLDINGssAddr;

#endif
