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
#define S_REG_INPUT_NREGS             29 // Ch0 Ch1
#define S_REG_HOLDING_START           0 //40000 等
#define S_REG_HOLDING_NREGS           128
 

 
struct REG_INPUTs{
	uint16_t voltageCh0;  
	uint16_t voltageCh1; 
	
	uint16_t DEBUG16[6];  
	uint32_t DEBUG32[4];
	uint32_t DEBUG161[10];
//22 int
};
struct REG__HOLDINGs{
	int16_t	PIDparam[22];
	int8_t  FuzzyCtrlRuleMap[7][7][3];
	int8_t  none;//96
	int16_t stdFlowValue[11];//0%--100%
};
extern struct REG_INPUTs * REG_INPUTsAddr;
extern struct REG__HOLDINGs * REG__HOLDINGssAddr;

#endif
