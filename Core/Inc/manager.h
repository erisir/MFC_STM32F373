/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __manager_H
#define __manager_H
 
/* Includes ------------------------------------------------------------------*/
#include "main.h"
///////////////////////all the type define goes here
typedef enum{
	emDigitalControl = 1,
	emVoltageControl, 
}controlModeEnum;
typedef enum{
	emHoldSetPoint = 0,
	emFollowSetPoint = 1, 
}HoldFollow;
typedef enum{
	emValvePID ,
	emValveClose ,
	emValveOpen ,	
}ValveMode;
typedef enum{
	emAccumulatorStop ,
	emAccumulatorPause,
	emAccumulatorPauseByOpenValve,
	emAccumulatorRunning ,	
}AccumulatorStatu;

struct TLinearFittingValue {//6 int32s
	uint16_t value[11];//0-100
	uint16_t none;
};
struct TControlMode {//1 int32s
	uint16_t controlMode ;
	uint16_t defaultCotrolMode;
	uint16_t saveEEPROM;
	uint16_t systemReset;
};

struct TSetPoint{//3 int32s
	uint16_t holdFollow;
	uint16_t delay;
	uint16_t digitalSetpoint;
	uint16_t softStartRate;
	uint16_t shutoffLevel;
	uint16_t activeSetpoint;

};

struct TZeroAndReadFlow{//4 int32s
	uint16_t zeroStatus;
	uint16_t accumulatorMode;
	uint16_t readFlow;	
	uint16_t none;	
	float accumulatorFlow;
	uint32_t targetNullValue;//FIXED 16.16
};
struct TValveCommand{//2 int32s
	uint16_t valveCommandMode;
	uint16_t valveCommand;
	uint16_t valveVoltage;
	uint16_t valveType;
};

struct TWarningsAlarms{//1 int32s
	uint16_t enableWarningsAlarms;
	uint16_t  clearWarningsAlarms;
};
struct TProduct{//6 int32s
	uint8_t firmwareRevision[8];//Text8
	uint8_t PCBRevision[8];//Text8
	uint8_t modelID[16];//Text16
	uint8_t MFCSeiral[16];//Text16
	uint8_t manufacturingDate[16];//Text16
	uint8_t calibrationDate[16];//Text16
	uint16_t productVersion;
	uint16_t none;
	uint8_t productName[32];//Text32
	uint8_t manufacturer[32];//Text32
};
struct TCalibrate{//6 int32s
	uint8_t targetGasName[32];//Text32
	uint16_t targetGasCode;
	uint16_t targetGasFullScaleRange;
	uint32_t tarGasConversionFactor;//FIXED 16.16
	
	uint8_t CalibrationGasName[32];//Text32
	uint16_t CalibrationGasCode;
	uint16_t CalibrationGasFullScaleRange;
	uint32_t CalGasConversionFactor;//FIXED 16.16
};

struct TMacBaudrate{//2 int32s
	uint16_t RS485MacAddress;
	uint16_t MBmode;
	uint16_t baudrate;
	uint16_t IRRCutoff;

};
struct TPidParameter{//11 int32s
  uint16_t none;
	uint16_t kpid[3]; // kp ki kd
	uint16_t kpidF[3]; // kp ki kd factor	
	uint16_t eFuzzyRule[3]; //  high middle low
	uint16_t ecFuzzyRule[3]; //  high middle low
 	
	uint16_t PID_Cutoff;
	uint16_t PID_ControlCycle;
	uint16_t PID_DeadZone;	

	
	uint32_t PWM_MAX;
	uint32_t PWM_MIN;
	uint32_t PWM_STEP;
};

struct TFuzzyCtrlRuleMap {//37 int32s
	int8_t data[7][7][3];
	int8_t none;
};

struct TRegInputWrap{
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
struct TRegHoldingWrap{//75*2= 150 int16t
	struct TPidParameter pPidParameter;//start address: 0
	struct TFuzzyCtrlRuleMap pFuzzyCtrlRuleMap;//start address: 22
	struct TLinearFittingValue pLinearFittingY;//start address: 96
	struct TControlMode pControlMode;// 必须写成数组的形式天知道为神马108 16bits +12 
	struct TSetPoint pSetPoint;
	struct TZeroAndReadFlow pZeroAndReadFlow;
	struct TValveCommand pValveCommand;
	struct TWarningsAlarms pWarningsAlarms;
	struct TProduct pProduct;
	struct TCalibrate pCalibrate;
	struct TMacBaudrate pMacBaudrate;	
};

void MFCInit(void);
void LinearFittingYInit(void);
void HolddingRegDataChange(void);
void SetContrlResource(uint8_t mode);
void EEPROM_INIT(void);

void EEPROM_READ(void);
void EEPROM_SAVE(void);


void Valve_Close(void);
void Valve_Open(void);


float piecewiselinearinterp(struct TLinearFittingValue * xDict,struct TLinearFittingValue * yDict,uint16_t DictSize,float xInput);//电压层面的分段线性插值
float VoltageToFlow(float voltage);
float FlowToVoltage(float flow);

void saveSevenStarUINT8DataToMBHoldingReg(uint8_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame);
void saveSevenStarUINT8DataToMBHoldingRegUINT16(uint16_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame);
void saveSevenStarUINT16DataToMBHoldingReg(uint16_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame);
void saveSevenStarUINT32DataToMBHoldingReg(uint32_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame);
void saveSevenStarTEXTxDataToMBHoldingRegUINT8(uint8_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame,uint8_t textSize);
void SevenStarExecute(uint8_t * pucFrame, uint16_t *usLength);

float UFRAC16ToFloat(uint16_t ufrac16);
uint16_t FloatToUFRAC16(float coverValue);
 
float GetTargetNullFlow(void);
uint8_t GetAccumulatorStatu(void);

void StartSoftStartTimer(uint16_t Voltage_Set_PointLinearFit,float rateFSpsc);
uint8_t GetSoftStartCounter(void);
void SoftStartCountintDown(void);


extern struct TRegInputWrap * pRegInputWrap;
extern struct TRegHoldingWrap * pRegHoldingWrap;
#endif /*__ manager_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
