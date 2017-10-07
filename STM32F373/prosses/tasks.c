//***************************************************************************************
//烈火微型四轴飞行器源码版权归烈火团队所有，未经烈火团队同意，请勿随意在网上传播本源码。
//与本软件相关书籍<<四轴飞行器DIY-基于STM32微控制器>>，由北航出版社正式出版，内容对本套包
//含的所有软件以及硬件相关都做了详细的讲解，有兴趣的网友可以从各大书店购买。
//与本软件配套的硬件：http://fire-dragon.taobao.com
//如果有网友做了各种有意义的改进，请随时与我们保持联系。
//QQ：16053729    烈火QQ群：234879071
//***************************************************************************************
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
		Debug1_H;
		VOL_IIR_Filter();		
		RS485_Receive_Data(RS485_RX_BUF,&len);
		if(len)
		{
			 parseData(RS485_RX_BUF,len);
 		}
		if (is_PID_Running()){
		PID_Setpoint_Change();
		}
		delay_ms(1);
		Debug1_L; 
}

 
void Task_200HZ(void)
{
	Debug2_H;
	if (is_PID_Running()) PID_Start();
	delay_ms(1);
	Debug2_L;
}
 
void Task_10HZ(void)
{
	Debug3_H;
	if(IsFlowReset()){
			//LED_ON_OFF();
			OnResetFlowDown();
	  }
	delay_ms(1);
	Debug3_L;
}
			
