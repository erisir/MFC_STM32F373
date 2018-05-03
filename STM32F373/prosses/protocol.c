/**
  ******************************************************************************
  * @file    protocol.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   protocol
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "protocol.h"
extern uint8_t commERROR;
extern u8 RS485_TX_BUF[64];
extern int16_t SDADC_ValueTable[256];
int16_t tempBuf[256];
uint8_t * pBuf;
unsigned char checksumCalc(unsigned char rec[])
{ 
	return (( unsigned char)rec[0])^(( unsigned char)rec[1])^(( unsigned char)rec[2])^(( unsigned char)rec[3]);
} 
//4 byte to float
float Byte2Float(unsigned char* buf,int offset)
{
	uint8_t  byteArry[4];
	byteArry[0] = buf[offset];
	byteArry[1] = buf[offset+1];
	byteArry[2] = buf[offset+2];
	byteArry[3] = buf[offset+3];
 return *((float*)byteArry);
} 
void ParseData(uint8_t *buf){
	//$ N > len-5 FLAG 【DATA】 Xlen(checksum)
	//uint8_t len = buf[3];
  uint16_t  data = 0;
	uint8_t len = 0;
	uint8_t i=0;
	uint32_t pwmValue =0;
	
	if( buf[0] != '$' )	return;	//数据校验
	if( buf[1] != 'N' )	return;	//数据校验
	if( buf[2] != '<' )	return;	//上位机下发
	//if( Get_Checksum(buf)!=buf[len] )	return;	//数据校验
	commERROR = 0;
 
	switch(buf[4]){
	case _CMD_SetFuzzyMap:
		Set_FuzzyMap_Param(buf);
		sprintf((char*)RS485_TX_BUF,"SetFuzzyMap:OK\n");
		RS485_PrintString(RS485_TX_BUF);
		break;
 
	case _CMD_ReadFuzzyMap:
		  for(i=0;i<7;i++){
			Get_FuzzyMap_Param(RS485_TX_BUF,i);
			USART1_Start_DMA_Send(RS485_TX_BUF,(uint8_t)RS485_TX_BUF[3]);
			delay_ms(50);
			}
			break;
  case _U_DEBUG:
		pBuf = (uint8_t*)SDADC_ValueTable;
		memcpy((uint8_t *)tempBuf,pBuf,512);	
		pBuf = (uint8_t *)tempBuf;
		for(len = 0;len<8;len++){
			memcpy(RS485_TX_BUF,pBuf,64);	
			USART1_Start_DMA_Send(RS485_TX_BUF,64);
			pBuf +=64;
		}		
		break;
	case _CMD_SetRunParam:
		Set_Running_Param(buf);
		sprintf((char*)RS485_TX_BUF,"Set_Running_Param:OK\n");
		RS485_PrintString(RS485_TX_BUF);
		break;//
	case _U_SetVOut:
		data = buf[5]*256+buf[6];
		sprintf((char*)RS485_TX_BUF,"_U_SetVOut[%d]:OK\n",data);
		RS485_PrintString(RS485_TX_BUF);
		AD5761_SetVotage(data);
		break;//
	
	case _CMD_SetPIDParam:
		Set_PID_Param(buf);
		sprintf((char*)RS485_TX_BUF,"Set_PID_Param:OK\n");
		RS485_PrintString(RS485_TX_BUF);

	 break;//
	 
	case _CMD_GetRunParam:
		Get_Running_Param(RS485_TX_BUF);
		USART1_Start_DMA_Send(RS485_TX_BUF,(uint8_t)RS485_TX_BUF[3]);
	 break;//
	 
	case _CMD_GetPIDParam:
    Get_PID_Param(RS485_TX_BUF);
		USART1_Start_DMA_Send(RS485_TX_BUF,(uint8_t)RS485_TX_BUF[3]);
	 break;//

	case _CMD_SetVClose:
		Valve_Close();
		sprintf((char*)RS485_TX_BUF,"SetVClose OK\n");
		RS485_PrintString(RS485_TX_BUF);
break;

	case _U_SetVOpen:
		Valve_Open();
		sprintf((char*)RS485_TX_BUF,"SetVOpen OK\n");
		RS485_PrintString(RS485_TX_BUF);
break;

	case _U_SetVPID:
		Valve_PID_Ctrl();
		sprintf((char*)RS485_TX_BUF,"SetVPIDCtrl OK\n");
		RS485_PrintString(RS485_TX_BUF);
break;

	default:
		sprintf((char*)RS485_TX_BUF,"unknow cmd unknow cmd unknow cmd unknow cmd unknow cmd\n");
		RS485_PrintString(RS485_TX_BUF);
break;
	}	
   
}
uint8_t Get_Checksum(u8 *buf)
{	
	uint8_t i;
    uint8_t checksum=0;
	uint8_t length = buf[3];

    for(i=0;i<length+2;i++)
    {
        checksum ^= (buf[3+i]&0xFF);
    }
    return checksum;
}
//以HEX的形式输出U8型数据
void PrintHexU8(uint8_t data)
{
//	TxBuff[Count++] = data;  
//	if(!(USART1->CR1 & USART_CR1_TXEIE))
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //打开TXE中断
}
//以HEX的形式输出S16型数据
void PrintHexS16(int16_t num)
{
	PrintHexU8((uint8_t)(num & 0xff00) >> 8);//先发送高8位，再发送低8位
	PrintHexU8((uint8_t)(num & 0x00ff));
}
//以字符的形式输出S8型数据
void PrintS8(int8_t num)
{
	uint8_t  bai,shi,ge;
	if(num<0)
	{
		PrintHexU8('-');
		num=-num;
	}
	else 
		PrintHexU8(' ');	
	bai=num/100;
	shi=num%100/10;
	ge =num%10;
	PrintHexU8('0'+bai);
	PrintHexU8('0'+shi);
	PrintHexU8('0'+ge);
}
//以字符的形式输出U8型数据
void PrintU8(uint8_t num)
{
	uint8_t  bai,shi,ge;
	bai=num/100;
	shi=num%100/10;
	ge =num%10;
	PrintHexU8('0'+bai);
	PrintHexU8('0'+shi);
	PrintHexU8('0'+ge);
}
//以字符的形式输出S16型数据 
void PrintS16(int16_t num)
{	
	uint8_t w5,w4,w3,w2,w1;
	if(num<0)
	{
		PrintHexU8('-');
		num=-num;
	}
	else 
		PrintHexU8(' ');
	
	w5=num%100000/10000;
	w4=num%10000/1000;
	w3=num%1000/100;
	w2=num%100/10;
	w1=num%10;
	PrintHexU8('0'+w5);
	PrintHexU8('0'+w4);
	PrintHexU8('0'+w3);
	PrintHexU8('0'+w2);
	PrintHexU8('0'+w1);
}
//以字符的形式输出U16型数据
void PrintU16(uint16_t num)
{	
	uint8_t w5,w4,w3,w2,w1;
	
	w5=num%100000/10000;
	w4=num%10000/1000;
	w3=num%1000/100;
	w2=num%100/10;
	w1=num%10;
	PrintHexU8(' ');
	PrintHexU8('0'+w5);
	PrintHexU8('0'+w4);
	PrintHexU8('0'+w3);
	PrintHexU8('0'+w2);
	PrintHexU8('0'+w1);
}
//输出字符串
void PrintString(uint8_t *s)
{
	uint8_t *p;
	p=s;
	while(*p != '\0')
	{
		PrintHexU8(*p);
		p++;
	}
}
/*********************************************END OF FILE**********************/
