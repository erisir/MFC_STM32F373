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
	
uint16_t data_x,data_y,data_z;
 
uint8_t isRunning=0;
uint16_t * Voltage_Set_PointCur;
uint16_t  Voltage_Set_PointLinearFit,lastVoltage_Set_Point;
float  flow_Set_PointGasFit;
 
uint16_t VirtAddVarTab[NB_OF_VARIABLES];//VirtAddVarTab
uint16_t linearFittingX[]={0,10*50,20*50,30*50,40*50,50*50,60*50,70*50,80*50,90*50,100*50};
struct TLinearFittingValue *sLinearFittingX = (struct TLinearFittingValue*)linearFittingX;
uint8_t accumulatorStatu= 0;//0 stop,1 pause,2 running
uint8_t softStartCounter=0;
void MFCInit(void)
{
	//fill holding register with default value before MB_Init();
	PIDInit();
	FuzzyCtrlRuleMapInit();

	pRegHoldingWrap->pLinearFittingY.value[0] = 0;
	pRegHoldingWrap->pLinearFittingY.value[1] = 9.8*50;//500sccm
	pRegHoldingWrap->pLinearFittingY.value[2] = 19.8*50;
	pRegHoldingWrap->pLinearFittingY.value[3] = 29.8*50;
	pRegHoldingWrap->pLinearFittingY.value[4] = 40*50;
	pRegHoldingWrap->pLinearFittingY.value[5] = 50*50;
	pRegHoldingWrap->pLinearFittingY.value[6] = 60*50;
	pRegHoldingWrap->pLinearFittingY.value[7] = 70*50;
	pRegHoldingWrap->pLinearFittingY.value[8] = 80*50;
	pRegHoldingWrap->pLinearFittingY.value[9] = 90*50;
	pRegHoldingWrap->pLinearFittingY.value[10] = 100*50;
	 
	pRegHoldingWrap->pControlMode.controlMode = emDigitalControl;
	pRegHoldingWrap->pControlMode.defaultCotrolMode=emDigitalControl;//default control mode on power on
	pRegHoldingWrap->pControlMode.saveEEPROM = 0;// dont save
	
	pRegHoldingWrap->pSetPoint.activeSetpoint=FloatToUFRAC16(0.0);//UFRAC16 current setpoint by external voltage
	pRegHoldingWrap->pSetPoint.delay = 0;//no delay
	pRegHoldingWrap->pSetPoint.digitalSetpoint = FloatToUFRAC16(0.0);//UFRAC16feedback target, user setpoint, FS%
	pRegHoldingWrap->pSetPoint.holdFollow = emFollowSetPoint;//HoldSetPoint action inmidiatly
	pRegHoldingWrap->pSetPoint.shutoffLevel = FloatToUFRAC16(0.015);//UFRAC161.5%FS to shutoff UFRAC16
	pRegHoldingWrap->pSetPoint.softStartRate = FloatToUFRAC16(0);//UFRAC16 turn off softstart
	 
	pRegHoldingWrap->pZeroAndReadFlow.accumulatorFlow = 0;//float
	pRegHoldingWrap->pZeroAndReadFlow.accumulatorMode = 4;//0 restart,1:pause,3,resume,4,nomal continue flag
	pRegHoldingWrap->pZeroAndReadFlow.readFlow = 0;//UFRAC16
	pRegHoldingWrap->pZeroAndReadFlow.targetNullValue = 0;//custumer set zeropoint
	pRegHoldingWrap->pZeroAndReadFlow.zeroStatus = 0;//set current to zero in 1,need to change to 0
	 
	pRegHoldingWrap->pValveCommand.valveCommand = emValveClose;// valve off,2 valve on,0,valve pid
	pRegHoldingWrap->pValveCommand.valveCommandMode=0;// not function
	pRegHoldingWrap->pValveCommand.valveType=0;// normal on type
	pRegHoldingWrap->pValveCommand.valveVoltage=0;// not function
	
	pRegHoldingWrap->pWarningsAlarms.clearWarningsAlarms=0;// not function
	pRegHoldingWrap->pWarningsAlarms.enableWarningsAlarms=0;// not function
	 
	
	strcpy((char*)&pRegHoldingWrap->pProduct.firmwareRevision,"01.01");
	strcpy((char*)&pRegHoldingWrap->pProduct.PCBRevision,"01.01");
	strcpy((char*)&pRegHoldingWrap->pProduct.modelID,"3687");
	strcpy((char*)&pRegHoldingWrap->pProduct.MFCSeiral,"255879");
	strcpy((char*)&pRegHoldingWrap->pProduct.manufacturingDate,"19/01/10");
	strcpy((char*)&pRegHoldingWrap->pProduct.calibrationDate,"19/01/10");
	pRegHoldingWrap->pProduct.productVersion=10;
	strcpy((char*)&pRegHoldingWrap->pProduct.productName,"MFCc51");
	strcpy((char*)&pRegHoldingWrap->pProduct.manufacturer,"Maice");
	
	strcpy((char*)&pRegHoldingWrap->pCalibrate.targetGasName,"N2");
	pRegHoldingWrap->pCalibrate.targetGasCode=13;
	pRegHoldingWrap->pCalibrate.targetGasFullScaleRange=500;
	pRegHoldingWrap->pCalibrate.tarGasConversionFactor=1*65536;//fix16.16
	
	strcpy((char*)&pRegHoldingWrap->pCalibrate.CalibrationGasName,"N2");
	pRegHoldingWrap->pCalibrate.CalibrationGasCode=13;
	pRegHoldingWrap->pCalibrate.CalibrationGasFullScaleRange=500;
	pRegHoldingWrap->pCalibrate.CalGasConversionFactor=1*65536;//fix16.16
	
	pRegHoldingWrap->pMacBaudrate.RS485MacAddress=0x20;
	pRegHoldingWrap->pMacBaudrate.baudrate=9600;
	pRegHoldingWrap->pMacBaudrate.MBmode=0;//RTU
	pRegHoldingWrap->pMacBaudrate.IRRCutoff=5;
	  
	SetContrlResource(pRegHoldingWrap->pControlMode.controlMode);
	accumulatorStatu = emAccumulatorRunning;
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
	
	EE_ReadVariable16bits(VirtAddVarTab[290], &data_x);
	if(data_x != 290)
	{
		EE_WriteVariable16bits(VirtAddVarTab[290], 290);
		EEPROM_SAVE();//app run in the first time,storage the default value to ROM
	}else{
		EEPROM_READ();//get the last save parameter from the ROM
	}
	HAL_FLASH_Lock();
	
	SetContrlResource(pRegHoldingWrap->pControlMode.defaultCotrolMode);
	pRegHoldingWrap->pValveCommand.valveCommand = emValveClose;
	Valve_Close();
 
}
uint8_t GetAccumulatorStatu()
{
	return  accumulatorStatu;//0 stop,1 pause,2 running;
}
void HolddingRegDataChange(void)
{
	//accumulator change 0 restart,1:pause,3,resume,4,start continue flag
	if(pRegHoldingWrap->pZeroAndReadFlow.accumulatorMode ==0)//reset
	{	
		pRegHoldingWrap->pZeroAndReadFlow.accumulatorMode=3;
		ResetFlowAccumulator();	
		accumulatorStatu = emAccumulatorRunning;
	}else if(pRegHoldingWrap->pZeroAndReadFlow.accumulatorMode ==1)//pause
	{
		accumulatorStatu = emAccumulatorPause;
	}else if(pRegHoldingWrap->pZeroAndReadFlow.accumulatorMode ==3)//resume
	{
		accumulatorStatu = emAccumulatorRunning;
	}
	
	//contrlmode change
	SetContrlResource(pRegHoldingWrap->pControlMode.controlMode);
	//pid parameter change
	FuzzyRuleInit();
	//eeprom change
	if(pRegHoldingWrap->pControlMode.saveEEPROM ==1)
	{
		pRegHoldingWrap->pControlMode.saveEEPROM=0;
		EEPROM_SAVE();
	}
	if(pRegHoldingWrap->pControlMode.systemReset ==1)
	{
		pRegHoldingWrap->pControlMode.systemReset=0;
		NVIC_SystemReset();
	}
	
	// valve mode change
	if(pRegHoldingWrap->pValveCommand.valveCommand ==emValveClose){
		Valve_Close();
	}else if(pRegHoldingWrap->pValveCommand.valveCommand ==emValveOpen){
		if(accumulatorStatu == emAccumulatorRunning)
			accumulatorStatu = emAccumulatorPauseByOpenValve;
		Valve_Open();
	}else if(pRegHoldingWrap->pValveCommand.valveCommand ==emValvePID){
		if(accumulatorStatu == emAccumulatorPauseByOpenValve)
			accumulatorStatu = emAccumulatorRunning;
	}
	//zero change
	if(pRegHoldingWrap->pZeroAndReadFlow.zeroStatus ==1){
		ResetFlowOffset();
		pRegHoldingWrap->pZeroAndReadFlow.zeroStatus =0;
	}
	//digital setpoint change
	if(*Voltage_Set_PointCur !=lastVoltage_Set_Point ){
		if(pRegHoldingWrap->pSetPoint.holdFollow ==emFollowSetPoint){
			if(pRegHoldingWrap->pSetPoint.delay<49 && pRegHoldingWrap->pSetPoint.delay>1){
				osDelay(100);
			}else{
				osDelay(pRegHoldingWrap->pSetPoint.delay);
			}
			lastVoltage_Set_Point = *Voltage_Set_PointCur ;
			flow_Set_PointGasFit =100*UFRAC16ToFloat(*Voltage_Set_PointCur)/(pRegHoldingWrap->pCalibrate.tarGasConversionFactor/65536.0f);//设定的时候要除 %
			if(flow_Set_PointGasFit<UFRAC16ToFloat(pRegHoldingWrap->pSetPoint.shutoffLevel)*100){
				pRegHoldingWrap->pValveCommand.valveCommand =emValveClose;
				Valve_Close();
			}else{	
				Voltage_Set_PointLinearFit = FlowToVoltage(flow_Set_PointGasFit);
				pRegInputWrap->voltageSetPoint = Voltage_Set_PointLinearFit;
				
				if(UFRAC16ToFloat(pRegHoldingWrap->pSetPoint.softStartRate)>0 &&((Voltage_Set_PointLinearFit-GetADCVoltage(0))>50)){
					StartSoftStartTimer(Voltage_Set_PointLinearFit,UFRAC16ToFloat(pRegHoldingWrap->pSetPoint.softStartRate));
				}else{
				setVoltageSetPoint(Voltage_Set_PointLinearFit);
				}
				
			}
		}
	}
	//Calculate_FilteringCoefficient
	Calculate_FilteringCoefficient(pRegHoldingWrap->pMacBaudrate.IRRCutoff);

}
void StartSoftStartTimer(uint16_t Voltage_Set_PointLinearFit,float rateFSpsc)
{
	float currentRate = Voltage_Set_PointLinearFit/50.0f;
	
	softStartCounter = currentRate/rateFSpsc;
  uint16_t targetVoltage = GetADCVoltage(0)+rateFSpsc*5000;
	if(targetVoltage<Voltage_Set_PointLinearFit){
	setVoltageSetPoint(targetVoltage);
	}else{
	setVoltageSetPoint(Voltage_Set_PointLinearFit);
	}

}
void SoftStartCountintDown(void)
{
	//softStartCounter--;
	uint16_t targetVoltage = GetADCVoltage(0)+UFRAC16ToFloat(pRegHoldingWrap->pSetPoint.softStartRate)*5000;
	if(targetVoltage<Voltage_Set_PointLinearFit){
	setVoltageSetPoint(targetVoltage);
	}else{
	softStartCounter=0;
	setVoltageSetPoint(Voltage_Set_PointLinearFit);		
	}
	
}
uint8_t GetSoftStartCounter()
{
	
	return softStartCounter;
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
			Voltage_Set_PointCur=&pRegHoldingWrap->pSetPoint.digitalSetpoint;//ufrat16
					
		break;
		case 2:
			Voltage_Set_PointCur = &pRegInputWrap->voltageCh1;//ufrat16
		break;
		
		default:
			Voltage_Set_PointCur=&pRegHoldingWrap->pSetPoint.digitalSetpoint;//ufrat16
		break;
	}
}
void EEPROM_SAVE(void)	 
{
	uint16_t * pbuf = (uint16_t *)pRegHoldingWrap;
	HAL_FLASH_Unlock();
	
	for(int i=0;i<S_REG_HOLDING_NREGS;i++){
		EE_WriteVariable16bits(VirtAddVarTab[i], *(pbuf++));
	}
	HAL_FLASH_Lock();
}
void EEPROM_READ(void)	// 
{
  uint16_t * pbuf = (uint16_t *)pRegHoldingWrap;
	HAL_FLASH_Unlock();
	for(int i=0;i<S_REG_HOLDING_NREGS;i++){
		EE_ReadVariable16bits(VirtAddVarTab[i], pbuf++);
	}
  HAL_FLASH_Lock();  
}

