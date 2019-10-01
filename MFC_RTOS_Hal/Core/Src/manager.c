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
uint16_t * Voltage_Set_PointCur;
uint16_t  Voltage_Set_PointLinearFit,lastVoltage_Set_Point;
float  flow_Set_PointGasFit;
 
uint16_t VirtAddVarTab[NB_OF_VARIABLES];//VirtAddVarTab
uint16_t linearFittingX[]={0,10*50,20*50,30*50,40*50,50*50,60*50,70*50,80*50,90*50,100*50};
uint8_t accumulatorStatu= 0;//0 stop,1 pause,2 running
void MFCInit(void)
{
	spid  = (struct _PID *)REG__HOLDINGssAddr->PIDparam;
	PIDInit();
	sFuzzyCtrlRuleMap = (struct _FuzzyCtrlRuleMap*)REG__HOLDINGssAddr->FuzzyCtrlRuleMap;
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
	sLinearFittingY->value[1] = 9.8*50;//500sccm
	sLinearFittingY->value[2] = 19.8*50;
	sLinearFittingY->value[3] = 29.8*50;
	sLinearFittingY->value[4] = 40*50;
	sLinearFittingY->value[5] = 50*50;
	sLinearFittingY->value[6] = 60*50;
	sLinearFittingY->value[7] = 70*50;
	sLinearFittingY->value[8] = 80*50;
	sLinearFittingY->value[9] = 90*50;
	sLinearFittingY->value[10] = 100*50;
	 
	sControlMode->controlMode = emDigitalControl;
	sControlMode->defaultCotrolMode=emDigitalControl;//default control mode on power on
	sControlMode->saveEEPROM = 0;// dont save
	
	sSetPoint->activeSetpoint=FloatToUFRAC16(0.5);//UFRAC16 current setpoint by external voltage
	sSetPoint->delay = 0;//no delay
	sSetPoint->digitalSetpoint = FloatToUFRAC16(0.5);//UFRAC16feedback target, user setpoint, FS%
	sSetPoint->holdFollow = emFollowSetPoint;//HoldSetPoint action inmidiatly
	sSetPoint->shutoffLevel = FloatToUFRAC16(0.015);//UFRAC161.5%FS to shutoff UFRAC16
	sSetPoint->softStartRate = FloatToUFRAC16(0);//UFRAC16 turn off softstart
	 
	sZeroAndReadFlow->accumulatorFlow = 0;//float
	sZeroAndReadFlow->accumulatorMode = 4;//0 restart,1:pause,3,resume,4,nomal continue flag
	sZeroAndReadFlow->readFlow = 0;//UFRAC16
	sZeroAndReadFlow->targetNullValue = 0;//custumer set zeropoint
	sZeroAndReadFlow->zeroStatus = 0;//set current to zero in 1,need to change to 0
	 
	sValveCommand->valveCommand = emValveClose;// valve off,2 valve on,0,valve pid
	sValveCommand->valveCommandMode=0;// not function
	sValveCommand->valveType=0;// normal on type
	sValveCommand->valveVoltage=0;// not function
	
	sWarningsAlarms->clearWarningsAlarms=0;// not function
	sWarningsAlarms->enableWarningsAlarms=0;// not function
	 
	
	sProduct->firmwareRevision=10;
	sProduct->PCBRevision=10;
	sProduct->modelID=10;
	sProduct->MFCSeiral=1234;
	sProduct->manufacturingDate=1909;
	sProduct->calibrationDate=1909;
	sProduct->productVersion=10;
	sProduct->productName=51;
	sProduct->manufacturer=51;
	
	sCalibrate->targetGasName=1;
	sCalibrate->targetGasCode=13;
	sCalibrate->targetGasFullScaleRange=1;
	sCalibrate->tarGasConversionFactor=1*65536;//fix16.16
	
	sCalibrate->CalibrationGasName=1;
	sCalibrate->CalibrationGasCode=13;
	sCalibrate->CalibrationGasFullScaleRange=1;
	sCalibrate->CalGasConversionFactor=1*65536;//fix16.16
	
	sMacBaudrate->RS485MacAddress=0x20;
	sMacBaudrate->baudrate=9600;
	sMacBaudrate->MBmode=0;//RTU
	sMacBaudrate->IRRCutoff=5;
	  
	SetContrlResource(sControlMode->controlMode);
	Valve_Close();
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
	
	SetContrlResource(sControlMode->defaultCotrolMode);
	sValveCommand->valveCommand = emValveClose;
	Valve_Close();
 
}
uint8_t GetAccumulatorStatu()
{
	return  accumulatorStatu;//0 stop,1 pause,2 running;
}
void HolddingRegDataChange(void)
{
	//accumulator change 0 restart,1:pause,3,resume,4,start continue flag
	if(sZeroAndReadFlow->accumulatorMode ==0)//reset
	{	
		sZeroAndReadFlow->accumulatorFlow=4;
		ResetFlowAccumulator();	
		accumulatorStatu = emAccumulatorRunning;
	}else if(sZeroAndReadFlow->accumulatorMode ==1)//pause
	{
		accumulatorStatu = emAccumulatorPause;
	}else if(sZeroAndReadFlow->accumulatorMode ==3)//resume
	{
		accumulatorStatu = emAccumulatorRunning;
	}
	
	//contrlmode change
	SetContrlResource(sControlMode->controlMode);
	//pid parameter change
	FuzzyRuleInit();
	//eeprom change
	if(sControlMode->saveEEPROM ==1)
	{
		sControlMode->saveEEPROM=0;
		EEPROM_SAVE();
	}
	// valve mode change
	if(sValveCommand->valveCommand ==emValveClose){
		Valve_Close();
	}else if(sValveCommand->valveCommand ==emValveOpen){
		if(accumulatorStatu == emAccumulatorRunning)
			accumulatorStatu = emAccumulatorPauseByOpenValve;
		Valve_Open();
	}else if(sValveCommand->valveCommand ==emValvePID){
		if(accumulatorStatu == emAccumulatorPauseByOpenValve)
			accumulatorStatu = emAccumulatorRunning;
	}
	//zero change
	if(sZeroAndReadFlow->zeroStatus ==1){
		ResetFlowOffset();
		sZeroAndReadFlow->zeroStatus =0;
	}
	//digital setpoint change
	if(*Voltage_Set_PointCur !=lastVoltage_Set_Point ){
		if(sSetPoint->holdFollow ==emFollowSetPoint){
			if(sSetPoint->delay<49 && sSetPoint->delay>1){
				osDelay(100);
			}else{
				osDelay(sSetPoint->delay);
			}
			lastVoltage_Set_Point = *Voltage_Set_PointCur ;
			flow_Set_PointGasFit =100*UFRAC16ToFloat(*Voltage_Set_PointCur)/(sCalibrate->tarGasConversionFactor/65536);//设定的时候要除 %
			Voltage_Set_PointLinearFit = FlowToVoltage(flow_Set_PointGasFit);
			REG_INPUTsAddr->voltageSetPoint = Voltage_Set_PointLinearFit;
			setVoltageSetPoint(Voltage_Set_PointLinearFit);
		}
	}
	//Calculate_FilteringCoefficient
	Calculate_FilteringCoefficient(sMacBaudrate->IRRCutoff);

}
void  Valve_Close(void)
{
	PWM_Output = 1;
	LoadPWM(PWM_Output) ;
}
void Valve_Open(void)
{
	PWM_Output = 65536*20;
	LoadPWM(PWM_Output) ;
}
void SetContrlResource(uint8_t mode)
{
	  
		switch(mode){
		case 1:
			Voltage_Set_PointCur=&sSetPoint->digitalSetpoint;//ufrat16
					
		break;
		case 2:
			Voltage_Set_PointCur = &REG_INPUTsAddr->voltageCh1;//ufrat16
		break;
		
		default:
			Voltage_Set_PointCur=&sSetPoint->digitalSetpoint;//ufrat16
		break;
	}
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

float piecewiselinearinterp(struct _LinearFittingValue * xDict,struct _LinearFittingValue * yDict,uint16_t DictSize,float xInput)
{
 
	uint16_t x1,x2,y1,y2,i=0;
	float yOutput=0;
  if(xInput<=xDict->value[0] ||xInput>=xDict->value[DictSize-1]){
		yOutput= xInput;
	}else{
		for(i=0;i<DictSize-1;i++){
			if( (xInput>=xDict->value[i]) && (xInput<xDict->value[i+1])){
				x1 = xDict->value[i];
				x2 = xDict->value[i+1];
				y1 = yDict->value[i];
				y2 = yDict->value[i+1];
				yOutput =(float)(xInput-x2)*y1/(x1-x2)+(xInput-x1)*y2/(x2-x1);
				break;
			}
		}
	}

	return yOutput;
}
/*
voltage:0-5000*1.25
return:0-100:  %
*/
float VoltageToFlow(float voltage) 
{
	return piecewiselinearinterp(sLinearFittingY,sLinearFittingX,11,voltage)/50;
}
float FlowToVoltage(float flow)//real Target voltage cover to current MFC sensor Voltage
{
	return piecewiselinearinterp(sLinearFittingX,sLinearFittingY,11,flow*50);
}
float UFRAC16ToFloat(uint16_t ufrac16)
{
	return (float) (ufrac16-16384)/32768;
}
uint16_t FloatToUFRAC16(float floatValue)
{
	return floatValue*32768+16384;
}
float GetTargetNullFlow(void)
{
	return 0;
}
void SevenStarExecute(uint8_t * pucFrame, uint16_t *usLength)
{
	uint8_t dataClass = pucFrame[4];
	uint8_t dataAttribute = pucFrame[6];

	switch (dataClass){
		case 0x69:// control mode & setpoint
			switch (dataAttribute){
				case 0x03:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sControlMode->controlMode,usLength,pucFrame);
				break;
				case 0x04:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sControlMode->defaultCotrolMode,usLength,pucFrame);
				break;
				case 0x06:
					sControlMode->saveEEPROM = pucFrame[7];
				break;
				case 0x05:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sSetPoint->holdFollow,usLength,pucFrame);
				break;
				case 0xA6:
					saveSevenStarUINT16DataToMBHoldingReg(&sSetPoint->delay,usLength,pucFrame);
				break;
				case 0xA4:
					saveSevenStarUINT16DataToMBHoldingReg(&sSetPoint->digitalSetpoint,usLength,pucFrame);
				break;
				case 0xA5:
					saveSevenStarUINT16DataToMBHoldingReg(&sSetPoint->digitalSetpoint,usLength,pucFrame);
				break;				
			}
		break;
		case 0x6A:// setpoint &valve cmd
			switch (dataAttribute){
				case 0xA4:
					saveSevenStarUINT16DataToMBHoldingReg(&sSetPoint->softStartRate,usLength,pucFrame);
				break;
				case 0xA2:
					saveSevenStarUINT16DataToMBHoldingReg(&sSetPoint->shutoffLevel,usLength,pucFrame);
				break;
				case 0xA1:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sValveCommand->valveCommandMode,usLength,pucFrame);
				break;
				case 0x01:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sValveCommand->valveCommand,usLength,pucFrame);
				break;	
				case 0x91:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sValveCommand->valveVoltage,usLength,pucFrame);
				break;	
				case 0x9C:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sValveCommand->valveType,usLength,pucFrame);
				break;					
			}
		break;
		case 0x68:// zero
			switch (dataAttribute){
				case 0xBA:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sZeroAndReadFlow->zeroStatus,usLength,pucFrame);
				break;
				case 0xB9:
					saveSevenStarUINT16DataToMBHoldingReg(&sZeroAndReadFlow->readFlow,usLength,pucFrame);
				break;				 			
			}
		break;
		case 0xA4:// Accumulator
			switch (dataAttribute){
				case 0x05:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sZeroAndReadFlow->accumulatorMode,usLength,pucFrame);
				break;
				case 0x03:
					saveSevenStarUINT32DataToMBHoldingReg(&sZeroAndReadFlow->accumulatorFlow,usLength,pucFrame);
				break;				 			
			}
			break;
		case 0x65:// warning
			switch (dataAttribute){
				case 0xA2:
					saveSevenStarUINT16DataToMBHoldingReg(&sWarningsAlarms->enableWarningsAlarms,usLength,pucFrame);
				break;
				case 0xA1:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sWarningsAlarms->clearWarningsAlarms,usLength,pucFrame);
				break;	
				case 0xA0:
					//saveSevenStarUINT8DataToMBHoldingRegUINT16(&sWarningsAlarms->none,usLength,pucFrame);
				break;					
			}
		break;
		case 0x01:// Product
			switch (dataAttribute){
				case 0x07:
					saveSevenStarUINT32DataToMBHoldingReg(&sProduct->productName,usLength,pucFrame);
				break;
				case 0x04:
					saveSevenStarUINT16DataToMBHoldingReg(&sProduct->productVersion,usLength,pucFrame);
				break;					
			}
		break;
	  case 0x64:// Product
			switch (dataAttribute){
				case 0x03:
					saveSevenStarUINT32DataToMBHoldingReg(&sProduct->manufacturer,usLength,pucFrame);
				break;
				case 0x04:
					saveSevenStarUINT16DataToMBHoldingReg(&sProduct->modelID,usLength,pucFrame);
				break;	
				case 0x05:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sProduct->firmwareRevision,usLength,pucFrame);
				break;	
				case 0x06:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sProduct->PCBRevision,usLength,pucFrame);
				break;	
				case 0x07:
					saveSevenStarUINT16DataToMBHoldingReg(&sProduct->MFCSeiral,usLength,pucFrame);
				break;	
				case 0x0a:
					saveSevenStarUINT16DataToMBHoldingReg(&sProduct->manufacturingDate,usLength,pucFrame);
				break;	
				case 0x0c:
					saveSevenStarUINT16DataToMBHoldingReg(&sProduct->calibrationDate,usLength,pucFrame);
				break;					
			}
		break;
		case 0x66:// Calibrate
			switch (dataAttribute){
				case 0x01:
					saveSevenStarUINT32DataToMBHoldingReg(&sCalibrate->targetGasName,usLength,pucFrame);
				break;
				case 0x02:
					saveSevenStarUINT16DataToMBHoldingReg(&sCalibrate->targetGasCode,usLength,pucFrame);
				break;	
				case 0x03:
					saveSevenStarUINT16DataToMBHoldingReg(&sCalibrate->targetGasFullScaleRange,usLength,pucFrame);
				break;	
				case 0x04:
					saveSevenStarUINT32DataToMBHoldingReg(&sCalibrate->tarGasConversionFactor,usLength,pucFrame);
				break;	
				case 0x06:
					saveSevenStarUINT32DataToMBHoldingReg(&sCalibrate->CalibrationGasName,usLength,pucFrame);
				break;	
				case 0x07:
					saveSevenStarUINT16DataToMBHoldingReg(&sCalibrate->CalibrationGasCode,usLength,pucFrame);
				break;	
				case 0x08:
					saveSevenStarUINT16DataToMBHoldingReg(&sCalibrate->CalibrationGasFullScaleRange,usLength,pucFrame);
				break;	
				case 0x09:
					saveSevenStarUINT32DataToMBHoldingReg(&sCalibrate->CalGasConversionFactor,usLength,pucFrame);
				break;					
			}
		break;
		case 0xA1:// Calibrate
			switch (dataAttribute){
				case 0x07:
					saveSevenStarUINT32DataToMBHoldingReg(&sZeroAndReadFlow->targetNullValue,usLength,pucFrame);
				break;
			}
		break;
		case 0x03:// Calibrate
			switch (dataAttribute){
				case 0x01:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sMacBaudrate->RS485MacAddress,usLength,pucFrame);
				break;
				case 0x02:
					saveSevenStarUINT16DataToMBHoldingReg(&sMacBaudrate->baudrate,usLength,pucFrame);
				break;
				case 0x03:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&sMacBaudrate->MBmode,usLength,pucFrame);
				break;
			}
		break;
	}
	HolddingRegDataChange();
}
void saveSevenStarUINT8DataToMBHoldingReg(uint8_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame)
{
if (pucFrame[2]== 0x80)//read
	{
		*usLength =1;
		pucFrame[3]=4;
		pucFrame[7]=* MBHoldRegAddress;
	}
	if (pucFrame[2]== 0x81)//write
	{
		* MBHoldRegAddress = pucFrame[7];
	}
}
void saveSevenStarUINT8DataToMBHoldingRegUINT16(uint16_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame)
{
if (pucFrame[2]== 0x80)//read
	{
		*usLength =1;
		pucFrame[3]=4;
		pucFrame[7]=* MBHoldRegAddress;
	}
	if (pucFrame[2]== 0x81)//write
	{
		* MBHoldRegAddress = pucFrame[7];
	}
}
void saveSevenStarUINT16DataToMBHoldingReg(uint16_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame)
{
if (pucFrame[2]== 0x80)//read
	{
		*usLength =2;
		pucFrame[3]=5;
		pucFrame[7]=(* MBHoldRegAddress)&0x00FF;//lowbit
		pucFrame[8]=(* MBHoldRegAddress)>>8;//hightbit
	}
	if (pucFrame[2]== 0x81)//write
	{
		* MBHoldRegAddress = pucFrame[7]+pucFrame[8]*256;
	}
}
void saveSevenStarUINT32DataToMBHoldingReg(uint32_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame)
{
if (pucFrame[2]== 0x80)//read
	{
		*usLength =4;
		pucFrame[3]=7;
		pucFrame[7]=(* MBHoldRegAddress)&0x00FF;//lowbit
		pucFrame[8]=((* MBHoldRegAddress)>>8)&0x00FF; 
		pucFrame[9]=((* MBHoldRegAddress)>>16)&0x00FF; 
		pucFrame[10]=((* MBHoldRegAddress)>>24)&0x00FF;//hightbit
	}
	if (pucFrame[2]== 0x81)//write
	{
		* MBHoldRegAddress = pucFrame[7]+pucFrame[8]*256+pucFrame[9]*65536+pucFrame[10]*256*65536;
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
