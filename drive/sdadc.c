#include "sdadc.h"
#include "delay.h"
#include <stdio.h>

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
// SDADCת���ĵ�ѹֵͨ��inject��ʽ����SRAM
int16_t InjectedConvData[2]={0};

uint16_t ADC_ConvertedSumWindow=10000;


void SetADCMeanWindow(float val){
	ADC_ConvertedSumWindow = val;
}

float  GetADCVoltage(unsigned char ch){//PID����
	float votage = 0.0; 
	votage =2* (((ADC_Mean(ch) + 32768) * SDADC_VREF) / (SDADC_GAIN * SDADC_RESOL));	
	return votage; // ��ȡת����ADֵ	 
}

float ADC_Mean(unsigned char ch) {
	int i = 0;
	int32_t sum=0;
	for(i=0;i<ADC_ConvertedSumWindow;i++){
		sum+=InjectedConvData[ch];	
	}
	return ((float)sum/(ADC_ConvertedSumWindow)); 
}
/*****************************************
---------------ADC��ֵ�˲�----------------
����N�㣬����ȥ�������Сֵ��ȡƽ��
*****************************************/
#define N 180 
int16_t ADC_Filter(unsigned char ch) 
{ 
	unsigned int count,i,j; 
	int16_t value_buf[N],temp; 
	unsigned int trimEnd = ADC_ConvertedSumWindow;
	int32_t  sum=0; 
	for  (count=0;count<N;count++) 
	{ 
		value_buf[count] = InjectedConvData[ch];	   //ȥ�����λ
	} 
	for (j=0;j<N-1;j++) //ð������
	{ 
		for (i=0;i<N-j;i++) 
		{ 
			if ( value_buf[i]>value_buf[i+1] ) 
			{ 
				temp = value_buf[i]; 
				value_buf[i] = value_buf[i+1];  
				value_buf[i+1] = temp; 
			} 
		} 
	} 
	for(count=trimEnd;count<N-trimEnd;count++) 
		sum += value_buf[count]; 
	return (int16_t)(sum/(N-trimEnd*2));  
}
 
/* Private function prototypes -----------------------------------------------*/

uint32_t SDADC1_Config(void)
{
	
  SDADC_AINStructTypeDef SDADC_AINStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  uint32_t SDADCTimeout = 0;
  /* POT_SDADC APB2 interface clock enable */
  RCC_APB2PeriphClockCmd(POT_SDADC_CLK, ENABLE);
  
  /* PWR APB1 interface clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Enable POT_SDADC analog interface */
  PWR_SDADCAnalogCmd(POT_SDADC_PWR, ENABLE);

  /* Set the SDADC divider: The SDADC should run @6MHz */
  /* If Sysclk is 72MHz, SDADC divider should be 12 */
  RCC_SDADCCLKConfig(RCC_SDADCCLK_SYSCLK_Div12);

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
 
  /* select POT_SDADC channel 5 to use conf0 only one channel each time*/
	SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_6, SDADC_Conf_0);
  SDADC_ChannelConfig(POT_SDADC, SDADC_Channel_5, SDADC_Conf_0);
	


  /* select channel(*) 5 */
	SDADC_InjectedChannelSelect(POT_SDADC, SDADC_Channel_5|SDADC_Channel_6);


  /* Enable continuous mode */
  SDADC_InjectedContinuousModeCmd(POT_SDADC, ENABLE);

  /* Exit initialization mode */
  SDADC_InitModeCmd(POT_SDADC, DISABLE);

  /* NVIC Configuration */
  NVIC_InitStructure.NVIC_IRQChannel = SDADC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

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
  SDADC_ITConfig(POT_SDADC, SDADC_IT_JEOC, ENABLE);
  /* Start a software start conversion */
  SDADC_SoftwareStartInjectedConv(POT_SDADC);
  
  return 0;
}

/**
  * @brief  This function handles SDADC1 interrupt request.
  * @param  None
  * @retval : None
  */
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
