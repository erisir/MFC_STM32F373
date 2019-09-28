/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "manager.h"

struct  _LinearFittingValue*  sLinearFittingX;
struct  _LinearFittingValue*  sLinearFittingY;
struct  _ControlMode*				  sControlMode;
struct  _SetPoint*					  sSetPoint ;
struct  _ZeroAndReadFlow*		  sZeroAndReadFlow ;
struct	_ValveCommand*		 	  sValveCommand ;
struct	_WarningsAlarms*			sWarningsAlarms ;
struct	_Product*							sProduct ;
struct	_Calibrate*						sCalibrate;
struct	_Sensor*							sSensor ;
struct	_MacBaudrate*					sMacBaudrate;
		
 
uint16_t data_x,data_y,data_z;
 
uint8_t isRunning=0;
 
 
uint16_t VirtAddVarTab[NB_OF_VARIABLES];//VirtAddVarTab
uint16_t linearFittingX[]={0,10*50,20*50,30*50,40*50,50*50,60*50,70*50,80*50,90*50,100*50};

void MFCInit(void)
{
	spid  = (struct _PID *)REG__HOLDINGssAddr->PIDparam;
	PIDInit();
	FuzzyCtrlRuleMap = (struct _FuzzyCtrlRuleMap*)REG__HOLDINGssAddr->FuzzyCtrlRuleMap;
	FuzzyCtrlRuleMapInit();
	sLinearFittingX = (struct _LinearFittingValue*)linearFittingX;
	sLinearFittingY = (struct _LinearFittingValue*)REG__HOLDINGssAddr->LinearFittingY;
	sControlMode = (struct _ControlMode*)REG__HOLDINGssAddr->ControlMode;
	sSetPoint = (struct _SetPoint*)REG__HOLDINGssAddr->SetPoint;
	sZeroAndReadFlow = (struct _ZeroAndReadFlow*)REG__HOLDINGssAddr->ZeroAndReadFlow;
	sValveCommand = (struct _ValveCommand*)REG__HOLDINGssAddr->ValveCommand;
	sWarningsAlarms = (struct _WarningsAlarms*)REG__HOLDINGssAddr->WarningsAlarms;
	sProduct = (struct _Product*)REG__HOLDINGssAddr->Product;
	sCalibrate = (struct _Calibrate*)REG__HOLDINGssAddr->Calibrate;
	sMacBaudrate = (struct _MacBaudrate*)REG__HOLDINGssAddr->MacBaudrate;
	
	sLinearFittingY->value[0] = 0;
	sLinearFittingY->value[1] = 5.4*50;//500sccm
	sLinearFittingY->value[2] = 16.6*50;
	sLinearFittingY->value[3] = 27.8*50;
	sLinearFittingY->value[4] = 38.8*50;
	sLinearFittingY->value[5] = 49.6*50;
	sLinearFittingY->value[6] = 60.2*50;
	sLinearFittingY->value[7] = 70.4*50;
	sLinearFittingY->value[8] = 80.4*50;
	sLinearFittingY->value[9] = 90*50;
	sLinearFittingY->value[10] = 99.6*50;
	
	sControlMode->controlMode = emDigitalControl;
	sControlMode->defaultCotrolMode=emDigitalControl;//default control mode on power on
	sControlMode->saveEEPROM = 0;// dont save
	
	sSetPoint->activeSetpoint=0;// current setpoint by external voltage
	sSetPoint->delay = 0;//no delay
	sSetPoint->digitalSetpoint = 0;//feedback target, user setpoint, FS%
	sSetPoint->holdFollow = FollowSetPoint;//HoldSetPoint action inmidiatly
	sSetPoint->shutoffLevel = 0x4000;//1.5%FS to shutoff UFRAC16
	sSetPoint->softStartRate = 0;// turn off softstart
	
	sZeroAndReadFlow->accumulatorFlow = 0;
	sZeroAndReadFlow->accumulatorMode = 0;//restart,1:pause,3,resume,4,nomal continue flag
	sZeroAndReadFlow->readFlow = 0;
	sZeroAndReadFlow->targetNullValue = 0;//custumer set zeropoint
	sZeroAndReadFlow->zeroStatus = 0;//set current to zero in 1,need to change to 0
	 
	sValveCommand->valveCommand = 1;// valve off,2 valve on,0,valve pid
	sValveCommand->valveCommandMode=0;// not function
	sValveCommand->valveType=0;// normal on type
	sValveCommand->valveVoltage=0;// not function
	
	sWarningsAlarms->clearWarningsAlarms=0;// not function
	sWarningsAlarms->enableWarningsAlarms=0;// not function
	sWarningsAlarms->none=0;// not function
	
	sProduct->firmwareRevision=1;
	sProduct->PCBRevision=1;
	sProduct->modelID=1;
	sProduct->MFCSeiral=1;
	sProduct->manufacturingDate=1;
	sProduct->calibrationDate=1;
	sProduct->productVersion=1;
	sProduct->productName=1;
	sProduct->manufacturer=1;
	
	sCalibrate->targetGasName=1;
	sCalibrate->targetGasCode=1;
	sCalibrate->targetGasFullScaleRange=1;
	sCalibrate->targetGasToCalibrationGasConversionFactor=1;
	sCalibrate->CalibrationGasName=1;
	sCalibrate->CalibrationGasCode=1;
	sCalibrate->CalibrationGasFullScaleRange=1;
	sCalibrate->CalibrationGasToN2ConversionFactor=1;
	
	sMacBaudrate->RS485MacAddress=0x20;
	sMacBaudrate->baudrate=9600;
	sMacBaudrate->reset=0;
}
 
