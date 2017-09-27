#include "mfc_usart1.h"

#include "../pid/mfc_pid.h"
#include "../adc/mfc_adc.h"
#include "../pwm/mfc_pwm.h"
#include "../TimBase/mfc_TiMbase.h"

extern __IO unsigned char busy; 
extern __IO  char uart_flag;
extern __IO unsigned char uart_start_flag;  
extern __IO unsigned char receive[16];

uint8_t TxCount=0;
uint8_t Count=0;

static uint8_t TxBuff[256];	//串口发送缓冲区
volatile uint8_t RxBuffer[50];//串口接收缓冲区

/**
 * @brief  USART1 GPIO 配置,工作模式配置。9600 8-N-1
 * @param  无
 * @retval 无
 */
void USART1_Init(void)
{
	USART1_GPIO_Config();
	USART1_NVIC_Config();
	RS485_TX_EN = 0;
}
void USART1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART1 clock */
	
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );

	/* USART1 GPIO config */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_7);        
	/*
	*  USART_TXDEN -> PA8 USART1_TX -> PA9 , USART1_RX ->        PA10
	*/    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
	
 
}

/// 配置USART1接收中断
void USART1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

	return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
	/* 等待串口1输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
}

unsigned char checksumCalc(unsigned char rec[])
{ 

	return (( unsigned char)rec[0])^(( unsigned char)rec[1])^(( unsigned char)rec[2])^(( unsigned char)rec[3]);
} 
//4 byte to float
float Byte2Float(unsigned char* byteArry)
{
 return *((float*)byteArry);
}
void parseCMD(){
	float v_data;
	unsigned char  byteArry[4];
	uart_flag = 0;
	
	byteArry[0] = receive[2];
	byteArry[1] = receive[3];
	byteArry[2] = receive[4];
	byteArry[3] = receive[5];
	
	v_data =  Byte2Float(byteArry);//receive[2]*256+receive[3] ;
	//printf("v_data=%.4f\n",v_data);
	RS485_TX_EN = 1;
	switch(receive[1]){
  
	case _U_SetVotage:
		SetSetPoint(v_data);
		printf("SetSetPoint:%.3f\n",v_data);
		break;//0 
	case _U_SetPTerm:
		SetPIDparam_P_inc(v_data);
		printf("SetPIDparam_P_inc:%.3f\n",v_data);
		break;//2
	case _U_SetITerm:
		SetPIDparam_I_inc(v_data);
		printf("SetPIDparam_I_inc:%.3f\n",v_data);
		break;//4
	case _U_SetDTerm:
		SetPIDparam_D_inc(v_data);
		printf("SetPIDparam_D_inc:%.3f\n",v_data);
		break;//6
	case _U_SetPWMVal:
		SetPWMValue(v_data);
		printf("SetPWMValue:%.3f\n",v_data);
		break;//8
	case _U_GetVotage:
		GetPosition();
		break;//8
	case _U_SetDura:
		GetPIDStatu();
		break;//8
	case _U_SetTClose:
		SetTClose();
		printf("SetTClose OK\n");
		break;//8
	case _U_SetTOpen:
		SetTOpen();
		printf("SetTOpen OK\n");
		break;//8
	case _U_SetTPID:
		SetTPID();
		printf("SetTPID OK\n");
		break;//8
	case _U_SetVotageTimes:
		SetVotageTimes(v_data);
		printf("SetVotageTimes:%.3f\n",v_data);
		break;//8
	case _U_SetPIDMode:
		SetPIDMode(v_data);
		printf("SetPIDMode:%.3f\n",v_data);
		break;//8
	case _U_SetPIDPeriod: 
		SetPIDPeriod(v_data);
		printf("SetPIDPeriod:%.3f\n",v_data);
		break;//8
	case _U_SetTIM4Prescaler: 
		TIM2_Set_Prescaler(v_data);
		printf("TIM2_Set_Prescaler:%.3f\n",v_data);
		break;
	case _U_SetPIDVotageChanel: 
		SetPIDVotageChanel(v_data);
		printf("SetPIDVotageChanel:%.3f\n",v_data);
		break;
	case _U_SetPIDThredHold: 
		SetPIDThredHold(v_data);
		printf("SetPIDThredHold:%.3f\n",v_data);
		break;
	case _U_SetForwardA: 
		SetForwardA(v_data);
		printf("SetForwardA:%.3f\n",v_data);
		break;
	case _U_SetForwardB:  
		SetForwardB(v_data);
		printf("SetForwardB:%.3f\n",v_data);
		break;
	case _U_SetForwardC:  
		SetForwardC(v_data);
		printf("SetForwardC:%.3f\n",v_data);
		break;
	case _U_SetBackwardA:  
		SetBackwardA(v_data);
		printf("SetBackwardA:%.3f\n",v_data);
		break;
	case _U_SetBackwardB:  
		SetBackwardB(v_data);
		printf("SetBackwardB:%.3f\n",v_data);
		break;
	case _U_SetBackwardC:  
		SetBackwardC(v_data);
		printf("SetBackwardC:%.3f\n",v_data);
		break;
	default:
		printf("unknow cmd\n");
		break;
	}	
  RS485_TX_EN = 0;	
}
unsigned char cmd_ready()
{
	return uart_flag; 
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
