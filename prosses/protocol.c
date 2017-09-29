#include "protocol.h"
#include "led.h"
#include "sdadc.h"
#include "rs485.h" 
#include "pwm.h" 

#include "pid.h" 
  
extern u8 RS485_TX_BUF[64];
unsigned char checksumCalc(unsigned char rec[])
{ 
	return (( unsigned char)rec[0])^(( unsigned char)rec[1])^(( unsigned char)rec[2])^(( unsigned char)rec[3]);
} 
//4 byte to float
float Byte2Float(unsigned char* byteArry)
{
 return *((float*)byteArry);
} 
void parseData(u8 *buf,u8 rxlen){
	//$ M > len-5 FLAG 【DATA】 Xlen(checksum)
 
	 
  float paramValue;
	uint8_t sum = 0,i;
	uint8_t len = buf[3] + 5;
  uint8_t  byteArry[4];
	for(i=3;i<len;i++)
		sum ^= buf[i];
	//if( sum!=buf[len] )	return;	//数据校验
	if( buf[0] != '$' )	return;	//数据校验
	if( buf[1] != 'N' )	return;	//数据校验
	if( buf[2] != '<' )	return;	//上位机下发
	 
	
 	byteArry[0] = buf[5];
	byteArry[1] = buf[6];
	byteArry[2] = buf[7];
	byteArry[3] = buf[8];
	
	paramValue = Byte2Float(byteArry);
 
	 //RS485_Send_Data(RS485_RX_BUF,len);
	switch(buf[4]){
  
	case _U_SetVotage:
		SetSetPoint(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetSetPoint:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//0 
	case _U_SetPTerm:
		SetPIDparam_P_inc(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPIDparam_P_inc:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//2
	case _U_SetITerm:
		SetPIDparam_I_inc(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPIDparam_I_inc:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//4
	case _U_SetDTerm:
		SetPIDparam_D_inc(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPIDparam_D_inc:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//6
	case _U_SetPWMVal:
		SetPWMValue(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPWMValue:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_GetVotage:
		sprintf((char *)RS485_TX_BUF,"@P%.3f,%.3f,%d\n",GetADCVoltage(0),GetADCVoltage(1),GetPIDOutput());
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetDura:
		GetPIDStatu((char *)RS485_TX_BUF);
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetTClose:
		SetTClose();
		sprintf((char*)RS485_TX_BUF,"SetTClose OK\n");
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetTOpen:
		SetTOpen();
		sprintf((char*)RS485_TX_BUF,"SetTOpen OK\n");
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetTPID:
		SetTPID();
		sprintf((char*)RS485_TX_BUF,"SetTPID OK\n");
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetVotageTimes:
		SetADCMeanWindow(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetVotageTimes:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetPIDMode:
		SetPIDMode(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPIDMode:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetPIDPeriod: 
		SetPIDPeriod(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPIDPeriod:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;//8
	case _U_SetTIM4Prescaler: 
		TIM2_Set_Prescaler(paramValue);
		sprintf((char*)RS485_TX_BUF,"TIM2_Set_Prescaler:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetPIDVotageChanel: 
		SetPIDVotageChanel(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPIDVotageChanel:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetPIDThredHold: 
		SetPIDThredHold(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetPIDThredHold:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetForwardA: 
		SetForwardA(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetForwardA:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetForwardB:  
		SetForwardB(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetForwardB:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetForwardC:  
		SetForwardC(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetForwardC:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetBackwardA:  
		SetBackwardA(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetBackwardA:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetBackwardB:  
		SetBackwardB(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetBackwardB:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	case _U_SetBackwardC:  
		SetBackwardC(paramValue);
		sprintf((char*)RS485_TX_BUF,"SetBackwardC:%.3f\n",paramValue);
		RS485_PrintString(RS485_TX_BUF);
break;
	default:
		sprintf((char*)RS485_TX_BUF,"unknow cmd\n");
		RS485_PrintString(RS485_TX_BUF);
break;
	}	
   
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
