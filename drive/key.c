#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 ;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_13);
	
	}
uint8_t IsFlowReset(void){
    return KEY_Down(GPIOA, GPIO_Pin_13);
}

uint8_t KEY_Down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
		  /*检测是否有按键按下 */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 ) 
	  {	   
	 	   /*延时消抖*/
	  	 delay_ms(10);		
	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )  
				{	 
					/*等待按键释放 */
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0);   
					return 	1	; 
				}
			else
					return 0;
		}
	else
		return 0;
}

