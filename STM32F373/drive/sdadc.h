#ifndef __ADC_H
#define	__ADC_H
#include "stm32f37x.h"
/* A potentiometer (POT) is connected to PB1 which corresponds to SDADC1 channel 5P */
#define POT_GPIO_PORT        GPIOB
#define POT_GPIO_PIN         GPIO_Pin_1|GPIO_Pin_0
#define POT_GPIO_CLK         RCC_AHBPeriph_GPIOB
#define POT_SDADC            SDADC1
#define POT_SDADC_CLK        RCC_APB2Periph_SDADC1
#define POT_SDADC_PWR        PWR_SDADCAnalog_1
#define POT_SDADC_VREF       SDADC_VREF_Ext /* External reference is selected */
#define POT_SDADC_GAIN       SDADC_Gain_1   /* Internal gain 1 is seleted: 
                                               SDADC_GAIN must be updated according to
                                               POT_SDADC_GAIN */
#define SDADC_GAIN           (uint32_t) 1  /* SDADC internal gain is set to 1: update this define
                                              according to POT_SDADC_GAIN */
#define SDADC_RESOL          (uint32_t) 65535 /* 2e16 - 1 */
#define SDADC_INIT_TIMEOUT   30 /* ~ about two SDADC clock cycles after INIT is set */
#define SDADC_CAL_TIMEOUT    4*30720 /*  ~5.12 ms at 6 MHz  in a single calibration sequence */
#define SDADC_VREF           (float) 3000  /* SDADC external reference is set to 3.3V */


struct _Voltage
{
float ch0;
float ch1;
};
extern struct _Voltage voltage;
extern struct _Voltage filter_voltage;

uint32_t SDADC1_Config(void);
void Calculate_FilteringCoefficient(float Time, float Cut_Off);
void VOL_IIR_Filter(void);
float GetADCVoltage(unsigned char ch);
int ADC_Mean(unsigned char ch);
#endif /* __ADC_H */

