#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

void TIM_Config(void);
void TIM_ReConfigFreq(u8 freq);
void TIM_ReConfigDuty(u8 freq, u8 duty);
void TIM_Close(void);



#endif /*__TIM_H*/
