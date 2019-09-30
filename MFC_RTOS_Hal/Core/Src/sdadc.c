/**
  ******************************************************************************
  * File Name          : SDADC.c
  * Description        : This file provides code for the configuration
  *                      of the SDADC instances.
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
#include "sdadc.h"

/* USER CODE BEGIN 0 */

#define ADCMeanWindow  1024//1024//偶数

#define ADCMeanWindowShift  10//偶数

int16_t bufLength = ADCMeanWindow*2;

int16_t SDADC_ValueTable[ADCMeanWindow*2]={0};
 
struct _VoltageRaw voltage;
struct _Voltage filter_voltage;
struct _VoltageSum sum_voltage;
uint16_t valtageOffset=0;
float VOL_IIR_FACTOR;
float FIRWindowPass1[10];
float FIRWindowPass2[10];
float FIRFilterResult1=0.0;
float FIRFilterResult2=0.0;
int8_t FIRFilterIndex1=0;
int8_t FIRFilterIndex2=0;
/* USER CODE END 0 */

SDADC_HandleTypeDef hsdadc1;
DMA_HandleTypeDef hdma_sdadc1;

/* SDADC1 init function */
void MX_SDADC1_Init(void)
{
  SDADC_ConfParamTypeDef ConfParamStruct = {0};

  /** Configure the SDADC low power mode, fast conversion mode,
  slow clock mode and SDADC1 reference voltage 
  */
  hsdadc1.Instance = SDADC1;
  hsdadc1.Init.IdleLowPowerMode = SDADC_LOWPOWER_NONE;
  hsdadc1.Init.FastConversionMode = SDADC_FAST_CONV_DISABLE;
  hsdadc1.Init.SlowClockMode = SDADC_SLOW_CLOCK_ENABLE;
  hsdadc1.Init.ReferenceVoltage = SDADC_VREF_EXT;
  hsdadc1.InjectedTrigger = SDADC_SOFTWARE_TRIGGER;
  if (HAL_SDADC_Init(&hsdadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the Injected Mode 
  */
  if (HAL_SDADC_SelectInjectedDelay(&hsdadc1, SDADC_INJECTED_DELAY_NONE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SDADC_SelectInjectedTrigger(&hsdadc1, SDADC_SOFTWARE_TRIGGER) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SDADC_InjectedConfigChannel(&hsdadc1, SDADC_CHANNEL_7|SDADC_CHANNEL_8, SDADC_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }
  /** Set parameters for SDADC configuration 0 Register 
  */
  ConfParamStruct.InputMode = SDADC_INPUT_MODE_SE_ZERO_REFERENCE;
  ConfParamStruct.Gain = SDADC_GAIN_1;
  ConfParamStruct.CommonMode = SDADC_COMMON_MODE_VSSA;
  ConfParamStruct.Offset = 0;
  if (HAL_SDADC_PrepareChannelConfig(&hsdadc1, SDADC_CONF_INDEX_0, &ConfParamStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the Regular Channel 
  */
  if (HAL_SDADC_AssociateChannelConfig(&hsdadc1, SDADC_CHANNEL_7, SDADC_CONF_INDEX_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the Injected Channel 
  */
  if (HAL_SDADC_AssociateChannelConfig(&hsdadc1, SDADC_CHANNEL_8, SDADC_CONF_INDEX_0) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SDADC_MspInit(SDADC_HandleTypeDef* sdadcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(sdadcHandle->Instance==SDADC1)
  {
  /* USER CODE BEGIN SDADC1_MspInit 0 */
	HAL_PWREx_EnableSDADC(PWR_SDADC_ANALOG1);
  /* USER CODE END SDADC1_MspInit 0 */
    /* SDADC1 clock enable */
    __HAL_RCC_SDADC1_CLK_ENABLE();
  
    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**SDADC1 GPIO Configuration    
    PE8     ------> SDADC1_AIN8P
    PE9     ------> SDADC1_AIN7P 
    */
    GPIO_InitStruct.Pin = Sensor_Pin|SetPoint_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* SDADC1 DMA Init */
    /* SDADC1 Init */
    hdma_sdadc1.Instance = DMA2_Channel3;
    hdma_sdadc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdadc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdadc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdadc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_sdadc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_sdadc1.Init.Mode = DMA_CIRCULAR;
    hdma_sdadc1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_sdadc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(sdadcHandle,hdma,hdma_sdadc1);

  /* USER CODE BEGIN SDADC1_MspInit 1 */

  /* USER CODE END SDADC1_MspInit 1 */
  }
}

void HAL_SDADC_MspDeInit(SDADC_HandleTypeDef* sdadcHandle)
{

  if(sdadcHandle->Instance==SDADC1)
  {
  /* USER CODE BEGIN SDADC1_MspDeInit 0 */

  /* USER CODE END SDADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SDADC1_CLK_DISABLE();
  
    /**SDADC1 GPIO Configuration    
    PE8     ------> SDADC1_AIN8P
    PE9     ------> SDADC1_AIN7P 
    */
    HAL_GPIO_DeInit(GPIOE, Sensor_Pin|SetPoint_Pin);

    /* SDADC1 DMA DeInit */
    HAL_DMA_DeInit(sdadcHandle->hdma);
  /* USER CODE BEGIN SDADC1_MspDeInit 1 */

  /* USER CODE END SDADC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

void SDADC_Config()
{
	HAL_SDADC_CalibrationStart(&hsdadc1, SDADC_CALIBRATION_SEQ_1);
 
  if (HAL_SDADC_PollForCalibEvent(&hsdadc1, 50) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_SDADC_SelectInjectedTrigger(&hsdadc1, SDADC_SOFTWARE_TRIGGER) != HAL_OK)
  {
    Error_Handler();
  }
	HAL_SDADC_InjectedStart_DMA(&hsdadc1, (uint32_t*)SDADC_ValueTable,bufLength);
	Calculate_FilteringCoefficient(50);
}
void Calculate_FilteringCoefficient(float Cut_Off) 
 { 
 	VOL_IIR_FACTOR =(float)( 10 /( 10 + 1.0f/(2.0f*3.14f*Cut_Off) )); 
 } 

void VOL_IIR_Filter()
{
	uint16_t i = 0;
	int16_t temp = 0;
	sum_voltage.ch0 = 0;
	sum_voltage.ch1 = 0;
	
	for(i=0;i<ADCMeanWindow;i++){
		sum_voltage.ch0 +=SDADC_ValueTable[2*i+1];
		sum_voltage.ch1 +=SDADC_ValueTable[2*i+0];
	}
	
	temp =sum_voltage.ch0>>ADCMeanWindowShift;
  filter_voltage.ch0=(float)((((temp + 32767) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));//raw
	temp =sum_voltage.ch1>>ADCMeanWindowShift;
	filter_voltage.ch1=(float)((((temp + 32767) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));//raw
  
	REG_INPUTsAddr->flowRawCh0= VoltageToFlow(filter_voltage.ch0); //linear fited %
	REG_INPUTsAddr->flowRealCh0 = (float)(REG_INPUTsAddr->flowRawCh0-REG_INPUTsAddr->flowOffsetCh0-GetTargetNullFlow())*1;//sCalibrate->tarGasConversionFactor;//target gas fited: read 
	
	//FIR fliter for ch0
	FIRFilterResult1 -=FIRWindowPass1[FIRFilterIndex1]; //pass1
	FIRWindowPass1[FIRFilterIndex1] = REG_INPUTsAddr->flowRealCh0/10;
	FIRFilterResult1 +=FIRWindowPass1[FIRFilterIndex1];
	FIRFilterIndex1 = (FIRFilterIndex1+1)%10;
	
	FIRFilterResult2 -=FIRWindowPass2[FIRFilterIndex2]; //pass2
	FIRWindowPass2[FIRFilterIndex2] = FIRFilterResult1/10;
	FIRFilterResult2 +=FIRWindowPass2[FIRFilterIndex2];
	FIRFilterIndex2 = (FIRFilterIndex2+1)%10;
	
	//IIR fliter for ch0
	//REG_INPUTsAddr->flowIIRFilterCh0 = (float)(REG_INPUTsAddr->flowIIRFilterCh0 + VOL_IIR_FACTOR*(REG_INPUTsAddr->flowRealCh0 - REG_INPUTsAddr->flowIIRFilterCh0)); 
	filter_voltage.ch0 = FIRFilterResult2*50;
	sZeroAndReadFlow->readFlow=FloatToUFRAC16(FIRFilterResult2/100);//digital read ported for Sevenstart
	REG_INPUTsAddr->voltageCh0=sZeroAndReadFlow->readFlow;//modbus
	
	AD5761_SetVoltage(FIRFilterResult2*50);// DAC output
	 	
	//IRR fliter for ch1,prepare external control signal
	REG_INPUTsAddr->flowRawCh1=filter_voltage.ch1/50;// control flow 0-100% input voltage is stable,no need to filter
	REG_INPUTsAddr->voltageCh1=FloatToUFRAC16(REG_INPUTsAddr->flowRawCh1/100);//control电压----来自流量检测的DAC输入,保存为UFRAT16 小数点格式
	
	
	//debug
	REG_INPUTsAddr->pwmOut=PWM_Output;
}
void ResetFlowAccumulator(void)
{
	sZeroAndReadFlow->accumulatorFlow=0;
}
void FlowAccumulator(void)//1pass/sec
{
	if(sZeroAndReadFlow->accumulatorMode==1){
		//sZeroAndReadFlow->accumulatorFlow+=filter_voltage.ch0/3000;
	}
}
void ResetFlowOffset(void)
{
	REG_INPUTsAddr->flowOffsetCh0 = REG_INPUTsAddr->flowRawCh0;
}
uint16_t GetADCVoltage(uint8_t ch)
{
	
	if (ch==0)
		return filter_voltage.ch0;// pid not filter
	else
		return filter_voltage.ch1;
}
void setValtageOffset(void)
{
	valtageOffset = filter_voltage.ch0;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
