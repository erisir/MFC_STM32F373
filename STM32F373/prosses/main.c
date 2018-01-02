/**
  ******************************************************************************
  * @file    main.c
  * @author  deadnight
  * @version V1.0
  * @date    2018-01-02
  * @brief   main
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#include "tasks.h" 

extern uint16_t Counters[CounterNum];
 
int main(void)
{ 
		Bsp_Int();		 
		while(1)
		{						
				if(Counters[0]>=getFeedBackTime())//~50Hz
				{			
						Counters[0] = 0;		
						if (is_PID_Running()) PID_Start();	
				}				
				if(Counters[1]>=10)//100Hz
				{				
						Counters[1] = 0;		
						VOL_IIR_Filter(); 
						if (is_PID_Running()){
								PID_Setpoint_Change();
						}		 
				}
				if(Counters[2]>=500)//2Hz
				{
						Counters[2] = 0;
						if(IsFlowReset()){
								OnResetFlowDown();
						}
				} 
		}
}
