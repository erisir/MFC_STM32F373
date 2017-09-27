#ifndef __USART1_H
#define	__USART1_H

#include "stm32f37x.h"
#include "../sys/sys.h"
#include <stdio.h>

#define _U_SetVotage	 '0'
#define _U_SetPTerm      '1'
#define _U_SetITerm      '2'
#define _U_SetDTerm	     '3'
#define _U_SetDura       '4'
#define _U_SetPWMVal     '5'
#define _U_GetVotage     '6'
#define _U_SetTClose     '7'
#define _U_SetTOpen      '8'
#define _U_SetTPID       '9'
#define _U_SetVotageTimes 'a'
#define _U_SetPIDMode 	  'b'
#define _U_SetPIDPeriod   'c'
#define _U_SetTIM4Prescaler 'd'
#define _U_SetPIDVotageChanel 'e'
#define _U_SetForwardA   'f'
#define _U_SetForwardB   'g'
#define _U_SetForwardC   'h'
#define _U_SetBackwardA   'i'
#define _U_SetBackwardB   'j'
#define _U_SetBackwardC   'k'
#define _U_SetPIDThredHold 'l'

#define RS485_TX_EN		PDout(8)	//485模式控制.0,接收;1,发送.

void USART1_Init(void);
void USART1_GPIO_Config(void);
void USART1_NVIC_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
unsigned char cmd_ready(void);
void parseCMD(void);
void PrintHexU8(uint8_t data);
void PrintHexS16(int16_t num);
void PrintS8( int8_t num);
void PrintU8(uint8_t num);
void PrintS16( int16_t num);
void PrintU16(uint16_t num);
void PrintString(uint8_t *s);
#endif /* __USART1_H */
