/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __manager_H
#define __manager_H
 
/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef enum{
	emDigitalControl = 1,
	emVoltageControl, 
}controlModeEnum;
typedef enum{
	HoldSetPoint = 0,
	FollowSetPoint = 1, 
}HoldFollow;
struct _LinearFittingValue {//6 int32s
	int16_t value[11];//0-100
	int16_t none;
};
struct _ControlMode {//1 int32s
	int8_t controlMode ;
	int8_t defaultCotrolMode;
	int8_t saveEEPROM;
	int8_t none;
};

struct _SetPoint{//3 int32s
	int8_t holdFollow;
	int16_t delay;
	int16_t digitalSetpoint;
	int16_t softStartRate;
	int16_t shutoffLevel;
	int16_t activeSetpoint;
	int8_t none;
};

struct _ZeroAndReadFlow{//3 int32s
	int8_t zeroStatus;
	int8_t accumulatorMode;
	uint16_t readFlow;	
	int32_t accumulatorFlow;
	uint32_t targetNullValue;
};
struct _ValveCommand{//1 int32s
	int8_t valveCommandMode;
	int8_t valveCommand;
	int8_t valveVoltage;
	int8_t valveType;
};

struct _WarningsAlarms{//1 int32s
	int16_t enableWarningsAlarms;
	int8_t  clearWarningsAlarms;
	int8_t none;
};
struct _Product{//5 int32s
	uint8_t firmwareRevision;
	uint8_t PCBRevision;
	uint16_t modelID;
	uint16_t MFCSeiral;
	uint16_t manufacturingDate;
	uint16_t calibrationDate;
	uint16_t productVersion;
	uint32_t productName;
	uint32_t manufacturer;
};
struct _Calibrate{//6 int32s
	uint32_t targetGasName;
	uint16_t targetGasCode;
	uint16_t targetGasFullScaleRange;
	uint32_t targetGasToCalibrationGasConversionFactor;
	
	uint32_t CalibrationGasName;
	uint16_t CalibrationGasCode;
	uint16_t CalibrationGasFullScaleRange;
	uint32_t CalibrationGasToN2ConversionFactor;
};
 
struct _MacBaudrate{//1 int32s
	uint8_t RS485MacAddress;
	uint8_t MBmode;
	uint16_t baudrate;

};

void MFCInit(void);
void LinearFittingYInit(void);


void EEPROM_INIT(void);

void EEPROM_READ(void);
void EEPROM_SAVE(void);


void Valve_Close(void);
void Valve_Open(void);


uint16_t piecewiselinearinterp(struct _LinearFittingValue * xDict,struct _LinearFittingValue * yDict,uint16_t DictSize,uint16_t xInput);//电压层面的分段线性插值
uint16_t VoltageToFlow(uint16_t voltage);
uint16_t FlowToVoltage(uint16_t flow);

void VoltageOutLinerFix(void);

void SevenStarExecute(uint8_t * pucFrame, uint16_t *usLength);

void UFRAC16ToFloat(uint8_t highBit,uint8_t lowBit,float *coverValue);
void FloatToUFRAC16(float coverValue, uint8_t *highBit,uint8_t *lowBit);


extern struct  _ControlMode*				sControlMode;
extern struct  _SetPoint*					  sSetPoint ;
extern struct  _ZeroAndReadFlow*		sZeroAndReadFlow ;
extern struct	_ValveCommand*		 	  sValveCommand ;
extern struct	_WarningsAlarms*			sWarningsAlarms ;
extern struct	_Product*							sProduct ;
extern struct	_Calibrate*						sCalibrate;
extern struct	_Sensor*							sSensor ;
extern struct	_MacBaudrate*					sMacBaudrate;

#endif /*__ manager_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
