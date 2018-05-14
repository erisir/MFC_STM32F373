/**
  ******************************************************************************
  * @file    sdadc.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   sdadc
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "sdadc.h"
#include "mb.h"
 
#define SDADC1_DR_Address             0x40016060
#define ADCMeanWindow  1024//1024//偶数
#define ADCMeanWindowShift  10//偶数
 
int16_t SDADC_ValueTable[ADCMeanWindow*2]={0};
 
struct _VoltageRaw voltage;
struct _Voltage filter_voltage;
struct _VoltageSum sum_voltage;

 
 
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
  filter_voltage.ch0=(float)(2.0f* (((temp + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	temp =sum_voltage.ch1>>ADCMeanWindowShift;
	filter_voltage.ch1=(float)(2.0f* (((temp + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	 
	REG_INPUTsAddr->voltageCh0=(USHORT) (filter_voltage.ch0*10); 
	REG_INPUTsAddr->voltageCh1=(USHORT) (filter_voltage.ch1*10); 
}
 

float  GetADCVoltage(unsigned char ch){//PID调用
	if(ch == 0)
		 return filter_voltage.ch0;
	 else
		 return  filter_voltage.ch1;
}
 
void ADC1_Init(void)
{	 
	SDADC1_Config();
}
 
uint8_t SDADC1_Config(void)
{
	
  SDADC_AINStructTypeDef SDADC_AINStructure;
  DMA_InitTypeDef   DMA_InitStructure;
  
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
  SDADC_VREFSelect(SDADC_VREF_VDDA);

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
  
	
  /* select POT_SDADC channel 5 to use conf0 only one channel each time*/
	
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_8, SDADC_Conf_0);
	SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_7, SDADC_Conf_0);


  /* select channel(*) 5 */
	SDADC_InjectedChannelSelect(POT_SDADC, SDADC_Channel_8|SDADC_Channel_7);


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
	////////////////////////////////// 
  /* Start a software start conversion */
  SDADC_SoftwareStartInjectedConv(POT_SDADC);
  
	////////////////////////////////// Using DMA
	 
  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

	DMA_DeInit(DMA2_Channel3); //

	/* DMA channel1 configuration ----------------------------------------------*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SDADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)& SDADC_ValueTable;
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

	DMA_Cmd(DMA2_Channel3, ENABLE);
	SDADC_DMAConfig(SDADC1,SDADC_DMATransfer_Injected, ENABLE);//DMA enable
 
  //////////////////////////////////////
	
  return 0;
}
 

 
/*********************************************END OF FILE**********************/
