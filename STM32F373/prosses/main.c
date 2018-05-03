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
extern uint32_t runningCounter;
uint8_t ticFlag[4];
uint32_t ticTimers[4];
uint32_t tocTimers[4];
void tic(uint8_t index)
{
	ticFlag[index]=5;
	ticTimers[index]=runningCounter;

}
void toc(uint8_t index)
{
	ticFlag[index]=0;
	tocTimers[index]=runningCounter-ticTimers[index];
}
void toggle(uint8_t index)
{
	if(ticFlag[index]<3)
			tic(index);
	else
			toc(index);
}
int main(void)
{ 
	  uint16_t a=10;
		float b=3,c=3.0,d=3.0f;
		Bsp_Int();		 
		while(1)
		{		
				if(Counters[0]>=50)//1000Hz
				{							
						//toggle(0);	
						Counters[0] = 0;							
						VOL_IIR_Filter();//40us									

				}				
				if(Counters[1]>=20)//500Hz
				{		
						//toggle(1);
						ParseMsgRecvQueue();//9 us	
						//VOL_IIR_Filter();//40us			
						Counters[1] = 0;								
				}
				if(Counters[2]>=Get_ControlCycle()*10)//5Hz
				{
						//toggle(2);
						Counters[2] = 0;						
						if (PID_isRunning()) PID_Start();	//80us											 
				} 
				if(Counters[3]>=5000)
				{			
					Counters[3] = 0;			
					if(IsFlowReset()){
								OnResetFlowDown();
						}	
				}
		}
}
