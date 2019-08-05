/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "mb.h"
#include "mbport.h"
#include "user_mb_app.h"

#include "sdadc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId IIR_FilterHandle;
osThreadId eMBPollHandle;
osThreadId PIDHandle;
osThreadId CheckKeyDownHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartIIR_Filter(void const * argument);
void StarteMBPoll(void const * argument);
void StartPID(void const * argument);
void StartCheckKeyDown(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of IIR_Filter */
  osThreadDef(IIR_Filter, StartIIR_Filter, osPriorityNormal, 0, 128);
  IIR_FilterHandle = osThreadCreate(osThread(IIR_Filter), NULL);

  /* definition and creation of eMBPoll */
  osThreadDef(eMBPoll, StarteMBPoll, osPriorityHigh, 0, 128);
  eMBPollHandle = osThreadCreate(osThread(eMBPoll), NULL);

  /* definition and creation of PID */
  osThreadDef(PID, StartPID, osPriorityRealtime, 0, 128);
  PIDHandle = osThreadCreate(osThread(PID), NULL);

  /* definition and creation of CheckKeyDown */
  osThreadDef(CheckKeyDown, StartCheckKeyDown, osPriorityBelowNormal, 0, 128);
  CheckKeyDownHandle = osThreadCreate(osThread(CheckKeyDown), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    
    
    
    
    

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartIIR_Filter */
/**
* @brief Function implementing the IIR_Filter thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIIR_Filter */
void StartIIR_Filter(void const * argument)
{
  /* USER CODE BEGIN StartIIR_Filter */
  /* Infinite loop */
  for(;;)
  {
		VOL_IIR_Filter();
    osDelay(500);	
  }
  /* USER CODE END StartIIR_Filter */
}

/* USER CODE BEGIN Header_StarteMBPoll */
/**
* @brief Function implementing the eMBPoll thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StarteMBPoll */
void StarteMBPoll(void const * argument)
{
  /* USER CODE BEGIN StarteMBPoll */
  /* Infinite loop */
  for(;;)
  {
		 osDelay(10);
		 eMBPoll( );		 
  }
  /* USER CODE END StarteMBPoll */
}

/* USER CODE BEGIN Header_StartPID */
/**
* @brief Function implementing the PID thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPID */
void StartPID(void const * argument)
{
  /* USER CODE BEGIN StartPID */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
			
  }
  /* USER CODE END StartPID */
}

/* USER CODE BEGIN Header_StartCheckKeyDown */
/**
* @brief Function implementing the CheckKeyDown thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCheckKeyDown */
void StartCheckKeyDown(void const * argument)
{
  /* USER CODE BEGIN StartCheckKeyDown */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCheckKeyDown */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
