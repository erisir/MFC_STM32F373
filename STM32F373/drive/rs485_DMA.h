#ifndef __RS485_DMA_H
#define __RS485_DMA_H			 
#include "sys.h"	 			
#include "stm32f37x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//RS485驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	
extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

typedef struct 
{
	uint8_t  FIRST_BYTE; 
	uint16_t START_CODE;
	uint8_t  BodyLen;
	uint8_t  CMD_TYPE;  	
}HOST_MSG_HEADER_T;//10 bytes

//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.
#define HOST_MSG_START_CODE_FIRST_BYTE '$'
#define HOST_MSG_START_CODE_LEN  4
 
#define HOST_MSG_START_CODE 0x4E3c//uint16_t$N<[len]
#define CRC_LEN 0
static void USART1_Configuration(uint32_t BaudRate);
void PutMsg2RecvQueue(uint8_t * pMsgBuffer);
uint8_t *GetRecvBuf(void);
void rs485_DMA_Init(uint32_t BaudRate);
void USART1_Start_DMA_Recv(void * recvBuf, uint32_t bufLen);
void USART1_Start_DMA_Send(void * sendBuf, uint32_t bufLen);
void RS485_PrintString(uint8_t  * sendBuf);
 
#endif	   

















