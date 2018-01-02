#ifndef __LED_H
#define	__LED_H
#include "../prosses/tasks.h"

#define LED_PIN      GPIO_Pin_8
#define LED_PORT       GPIOA
 
void LED_Init(void);
void LED_Open(void);
void LED_Close(void);
void LED_Toggle(void);
void LED_ON_OFF(void);
#endif /* __LED_H */