float piecewiselinearinterp(struct TLinearFittingValue * xDict,struct TLinearFittingValue * yDict,uint16_t DictSize,float xInput)
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
	return piecewiselinearinterp(&pRegHoldingWrap->pLinearFittingY,sLinearFittingX,11,voltage)/50;
}
float FlowToVoltage(float flow)//real Target voltage cover to current MFC sensor Voltage
{
	return piecewiselinearinterp(sLinearFittingX,&pRegHoldingWrap->pLinearFittingY,11,flow*50);
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
	return pRegHoldingWrap->pZeroAndReadFlow.targetNullValue/65536.0f;
}
void SevenStarExecute(uint8_t * pucFrame, uint16_t *usLength)
{
	uint8_t dataClass = pucFrame[4];
	uint8_t dataAttribute = pucFrame[6];

	switch (dataClass){
		case 0x69:// control mode & setpoint
			switch (dataAttribute){
				case 0x03:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pControlMode.controlMode,usLength,pucFrame);
				break;
				case 0x04:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pControlMode.defaultCotrolMode,usLength,pucFrame);
				break;
				case 0x06:
					pRegHoldingWrap->pControlMode.saveEEPROM = pucFrame[7];
				break;
				case 0x05:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pSetPoint.holdFollow,usLength,pucFrame);
				break;
				case 0xA6:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pSetPoint.delay,usLength,pucFrame);
				break;
				case 0xA4:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pSetPoint.digitalSetpoint,usLength,pucFrame);
				break;
				case 0xA5:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pSetPoint.digitalSetpoint,usLength,pucFrame);
				break;				
			}
		break;
		case 0x6A:// setpoint &valve cmd
			switch (dataAttribute){
				case 0xA4:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pSetPoint.softStartRate,usLength,pucFrame);
				break;
				case 0xA2:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pSetPoint.shutoffLevel,usLength,pucFrame);
				break;
				case 0xA1:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pValveCommand.valveCommandMode,usLength,pucFrame);
				break;
				case 0x01:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pValveCommand.valveCommand,usLength,pucFrame);
				break;	
				case 0x91:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pValveCommand.valveVoltage,usLength,pucFrame);
				break;	
				case 0x9C:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pValveCommand.valveType,usLength,pucFrame);
				break;					
			}
		break;
		case 0x68:// zero
			switch (dataAttribute){
				case 0xBA:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pZeroAndReadFlow.zeroStatus,usLength,pucFrame);
				break;
				case 0xB9:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pZeroAndReadFlow.readFlow,usLength,pucFrame);
				break;				 			
			}
		break;
		case 0xA4:// Accumulator
			switch (dataAttribute){
				case 0x05:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pZeroAndReadFlow.accumulatorMode,usLength,pucFrame);
				break;
				case 0x03:
					saveSevenStarUINT32DataToMBHoldingReg((uint32_t*)&pRegHoldingWrap->pZeroAndReadFlow.accumulatorFlow,usLength,pucFrame);
				break;				 			
			}
			break;
		case 0x65:// warning
			switch (dataAttribute){
				case 0xA2:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pWarningsAlarms.enableWarningsAlarms,usLength,pucFrame);
				break;
				case 0xA1:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pWarningsAlarms.clearWarningsAlarms,usLength,pucFrame);
				break;	
				case 0xA0:
					//saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pWarningsAlarms.none,usLength,pucFrame);
				break;					
			}
		break;
		case 0x01:// Product
			switch (dataAttribute){
				case 0x07:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.productName,usLength,pucFrame,32);
				
				break;
				case 0x04:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pProduct.productVersion,usLength,pucFrame);
				break;					
			}
		break;
	  case 0x64:// Product
			switch (dataAttribute){
				case 0x03:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.manufacturer,usLength,pucFrame,32);
				break;
				case 0x04:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.modelID,usLength,pucFrame,16);
				break;	
				case 0x05:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.firmwareRevision,usLength,pucFrame,8);
				break;	
				case 0x06:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.PCBRevision,usLength,pucFrame,8);
				break;	
				case 0x07:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.MFCSeiral,usLength,pucFrame,16);
				break;	
				case 0x0a:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.manufacturingDate,usLength,pucFrame,16);
				break;	
				case 0x0c:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pProduct.calibrationDate,usLength,pucFrame,16);
				break;					
			}
		break;
		case 0x66:// Calibrate
			switch (dataAttribute){
				case 0x01:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pCalibrate.targetGasName,usLength,pucFrame,32);
				break;
				case 0x02:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pCalibrate.targetGasCode,usLength,pucFrame);
				break;	
				case 0x03:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pCalibrate.targetGasFullScaleRange,usLength,pucFrame);
				break;	
				case 0x04:
					saveSevenStarUINT32DataToMBHoldingReg(&pRegHoldingWrap->pCalibrate.tarGasConversionFactor,usLength,pucFrame);
				break;	
				case 0x06:
					saveSevenStarTEXTxDataToMBHoldingRegUINT8(pRegHoldingWrap->pCalibrate.CalibrationGasName,usLength,pucFrame,32);
				break;	
				case 0x07:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pCalibrate.CalibrationGasCode,usLength,pucFrame);
				break;	
				case 0x08:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pCalibrate.CalibrationGasFullScaleRange,usLength,pucFrame);
				break;	
				case 0x09:
					saveSevenStarUINT32DataToMBHoldingReg(&pRegHoldingWrap->pCalibrate.CalGasConversionFactor,usLength,pucFrame);
				break;					
			}
		break;
		case 0xA1:// Calibrate
			switch (dataAttribute){
				case 0x07:
					saveSevenStarUINT32DataToMBHoldingReg(&pRegHoldingWrap->pZeroAndReadFlow.targetNullValue,usLength,pucFrame);
				break;
			}
		break;
		case 0x03:// Calibrate
			switch (dataAttribute){
				case 0x01:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pMacBaudrate.RS485MacAddress,usLength,pucFrame);
				break;
				case 0x02:
					saveSevenStarUINT16DataToMBHoldingReg(&pRegHoldingWrap->pMacBaudrate.baudrate,usLength,pucFrame);
				break;
				case 0x03:
					saveSevenStarUINT8DataToMBHoldingRegUINT16(&pRegHoldingWrap->pMacBaudrate.MBmode,usLength,pucFrame);
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
void saveSevenStarTEXTxDataToMBHoldingRegUINT8(uint8_t * MBHoldRegAddress,uint16_t *usLength,uint8_t * pucFrame,uint8_t textSize)
{
if (pucFrame[2]== 0x80)//read
	{
		*usLength =textSize;
		pucFrame[3]=3+textSize;
		memcpy(pucFrame, MBHoldRegAddress,textSize);
	}
	if (pucFrame[2]== 0x81)//write
	{
		memcpy(MBHoldRegAddress,pucFrame ,textSize);
	}
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
