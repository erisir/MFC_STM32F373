/**
  ******************************************************************************
  * @file    rs485.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   rs485
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 	    
#include "rs485.h"	
 

//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
u8 RS485_TX_BUF[64];	  //���ڷ��ͻ�����
//���յ������ݳ���
u8 RS485_RX_CNT=0;   		  

void USART1_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
	{	 
	 			 
		res =USART_ReceiveData(USART1); 	//��ȡ���յ�������
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
		} 
	}  											 
} 
 									 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 bound)
{  
	USART_InitTypeDef USART_InitStructure;
  
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);

  RS485_Mode_Rx();			//Ĭ��Ϊ����ģʽ
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_Mode_Tx();			//����Ϊ����ģʽ
  for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_Mode_Rx();				//����Ϊ����ģʽ	
}
void RS485_PrintString1(u8 *buf)
{
	RS485_Mode_Tx();			//����Ϊ����ģʽ
  while(*buf)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,*buf);
		buf++;
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_Mode_Rx();				//����Ϊ����ģʽ	
}
 
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_us(200);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}
void RS485_Mode_Rx(void)  
{ 
	//GPIO_ResetBits(GPIOD, GPIO_Pin_8 );
}
void RS485_Mode_Tx(void) 
{
	//GPIO_SetBits(GPIOD, GPIO_Pin_8 );
}


 




