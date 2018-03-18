#ifndef __ADC_H
#define	__ADC_H
#include "../prosses/tasks.h"
 
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
#define SDADC_VREF           (float) 2862  /* SDADC external reference is set to 3V */


struct _Voltage
{
float ch0;
float ch1;
};
struct _VoltageRaw
{
int16_t ch0;
int16_t ch1;
};
struct _VoltageSum
{
int32_t ch0;
int32_t ch1;
};

extern struct _VoltageRaw voltage;
extern struct _Voltage filter_voltage;
extern struct _VoltageSum sum_voltage;

void ADC1_Init(void);
uint8_t SDADC1_Config(void);
 
void VOL_IIR_Filter(void);
void VOL_IIR_FilterII(void);
float GetADCVoltage(unsigned char ch);
 
#endif /* __ADC_H */

