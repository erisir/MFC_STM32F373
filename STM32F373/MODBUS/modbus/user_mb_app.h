#ifndef    USER_APP
#define USER_APP
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

 
/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START        0
#define S_DISCRETE_INPUT_NDISCRETES   7
#define S_COIL_START                  0
#define S_COIL_NCOILS                 3 // ON OFF pid
#define S_REG_INPUT_START             0
#define S_REG_INPUT_NREGS             2 // Ch0 Ch1
#define S_REG_HOLDING_START           0
#define S_REG_HOLDING_NREGS           43
/* salve mode: holding register's all address */
#define          S_HD_RESERVE                     0
#define          S_HD_CPU_USAGE_MAJOR             1
#define          S_HD_CPU_USAGE_MINOR             2
/* salve mode: input register's all address */
#define          S_IN_RESERVE                     0
/* salve mode: coil's all address */
#define          S_CO_RESERVE                     0
/* salve mode: discrete's all address */
#define          S_DI_RESERVE                     0

/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START        0
#define M_DISCRETE_INPUT_NDISCRETES   16
#define M_COIL_START                  0
#define M_COIL_NCOILS                 64
#define M_REG_INPUT_START             0
#define M_REG_INPUT_NREGS             100  
#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           100
struct REG_INPUTs{
	uint16_t voltageCh0;  
	uint16_t voltageCh1; 
};
struct REG__HOLDINGs{
	//spid
	int kpid[3]; // kp ki kd
	int kpidF[3]; // kp ki kd factor	
	uint16_t eFuzzyRule[3]; //  high middle low
	uint16_t ecFuzzyRule[3]; //  high middle low
 	
	int PID_Cutoff;
	uint16_t PID_ControlCycle;
	int PID_DeadZone;
	 
 
	
	uint32_t PWM_MAX;
	uint32_t PWM_MIN;
	uint32_t PWM_STEP;//21 int
	//running
	uint16_t Voltage_Set_Point;
	uint32_t PWM_Output;
	uint16_t voltageCh0;  
	uint16_t voltageCh1; 
	
	uint16_t DEBUG0;  
	uint16_t DEBUG1; 
	uint16_t DEBUG2; 
	
	uint16_t DEBUG3;
	uint16_t DEBUG4;
	uint16_t DEBUG5;
	
	uint16_t DEBUG6;
	uint16_t DEBUG7;
	uint16_t DEBUG8;
	
	uint32_t DEBUG9;
	uint32_t DEBUGA;
	uint32_t DEBUGB;
	uint32_t DEBUGC;//22 int
	
};
/* master mode: holding register's all address */
#define          M_HD_RESERVE                     0
/* master mode: input register's all address */
#define          M_IN_RESERVE                     0
/* master mode: coil's all address */
#define          M_CO_RESERVE                     0
/* master mode: discrete's all address */
#define          M_DI_RESERVE                     0

#endif
