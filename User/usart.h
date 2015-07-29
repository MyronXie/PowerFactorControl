#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>
#include "lcd.h"

void USART_Config(void);
int fputc(int ch, FILE *f);
uint8_t USART_Scanf(void);



#endif /*__USART_H*/
