硬件使用说明：
RS485:	 
	PIN 30 PA9  485_TX
	PIN 31 PA10 485_RX

LED:
	PIN 29 PA8 LED

PWM:
	PIN39 PB3 TIM2 CH2
	
SDADC:
	(sensor)PIN 21 PE8
	(setPoint)PIN 22 PE9
	
主循环控制时钟:
	TIM4

延时控制时钟

	systick
	
FlowReset:
	PIN 34 PA13 TIM4 CH3
	
DEBUG

35 36 37 38
PF6 PF7 PA14 PA15

不能同时发送和接收 否则 cnt 变零

剩余
1、 modbus协议 可参考例子，FreeModbus..
2、分段PID实现