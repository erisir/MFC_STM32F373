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
 

//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
u8 RS485_TX_BUF[64];	  //串口发送缓冲区
//接收到的数据长度
u8 RS485_RX_CNT=0;   		  

void USART1_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{	 
	 			 
		res =USART_ReceiveData(USART1); 	//读取接收到的数据
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
		} 
	}  											 
} 
 									 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
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

  RS485_Mode_Rx();			//默认为接收模式
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_Mode_Tx();			//设置为发送模式
  for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_Mode_Rx();				//设置为接收模式	
}
void RS485_PrintString1(u8 *buf)
{
	RS485_Mode_Tx();			//设置为发送模式
  while(*buf)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,*buf);
		buf++;
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_Mode_Rx();				//设置为接收模式	
}
 
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_us(200);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
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


 




