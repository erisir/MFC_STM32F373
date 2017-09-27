
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "led.h"
#include "../hardware/RS485/rs485.h" 

int main(void)
{ 
	u8 rs485buf[64]; 
	u8 key;
	delay_init();
	LED_Init();		  
	
	KEY_Init();
	RS485_Init(115200);
	//RS485_ON_OFF();
	LED_ON_OFF();
	while(1)
	{
		if(IsFlowReset()){
			LED_ON_OFF();
			//ResetFlow();
	  }
		//RS485_Send_Data("1234\n",5);//发送5个字节 
		RS485_Receive_Data(rs485buf,&key);
		if(key)//接收到有数据
		{
			 RS485_Send_Data(rs485buf,key);//发送5个字节 
 		}
		delay_ms(100);
	}
}