void EEPROM_INIT(void)// 
{

	/* Unlock the Flash Program Erase controller */
  for (int VarValue = 0; VarValue < NB_OF_VARIABLES; VarValue++)
  {
    VirtAddVarTab[VarValue] = (uint16_t)(VarValue + 1);
  }

	HAL_FLASH_Unlock();
	EE_Init(VirtAddVarTab, EE_FORCED_ERASE);

	EE_ReadVariable16bits(VirtAddVarTab[160], &data_x);
	if(data_x != 160)
	{
		EE_WriteVariable16bits(VirtAddVarTab[160], 160);
		EEPROM_SAVE();
	}else{
		EEPROM_READ();
	}
	HAL_FLASH_Lock();
}
 
 
void  Valve_Close(void)
{
	PWM_Output = spid->PWM_MIN;
	LoadPWM(PWM_Output) ;
}
void Valve_Open(void)
{
	PWM_Output = spid->PWM_MAX;
	LoadPWM(PWM_Output) ;
}
 
void EEPROM_SAVE(void)	 
{
	uint16_t * pbuf = (uint16_t *)REG__HOLDINGssAddr;
	HAL_FLASH_Unlock();
	
	for(int i=0;i<150;i++){
		EE_WriteVariable16bits(VirtAddVarTab[i], *(pbuf++));
	}
	HAL_FLASH_Lock();
}
void EEPROM_READ(void)	// 
{
  uint16_t * pbuf = (uint16_t *)REG__HOLDINGssAddr;
	HAL_FLASH_Unlock();
	for(int i=0;i<150;i++){
		EE_ReadVariable16bits(VirtAddVarTab[i], pbuf++);
	}
  HAL_FLASH_Lock();
	  
}

uint16_t piecewiselinearinterp(struct _LinearFittingValue * xDict,struct _LinearFittingValue * yDict,uint16_t DictSize,uint16_t xInput)
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
	currVoltage= GetADCVoltage(0);
	outputVoltage=VoltageToFlow(currVoltage);
	
	REG_INPUTsAddr->DEBUG16[0]= currVoltage;
	REG_INPUTsAddr->DEBUG16[1]= outputVoltage;
	
	 
	
	AD5761_SetVoltage(outputVoltage);
}
 
uint16_t VoltageToFlow(uint16_t voltage) 
{
	return piecewiselinearinterp(sLinearFittingY,sLinearFittingX,11,voltage);
	//AD5761_SetVotage(vout);
}
uint16_t FlowToVoltage(uint16_t flow)//real Target voltage cover to current MFC sensor Voltage
{
	//uint16_t voltage = 0;
	//scalValue->value[0];
	//voltage = (uint16_t)(13.0f*filter_voltage.ch0);
	return piecewiselinearinterp(sLinearFittingX,sLinearFittingY,11,flow);
  //AD5761_SetVotage(voltage);
}
void UFRAC16ToFloat(uint8_t highBit,uint8_t lowBit,float *coverValue)
{
	uint16_t RawValue = highBit*256+lowBit;
	*coverValue= (float) (RawValue-16384)/32768;
}
void FloatToUFRAC16(float coverValue, uint8_t *highBit,uint8_t *lowBit)
{
	uint16_t temp = coverValue*32768+16384;
	*highBit = temp/256;
	*lowBit = temp%256;
}
void SevenStarExecute(uint8_t * pucFrame, uint16_t *usLength)
{
	uint8_t highBit,lowBit;
	float valueSet = 0.0;
	uint8_t readWrite = pucFrame[2];
	uint8_t dataLen = pucFrame[3];
	uint8_t dataClass = pucFrame[4];
	uint8_t dataInstance = pucFrame[5];
	uint8_t dataAttribute = pucFrame[6];
	if(dataLen ==5){
		highBit = pucFrame[8];
		lowBit = pucFrame[7];
		UFRAC16ToFloat(highBit, lowBit,&valueSet);
	}
	switch (dataClass){
		case 0x69:// control mode
			switch (dataAttribute){
				case 0x03://set control mode
					if (readWrite== 0x80)//read
					{
					}
					if (readWrite== 0x81)//write
					{
					}
					break;
				case 0x04://set default control mode
					break;
				case 0x06://save to ERROM
					break;
				

			}
			break;
	}
	if(readWrite == 0x80){//read
		pucFrame[3]= 5;//data len
		FloatToUFRAC16(0.119,&pucFrame[8],&pucFrame[7]);
		*usLength =2;
	}
	if(readWrite == 0x81){//write

	}
	
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
