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
	emHoldSetPoint = 0,
	emFollowSetPoint = 1, 
}HoldFollow;
typedef enum{
	emValvePID ,
	emValveClose ,
	emValveOpen ,	
}ValveMode;
struct _LinearFittingValue {//6 int32s
	uint16_t value[11];//0-100
	uint16_t none;
};
struct _ControlMode {//1 int32s
	uint8_t controlMode ;
	uint8_t defaultCotrolMode;
	uint8_t saveEEPROM;
	uint8_t none;
};

struct _SetPoint{//3 int32s
	uint8_t holdFollow;
	uint8_t none;
	uint16_t delay;
	uint16_t digitalSetpoint;
	uint16_t softStartRate;
	uint16_t shutoffLevel;
	uint16_t activeSetpoint;

};

struct _ZeroAndReadFlow{//3 int32s
	uint8_t zeroStatus;
	uint8_t accumulatorMode;
	uint16_t readFlow;	
	uint32_t accumulatorFlow;
	uint32_t targetNullValue;
};
struct _ValveCommand{//1 int32s
	uint8_t valveCommandMode;
	uint8_t valveCommand;
	uint8_t valveVoltage;
	uint8_t valveType;
};

struct _WarningsAlarms{//1 int32s
	uint16_t enableWarningsAlarms;
	uint8_t  clearWarningsAlarms;
	uint8_t none;
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
	uint32_t tarGasConversionFactor;
	
	uint32_t CalibrationGasName;
	uint16_t CalibrationGasCode;
	uint16_t CalibrationGasFullScaleRange;
	uint32_t CalGasConversionFactor;
};

struct _MacBaudrate{//1 int32s
	uint8_t RS485MacAddress;
	uint8_t MBmode;
	uint16_t baudrate;

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


float piecewiselinearinterp(struct _LinearFittingValue * xDict,struct _LinearFittingValue * yDict,uint16_t DictSize,float xInput);//电压层面的分段线性插值
float VoltageToFlow(float voltage);
float FlowToVoltage(float flow);

void saveSevenStarUINT8DataToMBHoldingReg(uint8_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame);
void saveSevenStarUINT16DataToMBHoldingReg(uint16_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame);
void saveSevenStarUINT32DataToMBHoldingReg(uint32_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame);
void SevenStarExecute(uint8_t * pucFrame, uint16_t *usLength);

float UFRAC16ToFloat(uint16_t ufrac16);
uint16_t FloatToUFRAC16(float coverValue);
 
float GetTargetNullFlow(void);

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
