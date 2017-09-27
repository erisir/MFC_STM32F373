#ifndef __ADC_H
#define	__ADC_H


#include "stm32f37x.h"
 
uint32_t SDADC1_Config(void);
void ADC_Config(void);
void GetPosition(void);
float GetADCVoltage(unsigned char ch);
void SetVotageTimes(float val);
int16_t ADC_Filter(unsigned char ch) ;
float ADC_Mean(unsigned char ch);
#endif /* __ADC_H */

