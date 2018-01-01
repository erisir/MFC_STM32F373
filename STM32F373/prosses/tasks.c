#include "tasks.h"
#include "pid.h"
#include "rs485_DMA.h"
#include "protocol.h"

extern u8 RS485_RX_BUF[64]; 
extern u8 RS485_TX_BUF[64];
 
 
void OnResetFlowDown(void)
{
	RS485_PrintString("0:\tOnResetFlowDown\n");
}
void Task_1000HZ(void)
{		
	//u8 len;
	//RS485_Receive_Data(RS485_RX_BUF,&len);
	//if(len)
	//{
	//	parseData(RS485_RX_BUF,len);
	//}
}

void Task_500HZ(void)
{	
	LED_Open();
	if (is_PID_Running()) PID_Start();
	LED_Close();
}

void Task_200HZ(void)
{ 
		
}
void Task_100HZ(void)
{
		//updateVoltageWindowBuf();		
 		VOL_IIR_Filter(); 
		if (is_PID_Running()){
			PID_Setpoint_Change();
		}
}
 
void Task_10HZ(void)
{
	
	
	if(IsFlowReset()){
			OnResetFlowDown();
	  }

}
			
