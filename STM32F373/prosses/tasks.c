#include "tasks.h"
#include "pid.h"
#include "protocol.h"

extern u8 RS485_RX_BUF[64]; 
extern u8 RS485_TX_BUF[64];
 
 
void OnResetFlowDown(void)
{
	RS485_PrintString("0:\tOnResetFlowDown\n");
}
void Task_500HZ(void)
{	
 	  u8 len;
		//Debug1_H;
		VOL_IIR_Filter();		 
		if (is_PID_Running()){
			PID_Setpoint_Change();
		}
		//delay_ms(1);
		//Debug1_L; 
}

 
void Task_200HZ(void)
{
	//Debug2_H;
	if (is_PID_Running()) PID_Start();
	//delay_ms(1);
	//Debug2_L;
}
 
void Task_10HZ(void)
{
	//Debug3_H;
	if(IsFlowReset()){
			//LED_ON_OFF();
			OnResetFlowDown();
	  }
	//delay_ms(1);
	//Debug3_L;
}
			
