#ifndef __SPI_H
#define __SPI_H
#include "../prosses/tasks.h"

#define SPI3_CSN_H GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define SPI3_CSN_L GPIO_ResetBits(GPIOA,GPIO_Pin_4)
/* Input Shift Register Format */
 
/* Input Shift Register Commands */
#define CMD_NOP										0x0
#define CMD_WR_TO_INPUT_REG				0x1
#define CMD_UPDATE_DAC_REG				0x2
#define CMD_WR_UPDATE_DAC_REG			0x03//0011 Write and update DAC register
#define CMD_WR_CTRL_REG						0x04//0100 Write to control register
#define CMD_NOP_ALT_1							0x5
#define CMD_NOP_ALT_2							0x6
#define CMD_SW_DATA_RESET					0x7
#define CMD_RESERVED							0x8
#define CMD_DIS_DAISY_CHAIN				0x9
#define CMD_RD_INPUT_REG					0xA
#define CMD_RD_DAC_REG						0xB
#define CMD_RD_CTRL_REG						0xC
#define CMD_NOP_ALT_3							0xD
#define CMD_NOP_ALT_4							0xE
#define CMD_SW_FULL_RESET					0xF

 
void SPI3_Init(void);  
void AD5761_Init(void); 			
void AD5761_Config(void);
void AD5761_ReadConfig(void);
void AD5761_SetVotage(uint16_t reg_data);

void SPI3_SetSpeed(uint8_t SpeedSet); 

uint8_t SPI3_RW(uint8_t data);

uint8_t SPI3_Write_Reg(uint8_t reg, uint16_t value);
uint8_t SPI3_WriteRead_Buff(uint8_t *pBuf, uint8_t uchars);

		 
#endif

