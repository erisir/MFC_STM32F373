/**
  ******************************************************************************
  * @file    tasks.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   tasks
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "tasks.h"

uint8_t Bsp_Int_Ok=0;


extern u8 RS485_RX_BUF[64]; 
extern u8 RS485_TX_BUF[64]; 
 
void OnResetFlowDown(void)
{
	LED_Blink(2);
	 
}
void Bsp_Int(void )
{
	
	//RCC_ClocksTypeDef RCC_Clocks;
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	
	delay_init();
	LED_Init();		  
	LED_ON_OFF();
 
	KEY_Init();
	TIM4_Init(); 
	PWM_Init(); 
	PID_Init() ;  
	ADC1_Init();  
	AD5761_Init() ;
  //Calculate_FilteringCoefficient(50);//sampel time,unit second,F_cutoff
 
  //	RCC_GetClocksFreq(&RCC_Clocks);
 
	
	Bsp_Int_Ok = 1;
}
void RCC_Configuration(void)
{
			RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);//USART1 PA9_TX  PA10_RX LED_PA8 FlowReset_PA13
			RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);//PWM_PB3
			RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOD, ENABLE);//USART1_EN
			RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOE, ENABLE);//SDADC_PE8 _PE9
			//RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOF, ENABLE);//DEBUG PF6 PF7 PA14 PA15
			RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1,ENABLE);  // Sdadc PWM_dma
			 
		  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);   //PWM
			RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3,  ENABLE );//SPI3
			RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );//USART1				
						

}
void NVIC_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //2bit 抢占，2bit响应 抢占优先，值小则优先 													
    
		/* USART1 Interrupt */
		 
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 //USART1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		 
   /* NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;//USART1 DMA Rx
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
		
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;//USART1 DMA Tx
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
    NVIC_Init(&NVIC_InitStructure);*/
    
		/* Counter Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		/* UART1 Timeout */
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

		NVIC_Init(&NVIC_InitStructure);

		/* PWM Interrupt */
		#ifdef __PWM_DITHER_MODE_
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init( &NVIC_InitStructure );
		#endif
		

}
void GPIO_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	  /* SDADC */		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
		
		/* reset key */		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/* LED*/			
		GPIO_InitStructure.GPIO_Pin = LED_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
		GPIO_Init(LED_PORT, &GPIO_InitStructure);
		
 	  /* PWM  */				 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1);//TIM2_CH2
		
		/* SPI  */		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3;//CLK MISO MOSI
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_6);//SPI
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_6);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_6);
		 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//NSS
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/* USART1 */
  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_7);//USART
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_7);   
}
 	
