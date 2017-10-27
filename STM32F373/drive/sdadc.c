#include "sdadc.h"
#include "delay.h"
#include "rs485.h" 
#include <stdio.h>
#define SDADC1_DR_Address             0x40016060
#define ADCMeanWindow  200//偶数
#define ADCMeanFacor   500500l
// SDADC转换的电压值通过inject方式传到SRAM
int16_t InjectedConvData[2]={0};
int16_t SDADC_ValueTab[ADCMeanWindow*2]={0};
uint16_t ADC_ValueTab[ADCMeanWindow*3]={0};	

 
int16_t VoltageData[2]={0};
int16_t VoltageDataRaw[2]={0};
int16_t VoltageInterRef = 0;

int16_t VoltageFilterBufCh0[ADCMeanWindow]={0};
int16_t VoltageFilterBufCh1[ADCMeanWindow]={0};

int8_t VoltageFilterCounter = 0;


float SapmleTime = 0.01f;
float VOL_IIR_FACTOR;
struct _Voltage voltage;
struct _Voltage filter_voltage;
/******************************************************************************
函数原型：	void Calculate_FilteringCoefficient(float Time, float Cut_Off)
功    能：	iir低通滤波参数计算
*******************************************************************************/ 
void Calculate_FilteringCoefficient(float Cut_Off)
{
	VOL_IIR_FACTOR =(float)( SapmleTime /( SapmleTime + 1/(2.0f*3.14*Cut_Off) ));
}

