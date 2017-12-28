#include "rs485_DMA.h"
#include <sys.h>
#include <string.h>
uint8_t DMAReciveState=0;
uint8_t USART1_DMA_SendBuf[64];
uint8_t USART1_DMA_HeadBuf[64];

uint8_t USART1_MSG_RecvQueue[128];
uint8_t * pMsgBuffer;
HOST_MSG_HEADER_T *pMCUMsgHeader;

uint8_t USART1_MSG_RecvCounter=0;
uint8_t gDMA2Channel5Running = 0;
uint8_t gDMA1Channel4Running = 0;

void rs485_DMA_Init(uint32_t BaudRate)
{
	USART1_Configuration(BaudRate);
	USART1_Start_DMA_Recv((void *)USART1_DMA_HeadBuf,1);
}
uint8_t *GetRecvBuf(void)
{
	
	HOST_MSG_HEADER_T * pBuf = (HOST_MSG_HEADER_T*)USART1_MSG_RecvQueue;
	
	return (uint8_t *)(pBuf+(USART1_MSG_RecvCounter-1));
}
void PutMsg2RecvQueue(uint8_t * pMsgBuffer)
{
	USART1_MSG_RecvCounter++;
}
static void USART1_Configuration(uint32_t BaudRate)
{
		 USART_InitTypeDef USART_InitStructure;
		
		 USART_InitStructure.USART_BaudRate =BaudRate;
		 USART_InitStructure.USART_WordLength =USART_WordLength_8b; 
		 USART_InitStructure.USART_StopBits =USART_StopBits_1; 
		 USART_InitStructure.USART_Parity = USART_Parity_No; 
		 USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; 
		 USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx; 

		 USART_Init(USART1,&USART_InitStructure);
		 USART_Cmd(USART1, ENABLE);  
}
void USART1_Start_DMA_Recv(void * recvBuf, uint32_t bufLen)
{
    DMA_InitTypeDef DMA_InitStructure;
      
    /* DMA1 Channel5 (triggered by USART1 Rxevent) Config */
    DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&(USART1->RDR);//
    DMA_InitStructure.DMA_MemoryBaseAddr =(u32)recvBuf;
    DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize =bufLen;
    DMA_InitStructure.DMA_PeripheralInc =DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode =DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority =DMA_Priority_High;
    DMA_InitStructure.DMA_M2M =DMA_M2M_Disable;
      
    DMA_DeInit(DMA1_Channel5);
    DMA_Init(DMA1_Channel5,&DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC,ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx,ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}
void RS485_PrintString(uint8_t * sendBuf)
{
	  USART1_Start_DMA_Send((void *)sendBuf,sizeof(sendBuf));
}
void USART1_Start_DMA_Send(void * sendBuf, uint32_t bufLen)
{
     DMA_InitTypeDef DMA_InitStructure;
 
    if (bufLen == 0)
        return ;
    memcpy(USART1_DMA_SendBuf, sendBuf, bufLen);
             
    DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&USART1->TDR);
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)USART1_DMA_SendBuf; 
    DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize =bufLen; 
    DMA_InitStructure.DMA_PeripheralInc =DMA_PeripheralInc_Disable; 
    DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable; 
    DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize =DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_Mode =DMA_Mode_Normal; 
    DMA_InitStructure.DMA_Priority =DMA_Priority_High; 
    DMA_InitStructure.DMA_M2M =DMA_M2M_Disable;
             
    DMA_DeInit(DMA1_Channel4); 
    DMA_Init(DMA1_Channel4,&DMA_InitStructure);          
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    gDMA1Channel4Running = 1;
 
}


void DMA1_Channel5_IRQHandler(void)// recv 
{       
    if(DMA_GetITStatus(DMA1_IT_TC5))
    {
        DMA_ClearITPendingBit(DMA1_IT_GL5);  
        DMA_Cmd(DMA1_Channel5, DISABLE);
 
        if(0 == DMAReciveState)
        {
            if(HOST_MSG_START_CODE_FIRST_BYTE== USART1_DMA_HeadBuf[0])
            {
                DMAReciveState = 1;
                USART1_Start_DMA_Recv((void*)(USART1_DMA_HeadBuf + 1), 3);
            }
            else
            {
               USART1_Start_DMA_Recv((void*)USART1_DMA_HeadBuf, 1);
            }
        }
        else if(1 == DMAReciveState)
        {
            if(HOST_MSG_START_CODE ==*(uint32_t *)USART1_DMA_HeadBuf)
            {
                 DMAReciveState = 2;
                 USART1_Start_DMA_Recv((void*)(USART1_DMA_HeadBuf + 4), 8);
            }
            else
            {
                DMAReciveState = 0;
                memset(USART1_DMA_HeadBuf, 0, sizeof(USART1_DMA_HeadBuf));
                USART1_Start_DMA_Recv((void*)USART1_DMA_HeadBuf, 1);
            }
        }
        else if(2 == DMAReciveState)
        {
           
            DMAReciveState = 3;
            pMsgBuffer = (uint8_t*)GetRecvBuf();
            if(pMsgBuffer == NULL)
            {
                DMAReciveState = 0;
                memset(USART1_DMA_HeadBuf, 0,sizeof(USART1_DMA_HeadBuf));
                USART1_Start_DMA_Recv((void*)USART1_DMA_HeadBuf, 1);               
                return ;
            }
            memcpy(pMsgBuffer,USART1_DMA_HeadBuf, sizeof(HOST_MSG_HEADER_T));
            pMCUMsgHeader = (HOST_MSG_HEADER_T*)pMsgBuffer;
            USART1_Start_DMA_Recv((void*)(pMsgBuffer +sizeof(HOST_MSG_HEADER_T)), pMCUMsgHeader->BodyLen + CRC_LEN);
        }
        else if(3 == DMAReciveState)
        {
            DMAReciveState = 0;
            memset(USART1_DMA_HeadBuf, 0,sizeof(USART1_DMA_HeadBuf));
            USART1_Start_DMA_Recv((void*)USART1_DMA_HeadBuf, 1);
            if (pMsgBuffer != NULL)
            {
                PutMsg2RecvQueue(pMsgBuffer);
                pMsgBuffer = NULL;
            }
        }
    }
}
void DMA1_Channel4_IRQHandler(void)// send complete
{
   if(DMA_GetITStatus(DMA1_IT_TC4)==SET)
    {
         DMA_ClearFlag(DMA1_IT_GL4);
         DMA_Cmd(DMA1_Channel4,DISABLE);
         gDMA1Channel4Running = 0;
    }
}
