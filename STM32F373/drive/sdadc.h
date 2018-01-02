#ifndef __ADC_H
#define	__ADC_H
#include "../prosses/tasks.h"
 

//#define __ADC_INJECTED_MODE_
#define  __ADC_DMA_MODE_
//#define __ADC_ADC1_MODE_
#define POT_GPIO_PORT        GPIOE
   
      
#define POT_SDADC            SDADC1
 
#define POT_SDADC_GAIN       SDADC_Gain_1   /* Internal gain 1 is seleted: 
                                               SDADC_GAIN must be updated according to
                                               POT_SDADC_GAIN */
#define SDADC_GAIN           (uint32_t) 1  /* SDADC internal gain is set to 1: update this define
                                              according to POT_SDADC_GAIN */
#define SDADC_RESOL          (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT   30 /* ~ about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT    4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF           (float) 3000  /* SDADC external reference is set to 3V */


struct _Voltage
{
float ch0;
float ch1;
};
extern struct _Voltage voltage;
extern struct _Voltage filter_voltage;

void ADC1_Init(void);
uint8_t SDADC1_Config(void);
uint8_t ADC1_Config(void);

void Calculate_FilteringCoefficient(float Cut_Off);
void VOL_IIR_Filter(void);
 
float GetADCVoltage(unsigned char ch);
void updateVoltageWindowBuf(void);


void updateRawData(void);
void ADC_Mean(void);
 
#endif /* __ADC_H */

