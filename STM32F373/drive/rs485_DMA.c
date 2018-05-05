/**
  ******************************************************************************
  * @file    rs485_DMA.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   rs485_DMA
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 	
#include "rs485_DMA.h"
 
uint8_t DMAReciveState=0;

uint8_t USART1_DMA_TxBuf[64];
uint8_t USART1_DMA_RxBuf[64];

uint8_t USART1_DMA_MsgQueue[128];
uint8_t * pMsgBuffer;
 

uint8_t USART1_MSG_Counter=0;

uint8_t gDMA1Channel5Running = 0;
uint8_t gDMA1Channel4Running = 0;
uint8_t commERROR=0;
uint8_t busy= 0;
uint8_t expDataLen = 0;
void rs485_DMA_Init(uint32_t BaudRate)
{
	USART1_Configuration(BaudRate);
	USART1_Start_DMA_Recv((void *)USART1_DMA_RxBuf,1); 
}
 
uint8_t *GetRecvBuf(void)
{
	pMsgBuffer = USART1_DMA_MsgQueue;	
	return (uint8_t *)(pMsgBuffer);
}
void PutMsg2RecvQueue(uint8_t * pBuf)
{
	if(USART1_MSG_Counter==0){
		memcpy(GetRecvBuf(),pBuf, HOST_MSG_START_CODE_LEN+pBuf[HOST_MSG_START_CODE_LEN-1]);		
		USART1_MSG_Counter=1;	
	}
}
void ParseMsgRecvQueue(void)
{
	if(USART1_MSG_Counter==1){
		ParseData(GetRecvBuf());
		USART1_MSG_Counter=0;
	}
}
static void USART1_Configuration(uint32_t BaudRate)
{
		 USART_InitTypeDef USART_InitStructure;
		 USART_Cmd(USART1, DISABLE); 
		 USART_InitStructure.USART_BaudRate =BaudRate;
		 USART_InitStructure.USART_WordLength =USART_WordLength_8b; 
		 USART_InitStructure.USART_StopBits =USART_StopBits_1; 
		 USART_InitStructure.USART_Parity = USART_Parity_No; 
		 USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; 
		 USART_InitStructure.USART_Mode =USART_Mode_Rx | USART_Mode_Tx; 
		 USART_DeInit(USART1);  
		 USART_Init(USART1,&USART_InitStructure);
		 USART_Cmd(USART1, ENABLE);  
}
void USART1_Start_DMA_Recv(void * recvBuf, uint32_t bufLen)
{
    DMA_InitTypeDef DMA_InitStructure; 
		gDMA1Channel5Running = 1;

		if(bufLen>64)
			bufLen = 64;
		if(bufLen<1)
			bufLen = 1;
 
		DMA_Cmd(DMA1_Channel5, DISABLE);
	  DMA_ClearFlag(DMA1_FLAG_TC5); 
	  DMA_ClearFlag(DMA1_FLAG_TE5);
		DMA_ITConfig(DMA1_Channel5, DMA_IT_TC,DISABLE);    
		DMA_ITConfig(DMA1_Channel5, DMA_IT_TE,DISABLE);		
    USART_DMACmd(USART1,USART_DMAReq_Rx,DISABLE);
	
    DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&USART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)recvBuf;
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
		DMA_ITConfig(DMA1_Channel5, DMA_IT_TE,ENABLE);
    DMA_ClearFlag(DMA1_FLAG_TE5);  
		DMA_ClearFlag(DMA1_FLAG_TC5); 
		
		USART_DMACmd(USART1, USART_DMAReq_Rx,ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
}
 
void RS485_PrintString(uint8_t * sendBuf)
{		
	  //USART1_Start_DMA_Send((void *)sendBuf,strlen((char *)sendBuf));		
}
void USART1_Start_DMA_Send(void * sendBuf, uint32_t bufLen)
{
    DMA_InitTypeDef DMA_InitStructure;
 
    if (bufLen <1)
        return ;
		if (bufLen >64)
        bufLen = 64;
		while(gDMA1Channel4Running ==1);//wait for last sending completed

		gDMA1Channel4Running = 1;
		DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC,DISABLE);
		DMA_ITConfig(DMA1_Channel4, DMA_IT_TE,DISABLE);
	  DMA_ClearFlag(DMA1_FLAG_TC4); 
	  DMA_ClearFlag(DMA1_FLAG_TE4); 
		
    USART_DMACmd(USART1,USART_DMAReq_Tx,DISABLE);
	
    memcpy((char *)USART1_DMA_TxBuf, (char *)sendBuf, bufLen);
    DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&USART1->TDR);
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)(USART1_DMA_TxBuf); 
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
		DMA_ITConfig(DMA1_Channel4, DMA_IT_TE,ENABLE);
		DMA_ClearFlag(DMA1_FLAG_TC4); 
		DMA_ClearFlag(DMA1_FLAG_TE4);  		
	 
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
    DMA_Cmd(DMA1_Channel4, ENABLE);
    
}


void DMA1_Channel5_IRQHandler(void)// recv 
{       
	 if(DMA_GetITStatus(DMA1_IT_TE5) !=RESET)
    {
        gDMA1Channel5Running = 0;
				DMA_ClearITPendingBit(DMA1_IT_TE5); 
				DMAReciveState = 0;
				USART1_Start_DMA_Recv((void*)USART1_DMA_RxBuf, 1);
		}
    if(DMA_GetITStatus(DMA1_IT_TC5) !=RESET)
    {
        gDMA1Channel5Running = 0;
        DMA_Cmd(DMA1_Channel5, DISABLE);				 
				USART_DMACmd(USART1, USART_DMAReq_Rx,DISABLE);
				DMA_ITConfig(DMA1_Channel5, DMA_IT_TC,DISABLE);
				DMA_ClearITPendingBit(DMA1_IT_TC5); 
			 				
        if(0 == DMAReciveState)
        {					  
            if(HOST_MSG_START_CODE_FIRST_BYTE== USART1_DMA_RxBuf[0])
            {
                DMAReciveState = 1;
                USART1_Start_DMA_Recv((void*)((char *)USART1_DMA_RxBuf + 1), HOST_MSG_START_CODE_LEN-1);//start byte ok,get next 3bytes
            }
            else
            {
               USART1_Start_DMA_Recv((void*)USART1_DMA_RxBuf, 1);
            }
        }
        else if(1 == DMAReciveState)
        {
						          
						if(USART1_DMA_RxBuf[1] =='N' && USART1_DMA_RxBuf[2] =='<')
            {
                 DMAReciveState = 2;
								 expDataLen  = USART1_DMA_RxBuf[HOST_MSG_START_CODE_LEN-1];
                 USART1_Start_DMA_Recv((void*)(USART1_DMA_RxBuf+HOST_MSG_START_CODE_LEN),expDataLen );//start code ok,last byte is body len,get body           
						}
            else
            {
                DMAReciveState = 0;
                memset(USART1_DMA_RxBuf, 0, HOST_MSG_START_CODE_LEN);
                USART1_Start_DMA_Recv((void*)USART1_DMA_RxBuf, 1);
            }
        }
        else if(2 == DMAReciveState)
        {           	
						PutMsg2RecvQueue(USART1_DMA_RxBuf);
						memset(USART1_DMA_RxBuf, 0,HOST_MSG_START_CODE_LEN+USART1_DMA_RxBuf[HOST_MSG_START_CODE_LEN-1]);	
						DMAReciveState = 0;								
					  USART1_Start_DMA_Recv((void*)USART1_DMA_RxBuf, 1);
        }else{
						DMAReciveState = 0;	
					  USART1_Start_DMA_Recv((void*)USART1_DMA_RxBuf, 1);
			  }
    }
}
void DMA1_Channel4_IRQHandler(void)// send complete
{
		if(DMA_GetITStatus(DMA1_IT_TC4) !=RESET)
    {
         DMA_ClearITPendingBit(DMA1_IT_TC4);
         DMA_Cmd(DMA1_Channel4,DISABLE);
         gDMA1Channel4Running = 0;
				 
    }
		if(DMA_GetITStatus(DMA1_IT_TE4) !=RESET)
    {
         DMA_ClearITPendingBit(DMA1_IT_TE4);
         DMA_Cmd(DMA1_Channel4,DISABLE);
         gDMA1Channel4Running = 0;
				 
    }
}
