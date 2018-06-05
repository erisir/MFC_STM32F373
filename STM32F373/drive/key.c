/**
  ******************************************************************************
  * @file    key.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   key
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "key.h"
 

void KEY_Init(void)
{	
	GPIO_SetBits(GPIOA, GPIO_Pin_13);
}
uint8_t IsFlowReset(void){
    return KEY_Down(GPIOA, GPIO_Pin_13);
}

uint8_t KEY_Down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
		  /*检测是否有按键按下 */
		uint16_t timeout = 0;
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 ) 
	  {	   
	 	   /*延时消抖*/
	  	 delay_ms(10);		
	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 )  
				{	 
					/*等待按键释放 */
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == 0 && timeout<3000){
						timeout++;
						delay_ms(1);
					} 
					if(timeout>=3000)
						return 	1	; 
					else
						return 0;
				}
			else
					return 0;
		}
	else
		return 0;
}

