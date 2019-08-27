/**
  ******************************************************************************
  * @file    pwm.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   pwm
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 	
#include "pwm.h"
  
/* Private variables ---------------------------------------------------------*/


//aDitherTable 查表用
uint8_t aDitherTable[32][32]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}; 
//vDitherTemp  用于根据LoadPWM的数值查表并处理成最终所需的波形
uint32_t vDitherTemp[DitherBit];
//vDither DMA的起始地址，保存两份同样的vDitherTemp用于勿扰写入Half interupt的时候写入前半段，TC 写入后半段
uint32_t vDither[DitherBit*2];
	
uint8_t BufferSize = DitherBit*2;

uint16_t duty_Cycle ;//取整，根据输出频率决定的基本分辨率
uint8_t Dither_Index;//余数，增强分辨率部分
	
uint8_t PWM_Value_FirstPart_Changed=0;
uint8_t PWM_Value_SecondPart_Changed=0;
	
/* Private function prototypes -----------------------------------------------*/

HAL_StatusTypeDef TIM1_PWM_UPDATE_DMA(TIM_HandleTypeDef *htim, uint32_t *pData,uint16_t Length);
void UpdateDitherTable(uint16_t *pDitherTable, uint16_t dutyCycle, uint32_t ditherV);
void UpdateIndex(void);
void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma);
void HalfTransferComplete(DMA_HandleTypeDef *DmaHandle);

	
/**************** 计算PWM重装值函数 *******************/
//注意：TIM_SetCompare4的4为ch4的寄存器（PA3）-TIM2 CH4
void    LoadPWM(uint32_t pwmval)
{	
	uint8_t i;
 
	duty_Cycle = (uint16_t)(pwmval/DitherBit);//取整
	Dither_Index = (uint16_t)(pwmval%DitherBit) ;//余数
	for(i = 0;i<DitherBit;i++){		
		vDitherTemp[i] =  (uint16_t)(duty_Cycle+aDitherTable[Dither_Index][i]);
	}
	PWM_Value_FirstPart_Changed =0;
	PWM_Value_SecondPart_Changed =0;
 
}
void StartTimPwmDMA()
{
	TIM1_PWM_UPDATE_DMA(&htim2,(uint32_t*) vDither,BufferSize);
	 /* Start the TIM1 base */	
  HAL_TIM_Base_Start(&htim2);
	
  /* Starts the PWM1 signal generation */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}
 


/**
  * @brief  Configure the TIM CCR1 update in DMA mode.
  * @param  htim : TIM handle
  * @param  pData: The source Buffer address.
  * @param  Length: The length of data to be transferred from memory to peripheral.
  * @retval HAL status
*/
HAL_StatusTypeDef TIM1_PWM_UPDATE_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length)
{

	  /* Set the DMA Period elapsed callback */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;

	  /* Set the DMA half period transfer callback */	
  htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = HalfTransferComplete;
     
  /* Enable the DMA channel */
	HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length);// Tim2-ch2
  
  /* Enable the TIM Update DMA request */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_UPDATE);	
	__HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);	
	
	/* Return function status */
 
  return HAL_OK;
}



/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  TimHandle : TIM handle
  * @retval None
  */
void HAL_TIM2_PeriodElapsedCallback(TIM_HandleTypeDef *TimHandle)
{
  /* Update the Dither generation table */
	uint8_t i= 0;
	for(i = 0;i<DitherBit;i++){
		vDither[i+DitherBit] = vDitherTemp[i];
	}
	PWM_Value_SecondPart_Changed =1;
}

/**
  * @brief  DMA half conversion complete callback
  * @note   This function is executed when the transfer half complete interrupt 
  *         is generated
  * @retval None
  */
void HalfTransferComplete(DMA_HandleTypeDef *DmaHandle)
{
 /* Update the Dither generation table */
  uint8_t i= 0;
	for(i = 0;i<DitherBit;i++){
		vDither[i] = vDitherTemp[i];
	}
	PWM_Value_FirstPart_Changed = 1;
}

/**
  * @brief  DMA conversion error callback
  * @note   This function is executed when the transfer error interrupt 
  *         is generated during DMA transfer
  * @retval None
  */
void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
	  /* DMA transfer Error handle*/
    Error_Handler();
}
/**
  * @brief  TIM DMA Period Elapse complete callback. 
  * @param  hdma : pointer to DMA handle.
  * @retval None
  */
void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef* htim = ( TIM_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  htim->State= HAL_TIM_STATE_READY;
  
  HAL_TIM2_PeriodElapsedCallback(htim);
}

/*********************************************END OF FILE**********************/
