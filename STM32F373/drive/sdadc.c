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
 
#define SDADC1_DR_Address             0x40016060
#define ADCMeanWindow  1024//偶数
#define ADCMeanWindowShift  10//偶数
 
#define FilterWindowSHIFT 3   //
#define FilterWindowLen  10   //LEN=2^SHIFT+2 

#define FilterWindowSHIFTII 1   //
#define FilterWindowLenII  2   //LEN=2^SHIFT+2 

int16_t ch0VoltageBuf[FilterWindowLen];
int16_t ch1VoltageBuf[FilterWindowLen];

int16_t ch0VoltageBufII[FilterWindowLenII];
int16_t ch1VoltageBufII[FilterWindowLenII];

int16_t filterCounter = 0;
int16_t pdata;   
int16_t pmaxCh0,pminCh0,pmaxCh1,pminCh1; 

int16_t pdataII;   
int16_t pmaxCh0II,pminCh0II,pmaxCh1II,pminCh1II; 

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
	 
	return;
	
	pmaxCh0 =pminCh0=SDADC_ValueTable[0];
	pmaxCh1 =pminCh1=SDADC_ValueTable[1];
	for(i=1;i<ADCMeanWindow;i++){
		if(pmaxCh0<SDADC_ValueTable[2*i+0])
				pmaxCh0=SDADC_ValueTable[2*i+0];
		else if(pminCh0>SDADC_ValueTable[2*i+0])
				pminCh0=SDADC_ValueTable[2*i+0];
				
		if(pmaxCh1<SDADC_ValueTable[2*i+1])
				pmaxCh1=SDADC_ValueTable[2*i+1];
		else if(pminCh1>SDADC_ValueTable[2*i+1])
				pminCh1=SDADC_ValueTable[2*i+1];
	}
  
	pdata=(pdata+1)%FilterWindowLen; //pdata=[0<->FilterWindowLen-1]
	sum_voltage.ch0 = pminCh0+pmaxCh0;
	sum_voltage.ch1 = pminCh1+pmaxCh1;
	pminCh1 = pminCh0=0;
	pmaxCh0 = pmaxCh1=0;
	ch0VoltageBuf[pdata]=sum_voltage.ch0>>1;
	ch1VoltageBuf[pdata]=sum_voltage.ch1>>1;
	
  sum_voltage.ch0 = 0;
	sum_voltage.ch1 = 0;
  for(i=0;i<FilterWindowLen;i++){
     sum_voltage.ch0 +=ch0VoltageBuf[i];
		 sum_voltage.ch1 +=ch1VoltageBuf[i]; 
   }
	 if(ch0VoltageBuf[pdata]>ch0VoltageBuf[pmaxCh0])
      pmaxCh0=pdata;    
	 else if(ch0VoltageBuf[pdata]<ch0VoltageBuf[pminCh0])
      pminCh0=pdata;  
			
	 if(ch1VoltageBuf[pdata]>ch1VoltageBuf[pmaxCh1])
      pmaxCh1=pdata;    
	 else if(ch1VoltageBuf[pdata]<ch1VoltageBuf[pminCh1])
      pminCh1=pdata;  		

		if(pdata==pmaxCh0)   
		 {      
					for(i=0;i<FilterWindowLen;i++)
							if(ch0VoltageBuf[i]>ch0VoltageBuf[pmaxCh0])
							   pmaxCh0=i;
		 }
		 else if(pdata==pminCh0) 
		 {      
				 for(i=0;i<FilterWindowLen;i++)
							if(ch0VoltageBuf[i]<ch0VoltageBuf[pminCh0])
								  pminCh0=i;
		 }
		 
		 if(pdata==pmaxCh1)   
		 {      
					for(i=0;i<FilterWindowLen;i++)
							if(ch1VoltageBuf[i]>ch1VoltageBuf[pmaxCh1])
							   pmaxCh1=i;
		 }
		 else if(pdata==pminCh1) 
		 {      
				 for(i=0;i<FilterWindowLen;i++)
							if(ch1VoltageBuf[i]<ch1VoltageBuf[pminCh1])
								  pminCh1=i;
		 }
	sum_voltage.ch0 =sum_voltage.ch0 -ch0VoltageBuf[pminCh0]-ch0VoltageBuf[pmaxCh0];
	sum_voltage.ch1 =sum_voltage.ch1 -ch1VoltageBuf[pminCh1]-ch1VoltageBuf[pmaxCh1]; 
	
	temp =(sum_voltage.ch0>>FilterWindowSHIFT)& 0xFFFF;
	filter_voltage.ch0 = (float)(2.0f* (((temp  + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	temp =(sum_voltage.ch1>>FilterWindowSHIFT)& 0xFFFF;
	//filter_voltage.ch1 = (float)(2.0f* (((temp  + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	ch1VoltageBufII[filterCounter] =temp;// (float)(2.0f* (((temp + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));	
 /*
	temp =(sum_voltage.ch0>>FilterWindowSHIFT)& 0xFFFF;
	ch0VoltageBufII[filterCounter] =temp;//(float)(2.0f* (((temp  + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	temp =(sum_voltage.ch1>>FilterWindowSHIFT)& 0xFFFF;
	ch1VoltageBufII[filterCounter] =temp;// (float)(2.0f* (((temp + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));	
 /*
	filterCounter++;
	if(filterCounter>=FilterWindowLenII){
		filterCounter = 0;
		sum_voltage.ch0 = 0;
		sum_voltage.ch1 = 0;
		for(i = 0;i<FilterWindowLenII;i++){
			sum_voltage.ch0 +=ch0VoltageBufII[i];
			sum_voltage.ch1 +=ch1VoltageBufII[i];
		}
		temp =(sum_voltage.ch0>>FilterWindowSHIFTII)& 0xFFFF;
		filter_voltage.ch0 = (float)(2.0f* (((temp  + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
		temp =(sum_voltage.ch1>>FilterWindowSHIFTII)& 0xFFFF;
		filter_voltage.ch1 = (float)(2.0f* (((temp + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));	
	}*/
	//AD5761_SetVotage(65536.0f*(filter_voltage.ch0/5000.0f));
	
}
void VOL_IIR_FilterII()
{
	uint16_t i = 0;
	int16_t temp = 0;
	pdataII=(pdataII+1)%FilterWindowLen; //pdata=[0<->FilterWindowLen-1]
	ch0VoltageBufII[pdataII]=voltage.ch0;
	ch1VoltageBufII[pdataII]=voltage.ch0;
	
  sum_voltage.ch0 = 0;
	sum_voltage.ch1 = 0;
  for(i=0;i<FilterWindowLen;i++){
     sum_voltage.ch0 +=ch0VoltageBuf[i];
		 sum_voltage.ch1 +=ch1VoltageBuf[i]; 
   }
	 if(ch0VoltageBufII[pdataII]>ch0VoltageBufII[pmaxCh0II])
      pmaxCh0II=pdataII;    
	 else if(ch0VoltageBufII[pdataII]<ch0VoltageBufII[pminCh0II])
      pminCh0II=pdataII;  
			
	 if(ch1VoltageBufII[pdataII]>ch1VoltageBufII[pmaxCh1II])
      pmaxCh1II=pdataII;    
	 else if(ch1VoltageBufII[pdataII]<ch1VoltageBufII[pminCh1II])
      pminCh1II=pdataII;  		

		if(pdataII==pmaxCh0II)   
		 {      
					for(i=0;i<FilterWindowLen;i++)
							if(ch0VoltageBufII[i]>ch0VoltageBufII[pmaxCh0II])
							   pmaxCh0II=i;
		 }
		 else if(pdataII==pminCh0II) 
		 {      
				 for(i=0;i<FilterWindowLen;i++)
							if(ch0VoltageBufII[i]<ch0VoltageBufII[pminCh0II])
								  pminCh0II=i;
		 }
		 
		 if(pdataII==pmaxCh1II)   
		 {      
					for(i=0;i<FilterWindowLen;i++)
							if(ch1VoltageBufII[i]>ch1VoltageBufII[pmaxCh1II])
							   pmaxCh1II=i;
		 }
		 else if(pdataII==pminCh1II) 
		 {      
				 for(i=0;i<FilterWindowLen;i++)
							if(ch1VoltageBufII[i]<ch1VoltageBufII[pminCh1II])
								  pminCh1II=i;
		 }
	sum_voltage.ch0 =sum_voltage.ch0 -ch0VoltageBufII[pminCh0II]-ch0VoltageBufII[pmaxCh0II];
	sum_voltage.ch1 =sum_voltage.ch1 -ch1VoltageBufII[pminCh1II]-ch1VoltageBufII[pmaxCh1II]; 
	temp =(sum_voltage.ch0>>FilterWindowSHIFT)& 0xFFFF;
	filter_voltage.ch0 =(float)(2.0f* (((temp  + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));
	temp =(sum_voltage.ch1>>FilterWindowSHIFT)& 0xFFFF;
	filter_voltage.ch1 = (float)(2.0f* (((temp + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL)));	
	//AD5761_SetVotage(65536.0f*(filter_voltage.ch0/5000.0f));
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
