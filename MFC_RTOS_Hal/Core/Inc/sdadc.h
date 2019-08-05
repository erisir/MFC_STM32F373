/**
  ******************************************************************************
  * File Name          : SDADC.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sdadc_H
#define __sdadc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "port.h"
#include "user_mb_app.h"
/* USER CODE END Includes */

extern SDADC_HandleTypeDef hsdadc1;

/* USER CODE BEGIN Private defines */

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

/* USER CODE END Private defines */

void MX_SDADC1_Init(void);

/* USER CODE BEGIN Prototypes */

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
 
void VOL_IIR_Filter(void);
float GetADCVoltage(unsigned char ch);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ sdadc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
