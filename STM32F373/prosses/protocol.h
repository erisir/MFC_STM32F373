#ifndef __USART1_H
#define	__USART1_H

#include "tasks.h"

#define _CMD_SetRunParam	 '0'
#define _CMD_SetPIDParam	 '1'

#define _CMD_GetRunParam	 '2'
#define _CMD_GetPIDParam	 '3'

#define _CMD_SetVClose		 '4'
#define _U_SetVOpen				 '5'
#define _U_SetVPID				 '6'
#define _U_SetVOut				 '7'


#define RS485_TX_EN		PDout(8)	//485模式控制.0,接收;1,发送.

float Byte2Float(unsigned char* buf,int offset);
void ParseData(uint8_t *buf);
void PrintHexU8(uint8_t data);
void PrintHexS16(int16_t num);
void PrintS8( int8_t num);
void PrintU8(uint8_t num);
void PrintS16( int16_t num);
void PrintU16(uint16_t num);
void PrintString(uint8_t *s);
void PrintString(uint8_t *s);

uint8_t Get_Checksum(u8 *buf);
#endif /* __USART1_H */
