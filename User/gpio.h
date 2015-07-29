#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"
#include "systick.h"

void GPIO_Config(void);
void LED_ON(u8 light);
void LED_OFF(u8 light);
u8 Key_Scan(u8 button);



#endif /*__GPIO_H*/
