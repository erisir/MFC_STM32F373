#ifndef __RS485_DMA_H
#define __RS485_DMA_H			 
#include "sys.h"	 			
#include "stm32f37x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//RS485���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	
extern u8 RS485_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���

typedef struct 
{
	uint8_t  FIRST_BYTE; 
	uint16_t START_CODE;
	uint8_t  BodyLen;
	uint8_t  CMD_TYPE;  	
}HOST_MSG_HEADER_T;//10 bytes

//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART2_RX 	1			//0,������;1,����.
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

















