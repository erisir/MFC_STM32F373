/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi3;

/* USER CODE BEGIN Private defines */
/* Input Shift Register Commands */
#define CMD_NOP										0x00
#define CMD_WR_TO_INPUT_REG				0x01
#define CMD_UPDATE_DAC_REG				0x02
#define CMD_WR_UPDATE_DAC_REG			0x03//0011 Write and update DAC register
#define CMD_WR_CTRL_REG						0x04//0100 Write to control register
#define CMD_NOP_ALT_1							0x05
#define CMD_NOP_ALT_2							0x06
#define CMD_SW_DATA_RESET					0x07
#define CMD_RESERVED							0x08
#define CMD_DIS_DAISY_CHAIN				0x09
#define CMD_RD_INPUT_REG					0x0A
#define CMD_RD_DAC_REG						0x0B
#define CMD_RD_CTRL_REG						0x0C
#define CMD_NOP_ALT_3							0x0D
#define CMD_NOP_ALT_4							0x0E
#define CMD_SW_FULL_RESET					0x0F

 
#define SPI3_CSN_H HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
#define SPI3_CSN_L HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)

/* USER CODE END Private defines */

void MX_SPI3_Init(void);

/* USER CODE BEGIN Prototypes */

void AD5761_Config(void);
 
void AD5761_SetVoltage(uint16_t reg_data);

 
uint8_t SPI3_ReadWriteByte(uint8_t TxData);
uint8_t SPI3_Write_Reg(uint8_t CMD_TYPE, uint16_t value);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
