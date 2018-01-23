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
extern uint8_t commERROR;
int main(void)
{ 
		Bsp_Int();		 
		while(1)
		{						
				if(Counters[0]>=Get_ControlCycle())//~50Hz
				{			
						Counters[0] = 0;
						if (PID_isRunning()) PID_Start();	
				}				
				if(Counters[1]>=20)//100Hz
				{				
						Counters[1] = 0;		
						VOL_IIR_Filter(); 
				}
				if(Counters[2]>=500)//2Hz
				{
						Counters[2] = 0;
						if(IsFlowReset()){
								OnResetFlowDown();
						}
						 
				} 
				if(Counters[3]>=2)
				{
					Counters[3] = 0;
					ParseMsgRecvQueue();
				}
		}
}
