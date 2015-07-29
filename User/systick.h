#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SYSTICK_Config(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);



#endif /*__SYSTICK_H*/
