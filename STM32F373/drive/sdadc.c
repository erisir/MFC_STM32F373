#include "sdadc.h"
#include "delay.h"
#include <stdio.h>
#define SDADC1_DR_Address             0x40016060
 
// SDADC转换的电压值通过inject方式传到SRAM
int16_t InjectedConvData[2]={0};

int16_t SDADC_RegularConvertedValueTab[100];
 
static float VOL_IIR_FACTOR;
struct _Voltage voltage;
struct _Voltage filter_voltage;
/******************************************************************************
函数原型：	void Calculate_FilteringCoefficient(float Time, float Cut_Off)
功    能：	iir低通滤波参数计算
*******************************************************************************/ 
void Calculate_FilteringCoefficient(float Time, float Cut_Off)
{
	VOL_IIR_FACTOR = Time /( Time + 1/(2.0f*3.14*Cut_Off) );
}

/******************************************************************************
函数原型：	void ACC_IIR_Filter(struct _acc *Acc_in,struct _acc *Acc_out)
功    能：	iir低通滤波
*******************************************************************************/ 
void VOL_IIR_Filter()
{
	ADC_Mean();
  voltage.ch0 = 2* (((InjectedConvData[0] + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL));
	voltage.ch1 = 2* (((InjectedConvData[1] + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL));
	
	filter_voltage.ch0 = (float)(filter_voltage.ch0 + VOL_IIR_FACTOR*(voltage.ch0 - filter_voltage.ch0)); 
	filter_voltage.ch1 = (float)(filter_voltage.ch1 + VOL_IIR_FACTOR*(voltage.ch1 - filter_voltage.ch1)); 
}

float  GetADCVoltage(unsigned char ch){//PID调用
	 if(ch == 0)
		 return filter_voltage.ch0;
	 else
		 return filter_voltage.ch1;
}

void ADC_Mean(void) {
	int i = 0;
	int32_t sum=0,sum1=0;
	for(i=0;i<50;i++){
		sum +=( int32_t)SDADC_RegularConvertedValueTab[i*2];	
		sum1 +=( int32_t)SDADC_RegularConvertedValueTab[i*2+1];	
	}
	InjectedConvData[1]=(int16_t)(sum/50); 
	InjectedConvData[0]=(int16_t)(sum1/50); 
}
 
uint32_t SDADC1_Config(void)
{
	
  SDADC_AINStructTypeDef SDADC_AINStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  uint32_t SDADCTimeout = 0;
  /* POT_SDADC APB2 interface clock enable */
  RCC_APB2PeriphClockCmd(POT_SDADC_CLK, ENABLE);
  
  /* PWR APB1 interface clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Enable POT_SDADC analog interface */
  PWR_SDADCAnalogCmd(POT_SDADC_PWR, ENABLE);

  /* Set the SDADC divider: The SDADC should run @0.5~6MHz */
  /* If Sysclk is 72MHz, SDADC divider should be 12 */
  RCC_SDADCCLKConfig(RCC_SDADCCLK_SYSCLK_Div48);

  /* POT_GPIO_CLK Peripheral clock enable */
  RCC_AHBPeriphClockCmd(POT_GPIO_CLK, ENABLE);

  /* POT_SDADC channel 5P (PB1) */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = POT_GPIO_PIN;
  GPIO_Init(POT_GPIO_PORT, &GPIO_InitStructure);

  /* Select External reference: The reference voltage selection is available
     only in SDADC1 and therefore to select the VREF for SDADC2/SDADC3, SDADC1
     clock must be already enabled */
  SDADC_VREFSelect(POT_SDADC_VREF);

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
  DMA_Config();
	SDADC_DMAConfig(SDADC1,SDADC_DMATransfer_Injected, ENABLE);//DMA enable
 
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

  /* Enable end of injected conversion interrupt */
  //SDADC_ITConfig(POT_SDADC, SDADC_IT_JEOC, ENABLE);
  /* Start a software start conversion */
  SDADC_SoftwareStartInjectedConv(POT_SDADC);
  
  return 0;
}
void DMA_Config(void)
{
DMA_InitTypeDef   DMA_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);

DMA_DeInit(DMA2_Channel3); //

/* DMA channel1 configuration ----------------------------------------------*/
DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SDADC1_DR_Address;
DMA_InitStructure.DMA_MemoryBaseAddr =(u32)& SDADC_RegularConvertedValueTab;
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_BufferSize = 100;
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
/*********************************************END OF FILE**********************/
