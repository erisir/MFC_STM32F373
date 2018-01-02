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
  SPI_Cmd(SPI3, DISABLE); //ֹͣSPI����
	
	
 		

  
	//AD5761 rising edge ���� latched ��falling edge  transferred in
	// SDO Data is clocked out on the rising edge of SCLK and is valid on the falling edge of SCLK.
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����	 	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
  SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����  
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	
}   
 
  
void SPI3_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI3->CR1&=0XFFC7;
	SPI3->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI3,ENABLE); 
} 
 
uint8_t SPI3_RW(uint8_t data) 
{ 
	uint8_t retry=0;
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)//��SPI���ͻ������ǿ�ʱ�ȴ�  
	{
		retry++;
		if(retry>100)return retry;
	}
	SPI_SendData8(SPI3, data);//ͨ��SPI���߷���һ�ֽ�����
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)//��SPI���ջ�����Ϊ��ʱ�ȴ�
	{
		retry++;
		if(retry>110)return retry;
	}
	return SPI_ReceiveData8(SPI3);
}
 
/******************************************************************************
����ԭ�ͣ�	uint8_t SPI3_Write_Reg(uint8_t reg, uint8_t value)
��    �ܣ�	SPI3д�Ĵ���
�� �� ֵ��	SPI3д�Ĵ�������ֵ
*******************************************************************************/
uint8_t SPI3_Write_Reg(uint8_t reg, uint16_t value)
{
	uint8_t status,data;
	SPI3_CSN_L;		//ѡͨSPI3����
	delay_ms(1);
	status = SPI3_RW(reg);//д�Ĵ�����ַ	
	
	data=(value & 0xFF00) >> 8;
	
	SPI3_RW( data);	//д����
	
	data = (value & 0x00FF) >> 0;
	
	SPI3_RW(data);	//д����
	delay_ms(1);
	SPI3_CSN_H;		//��ֹSPI3����
	return 	status;
}
/******************************************************************************
����ԭ�ͣ�	uint8_t SPI3_Read_Buff(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
��    �ܣ�	SPI3��������
�� �� ֵ��	����������
*******************************************************************************/
uint8_t SPI3_WriteRead_Buff( uint8_t *pBuf, uint8_t uchars)
{
	uint8_t i;
	SPI3_CSN_L;	//ѡͨSPI3���� 
	delay_ms(1);
	for(i=0; i<uchars; i++)
	{
		pBuf[i] = SPI3_RW(pBuf[i]);//��ȡ��������	
	}
	delay_ms(1);
	SPI3_CSN_H;	//��ֹSPI3����
  return 	0;
}
 


























