/**
  ******************************************************************************
  * File Name          : SPI.c
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

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */
 
/* USER CODE END 0 */

SPI_HandleTypeDef hspi3;

/* SPI3 init function */
void MX_SPI3_Init(void)
{

  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI3)
  {
  /* USER CODE BEGIN SPI3_MspInit 0 */
  
  /* USER CODE END SPI3_MspInit 0 */
    /* SPI3 clock enable */
    __HAL_RCC_SPI3_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI3 GPIO Configuration    
    PA1     ------> SPI3_SCK
    PA2     ------> SPI3_MISO
    PA3     ------> SPI3_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI3_MspInit 1 */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4,GPIO_PIN_SET);
  /* USER CODE END SPI3_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI3)
  {
  /* USER CODE BEGIN SPI3_MspDeInit 0 */

  /* USER CODE END SPI3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();
  
    /**SPI3 GPIO Configuration    
    PA1     ------> SPI3_SCK
    PA2     ------> SPI3_MISO
    PA3     ------> SPI3_MOSI 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

  /* USER CODE BEGIN SPI3_MspDeInit 1 */

  /* USER CODE END SPI3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

 //SPI5 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI3_ReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata,retry=0;
    //HAL_SPI_TransmitReceive(&hspi3,&TxData,&Rxdata,1, 1000);

		while (__HAL_SPI_GET_FLAG(&hspi3, SPI_FLAG_TXE) == RESET)//当SPI发送缓冲器非空时等待  很重要！！！
		{
			retry++;
			if(retry>100)return retry;
		}  
		HAL_SPI_Transmit(&hspi3,&TxData,1, 1000);   
		retry=0;
		while (__HAL_SPI_GET_FLAG(&hspi3, SPI_FLAG_RXNE) == RESET)//当SPI发送缓冲器非空时等待  
		{
			retry++;
			if(retry>100)return retry;
		} 
		HAL_SPI_Receive(&hspi3,&Rxdata,1, 1000);
		return Rxdata;          		    //返回收到的数据		
}
void AD5761_Config(void){
 // initialize DAC (AD5761)
	
  uint16_t value;
  uint16_t CV  = 0; // CLEAR voltage= 0,zero 1 midscale 2 full
  uint16_t OVR = 0; // 5% overrange= 0 disabled,1 enable
  uint16_t B2C = 0; // Bipolar range= 0 straight coded(unsiged) 1 twos complement coded (signed)
  uint16_t ETS = 1; // Thermal shutdown =  0 disabled,1 enable
  uint16_t IRO = 1; // Internal reference = 0 off 1 on
  uint16_t PV  = 0; // Power up voltage = 0,zero 1 midscale 2 full
  uint16_t RA  = 3; // Output range =0 -10 to 10
									  /*               1 0 to 10
											               2 -5 to 5
																		 3 0  to 5 
																		 4 -2.5 to 7.5
																		 5 -3 to 3
																		 6 0 to 16
																		 7 0 to 20		*/
  value =  (CV<<9) | (OVR<<8) | (B2C<<7) | (ETS<<6) | (IRO<<5) | (PV<<3) | (RA);  
	SPI3_Write_Reg(CMD_NOP,0);
	SPI3_Write_Reg(CMD_WR_CTRL_REG,value);
	SPI3_Write_Reg(CMD_NOP,0);
	//SPI3_Write_Reg(CMD_SW_FULL_RESET,0);
}
/******************************************************************************
函数原型：	uint8_t SPI3_Write_Reg(uint8_t reg, uint8_t value)
功    能：	SPI3写寄存器
返 回 值：	SPI3写寄存器返回值
*******************************************************************************/
uint8_t SPI3_Write_Reg(uint8_t CMD_TYPE, uint16_t value)
{
	uint8_t Rxdata,data;

	SPI3_CSN_L;		//选通SPI3器件
  HAL_Delay(1); 
	Rxdata = SPI3_ReadWriteByte(CMD_TYPE);//写寄存器地址	
	data=(value & 0xFF00) >> 8;
	
	SPI3_ReadWriteByte( data);	//写数据
	data = (value & 0x00FF) >> 0;

	SPI3_ReadWriteByte(data);	//写数据

  HAL_Delay(1); 
	SPI3_CSN_H;		//禁止SPI3器件
	return 	Rxdata;
}
void AD5761_SetVotage(uint16_t DAC_output){
	SPI3_Write_Reg(CMD_WR_UPDATE_DAC_REG,DAC_output);	
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
