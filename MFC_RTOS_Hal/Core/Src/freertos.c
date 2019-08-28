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
/* Virtual address Tab defined by the user: 0x0000 and 0xFFFF values are prohibited */
uint16_t VirtAddVarTabTest[NB_OF_VARIABLES];
uint32_t Index = 0;
__IO uint32_t ErasingOnGoing = 0;
uint32_t VarDataTab[NB_OF_VARIABLES] = {0};
uint32_t VarValue=0;
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
osThreadId SDADCIIRFilterHandle;
osThreadId eMBPollHandle;
osThreadId PIDHandle;
osThreadId CheckKeyDownHandle;
osThreadId TaskMonitorHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartSDADCIIRFilter(void const * argument);
void StarteMBPoll(void const * argument);
void StartPID(void const * argument);
void StartCheckKeyDown(void const * argument);
void StartTaskMonitor(void const * argument);

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

  /* definition and creation of SDADCIIRFilter */
  osThreadDef(SDADCIIRFilter, StartSDADCIIRFilter, osPriorityNormal, 0, 128);
  SDADCIIRFilterHandle = osThreadCreate(osThread(SDADCIIRFilter), NULL);

  /* definition and creation of eMBPoll */
  osThreadDef(eMBPoll, StarteMBPoll, osPriorityHigh, 0, 128);
  eMBPollHandle = osThreadCreate(osThread(eMBPoll), NULL);

  /* definition and creation of PID */
  osThreadDef(PID, StartPID, osPriorityRealtime, 0, 128);
  PIDHandle = osThreadCreate(osThread(PID), NULL);

  /* definition and creation of CheckKeyDown */
  osThreadDef(CheckKeyDown, StartCheckKeyDown, osPriorityBelowNormal, 0, 128);
  CheckKeyDownHandle = osThreadCreate(osThread(CheckKeyDown), NULL);

  /* definition and creation of TaskMonitor */
  osThreadDef(TaskMonitor, StartTaskMonitor, osPriorityRealtime, 0, 128);
  TaskMonitorHandle = osThreadCreate(osThread(TaskMonitor), NULL);

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

/* USER CODE BEGIN Header_StartSDADCIIRFilter */
/**
* @brief Function implementing the SDADCIIRFilter thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSDADCIIRFilter */
void StartSDADCIIRFilter(void const * argument)
{
  /* USER CODE BEGIN StartSDADCIIRFilter */
	
  /* Infinite loop */
  for(;;)
  {
		VOL_IIR_Filter();
		//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);			
    osDelay(10);
  }
  /* USER CODE END StartSDADCIIRFilter */
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
    osDelay(10);//一定要空闲等待，要不高优先级的任务不会退出导致其他任务无法执行！
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
		if(TRUE == PID_isRunning()){
			PID_Start();
			osDelay(Get_ControlCycle());
		}else{
			osDelay(1000);
		}
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
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
	static USHORT value = 0;
  for(;;)
  {
		value+= 1000;
		if(value>65335)
			value = 4096;		
		AD5761_SetVotage(value);
		//AD5761_Config();
    osDelay(2000);
  }
  /* USER CODE END StartCheckKeyDown */
}

/* USER CODE BEGIN Header_StartTaskMonitor */
/**
* @brief Function implementing the TaskMonitor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskMonitor */
void StartTaskMonitor(void const * argument)
{
  /* USER CODE BEGIN StartTaskMonitor */
  /* Infinite loop */
	 
  for(;;)
  {
    osDelay(5000);
  }
  /* USER CODE END StartTaskMonitor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
