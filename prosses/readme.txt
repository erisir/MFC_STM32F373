硬件使用说明：
RS485:

	PD8 485控制
	PA9 485_TX
	PA10 485_RX

LED:
	PA8 LED



PWM:
	TIM2cH2 PA1=>tim2CH4 pA3
	
SDADC:
	PB0 PB1
	
主循环控制时钟:
	TIM4

延时控制时钟

	systick
	
FlowReset:

	PA13
	
DEBUG

35 36 37 38
PF6 PF7 PA14 PA15

不能同时发送和接收 否则 cnt 变零

剩余
1、 modbus协议 可参考例子，FreeModbus..
2、分段PID实现