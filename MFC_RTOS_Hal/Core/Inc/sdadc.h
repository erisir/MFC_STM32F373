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

/* USER CODE END Includes */

extern SDADC_HandleTypeDef hsdadc1;

/* USER CODE BEGIN Private defines */


#define SDADC_GAIN           (float) 0.51564  /* SDADC internal gain is set to 1: update this define according to POT_SDADC_GAIN */

#define SDADC_RESOL          (uint32_t) 65535 /* 2e16 - 1 */

#define SDADC_VREF           (float) 2990  /* SDADC external reference is set to 3V */

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
void SDADC_Config(void);
void Calculate_FilteringCoefficient(float Cut_Off);
void setValtageOffset(void);
void FlowAccumulator(void);
uint16_t GetADCVoltage(uint8_t ch);
void ResetFlowAccumulator(void);
void ResetFlowOffset(void);
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