void updateVoltageWindowBuf(void)//100Hz
{
	 
	ADC_Mean();
	VoltageFilterBufCh0[VoltageFilterCounter] =VoltageDataRaw[0];
	VoltageFilterBufCh1[VoltageFilterCounter] =VoltageDataRaw[1];

	VoltageFilterCounter++;
	if(VoltageFilterCounter==ADCMeanWindow)
		VoltageFilterCounter = 0;
}
void updateRawData(void)//取中值 10Hz
{
	uint8_t i;
	int32_t temp0=0,temp1=0;
	ADC_Mean();
	/*for (i=0;i<ADCMeanWindow;i++) 
	{ 
		temp0 +=VoltageFilterBufCh0[i]*(i+1);
		temp1 +=VoltageFilterBufCh1[i]*(i+1);	 
	} */
	
	VoltageData[0]=VoltageDataRaw[0];//temp0/ADCMeanFacor;
	VoltageData[1]=VoltageDataRaw[1];//temp1/ADCMeanFacor;
	
}
void ADC_Mean(void) {
	int i = 0;
	int32_t sum0=0,sum1=0,sum2=0;
	for(i=0;i<ADCMeanWindow;i++){
		#ifdef __ADC_DMA_MODE_
		sum1  +=( int32_t)SDADC_ValueTab[i*2];	
		sum0 +=( int32_t)SDADC_ValueTab[i*2+1];	
		#endif
		
		#ifdef __ADC_INJECTED_MODE_
		sum0 +=( int32_t)InjectedConvData[0];	
		sum1 +=( int32_t)InjectedConvData[1];	
		#endif
		
		#ifdef __ADC_ADC1_MODE_
		sum0 +=( int32_t)ADC_ValueTab[i*3];	
		sum1 +=( int32_t)ADC_ValueTab[i*3+1];	
		sum2 +=( int32_t)ADC_ValueTab[i*3+2];	
		#endif
	}
	VoltageDataRaw[0]=(int16_t)(sum0/ADCMeanWindow); 
	VoltageDataRaw[1]=(int16_t)(sum1/ADCMeanWindow); 	
	VoltageInterRef = (int16_t)(sum2/ADCMeanWindow); 
}
void VOL_IIR_Filter()
{
	updateRawData();
	#ifdef __ADC_DMA_MODE_
  voltage.ch0 = (float)(2.0f* (((VoltageData[0] + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	voltage.ch1 = (float)(2.0f* (((VoltageData[1] + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	#endif
	
	#ifdef __ADC_INJECTED_MODE_
  voltage.ch0 = (float)(2.0f* (((VoltageData[0] + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	voltage.ch1 = (float)(2.0f* (((VoltageData[1] + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	#endif
	
	#ifdef __ADC_ADC1_MODE_
	voltage.ch0 = (float)(2.0f*1000*VoltageData[0]/VoltageInterRef*1.2f);
	voltage.ch1 = (float)(2.0f*1000*VoltageData[1]/VoltageInterRef*1.2f);
	#endif
	
	filter_voltage.ch0 = (float)(filter_voltage.ch0 + VOL_IIR_FACTOR*(voltage.ch0 - filter_voltage.ch0)); 
	filter_voltage.ch1 =(float)(filter_voltage.ch1 + VOL_IIR_FACTOR*(voltage.ch1 - filter_voltage.ch1)); 
}

float  GetADCVoltage(unsigned char ch){//PID调用
	if(ch == 0)
		 return filter_voltage.ch0;
	 else
		 return filter_voltage.ch1;
}
 

 
void ADC1_Init(void)
{
	ADC_GPIO_Config();
 
	#ifdef __ADC_ADC1_MODE_
	ADC1_Mode_Config();
	#endif
	
	#ifdef __ADC_INJECTED_MODE_
	SDADC1_Config();
	#endif
	
	#ifdef __ADC_DMA_MODE_
	SDADC1_Config();
	#endif
}

void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	  /* POT_SDADC channel 5P (PB1) */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC<14M
	
	DMA_DeInit(DMA1_Channel1); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ValueTab; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructure.DMA_BufferSize = ADCMeanWindow*3; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 

 
	DMA_Cmd(DMA1_Channel1, ENABLE);
//////////////////////////////////////////////////////////////////////////////////////////////
	ADC_DeInit(ADC1);  
	ADC_TempSensorVrefintCmd(ENABLE); 
 
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 3; 
	ADC_Init(ADC1, &ADC_InitStructure);

 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);	 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);	 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17,3, ADC_SampleTime_239Cycles5);	//inter reference(1.2V)	
	
 
	ADC_DMACmd(ADC1, ENABLE);
 
	ADC_Cmd(ADC1, ENABLE);
 
	ADC_ResetCalibration(ADC1);
 
	while(ADC_GetResetCalibrationStatus(ADC1));
 
	ADC_StartCalibration(ADC1);
 
	while(ADC_GetCalibrationStatus(ADC1));
 
	ADC_SoftwareStartConv(ADC1);
}
uint32_t SDADC1_Config(void)
{
	
  SDADC_AINStructTypeDef SDADC_AINStructure;
  
  
  uint32_t SDADCTimeout = 0;
  /* POT_SDADC APB2 interface clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SDADC1, ENABLE);
  
  /* PWR APB1 interface clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Enable POT_SDADC analog interface */
  PWR_SDADCAnalogCmd(PWR_SDADCAnalog_1, ENABLE);

  /* Set the SDADC divider: The SDADC should run @0.5~6MHz */
  /* If Sysclk is 72MHz, SDADC divider should be 12 */
  RCC_SDADCCLKConfig(RCC_SDADCCLK_SYSCLK_Div48);


  /* Select External reference: The reference voltage selection is available
     only in SDADC1 and therefore to select the VREF for SDADC2/SDADC3, SDADC1
     clock must be already enabled */
  SDADC_VREFSelect(SDADC_VREF_Ext);

  /* Insert delay equal to ~5 ms */
  delay_ms(5);
  /* Enable POT_SDADC */
  SDADC_Cmd(POT_SDADC, ENABLE);

  /* Enter initialization mode */
  SDADC_InitModeCmd(POT_SDADC, ENABLE);
  SDADCTimeout = SDADC_INIT_TIMEOUT;
  /* wait for INITRDY flag to be set */
  while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_INITRDY) == RESET) && (--SDADCTimeout != 0));

  if(SDADCTimeout == 0)
  {
    /* INITRDY flag can not set */
    return 1;
  }

  /* Analog Input configuration conf0: use single ended zero reference */
  SDADC_AINStructure.SDADC_InputMode = SDADC_InputMode_SEZeroReference;
  SDADC_AINStructure.SDADC_Gain = POT_SDADC_GAIN;
  SDADC_AINStructure.SDADC_CommonMode = SDADC_CommonMode_VSSA;
  SDADC_AINStructure.SDADC_Offset = 0;
  SDADC_AINInit(POT_SDADC, SDADC_Conf_0, &SDADC_AINStructure);
  
	////////////////////////////////// Using DMA
	#ifdef __ADC_DMA_MODE_
	SDADC_DMA_Config();
	SDADC_DMAConfig(SDADC1,SDADC_DMATransfer_Injected, ENABLE);//DMA enable
	#endif
  //////////////////////////////////////
  /* select POT_SDADC channel 5 to use conf0 only one channel each time*/
	
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_5, SDADC_Conf_0);
	SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_6, SDADC_Conf_0);


  /* select channel(*) 5 */
	SDADC_InjectedChannelSelect(POT_SDADC, SDADC_Channel_5|SDADC_Channel_6);


  /* Enable continuous mode */
  SDADC_InjectedContinuousModeCmd(POT_SDADC, ENABLE);

  /* Exit initialization mode */
  SDADC_InitModeCmd(POT_SDADC, DISABLE);

 

  /* configure calibration to be performed on conf0 */
  SDADC_CalibrationSequenceConfig(POT_SDADC, SDADC_CalibrationSequence_1);
  /* start POT_SDADC Calibration */
  SDADC_StartCalibration(POT_SDADC);
  /* Set calibration timeout: 5.12 ms at 6 MHz in a single calibration sequence */
  SDADCTimeout = SDADC_CAL_TIMEOUT;
  /* wait for POT_SDADC Calibration process to end */
  while((SDADC_GetFlagStatus(POT_SDADC, SDADC_FLAG_EOCAL) == RESET) && (--SDADCTimeout != 0));
  
  if(SDADCTimeout == 0)
  {
    /* EOCAL flag can not set */
    return 2;
  }
	////////////////////////////////// Using injected
  /* Enable end of injected conversion interrupt */
	#ifdef __ADC_INJECTED_MODE_
  SDADC_ITConfig(POT_SDADC, SDADC_IT_JEOC, ENABLE);
	SDADC_NVIC_Config();
	#endif
	////////////////////////////////// 
  /* Start a software start conversion */
  SDADC_SoftwareStartInjectedConv(POT_SDADC);
  
  return 0;
}
void 	SDADC_NVIC_Config(void)
{	
	  /* NVIC Configuration */
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = SDADC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	}
void SDADC_DMA_Config(void)
{
DMA_InitTypeDef   DMA_InitStructure;
//NVIC_InitTypeDef NVIC_InitStructure;
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

DMA_DeInit(DMA2_Channel3); //

/* DMA channel1 configuration ----------------------------------------------*/
DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SDADC1_DR_Address;
DMA_InitStructure.DMA_MemoryBaseAddr =(u32)& SDADC_ValueTab;
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = ADCMeanWindow*2;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // DMA_MemoryInc_Disable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
DMA_Init(DMA2_Channel3, &DMA_InitStructure);


/*NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
				
DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);       
 */
/* Enable DMA channel1 */
DMA_Cmd(DMA2_Channel3, ENABLE);

} 

void SDADC1_IRQHandler(void)
 {
  uint32_t ChannelIndex;
  int16_t value = 0;
  if(SDADC_GetFlagStatus(SDADC1, SDADC_FLAG_JEOC) != RESET)
  {
    /* Get the converted value */
    value = SDADC_GetInjectedConversionValue(SDADC1, &ChannelIndex);
		if(ChannelIndex == 0x00050020)//5 PB1
			{
			InjectedConvData[1] = value;
		}
		if(ChannelIndex == 0x00060040)//6 PB0
			{
			InjectedConvData[0] = value;
		}
		 
  }
}
/*********************************************END OF FILE**********************/
