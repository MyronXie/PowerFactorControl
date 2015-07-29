#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"

void RTC_Config(void);
void RTC_Configuration(void);
void Time_Adjust(void);
void Time_Show(void);
void Time_Display(uint32_t TimeVar);


#endif /*__RTC_H*/
