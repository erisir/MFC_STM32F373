/**
  ******************************************************************************
  * @file    spi.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   spi
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 	
#include "spi.h"
void AD5761_Init(void){ 
	SPI3_Init();
	AD5761_Config();
	AD5761_SetVotage(0);
	//AD5761_ReadConfig(); 
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
	 
	SPI3_Write_Reg(CMD_WR_CTRL_REG,value);
	//SPI3_Write_Reg(CMD_SW_FULL_RESET,0);
}
 
void AD5761_SetVotage(uint16_t DAC_output){
	SPI3_Write_Reg(CMD_WR_UPDATE_DAC_REG,DAC_output);
}

void AD5761_ReadConfig(void){
	  // update DAC
	uint8_t buf[64];
	SPI3_Write_Reg(CMD_RD_CTRL_REG,0);
  buf[0]=CMD_NOP;
	SPI3_WriteRead_Buff(buf,3);
	sprintf((char*)buf,"SPI_RecieveData[%d %d %d ]\n",buf[0],buf[1],buf[2]);
	RS485_PrintString(buf);
}
 

void SPI3_Init()
{
 
  SPI_InitTypeDef  SPI_InitStructure;
  SPI_Cmd(SPI3, DISABLE); //停止SPI外设
	
	
 		

  
	//AD5761 rising edge 锁存 latched ；falling edge  transferred in
	// SDO Data is clocked out on the rising edge of SCLK and is valid on the falling edge of SCLK.
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样	 	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
  SPI_Cmd(SPI3, ENABLE); //使能SPI外设  
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	
}   
 
  
void SPI3_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI3->CR1&=0XFFC7;
	SPI3->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI3,ENABLE); 
} 
 
uint8_t SPI3_RW(uint8_t data) 
{ 
	uint8_t retry=0;
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)//当SPI发送缓冲器非空时等待  
	{
		retry++;
		if(retry>100)return retry;
	}
	SPI_SendData8(SPI3, data);//通过SPI总线发送一字节数据
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)//当SPI接收缓冲器为空时等待
	{
		retry++;
		if(retry>110)return retry;
	}
	return SPI_ReceiveData8(SPI3);
}
 
/******************************************************************************
函数原型：	uint8_t SPI3_Write_Reg(uint8_t reg, uint8_t value)
功    能：	SPI3写寄存器
返 回 值：	SPI3写寄存器返回值
*******************************************************************************/
uint8_t SPI3_Write_Reg(uint8_t reg, uint16_t value)
{
	uint8_t status,data;
	SPI3_CSN_L;		//选通SPI3器件
	delay_ms(1);
	status = SPI3_RW(reg);//写寄存器地址	
	
	data=(value & 0xFF00) >> 8;
	
	SPI3_RW( data);	//写数据
	
	data = (value & 0x00FF) >> 0;
	
	SPI3_RW(data);	//写数据
	delay_ms(1);
	SPI3_CSN_H;		//禁止SPI3器件
	return 	status;
}
/******************************************************************************
函数原型：	uint8_t SPI3_Read_Buff(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
功    能：	SPI3读缓冲区
返 回 值：	缓冲区数据
*******************************************************************************/
uint8_t SPI3_WriteRead_Buff( uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	SPI3_CSN_L;	//选通SPI3器件 
	delay_ms(1);
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = SPI3_RW(pBuf[i]);//读取返回数据	
	}
	delay_ms(1);
	SPI3_CSN_H;	//禁止SPI3器件
  return 	0;
}
 


























