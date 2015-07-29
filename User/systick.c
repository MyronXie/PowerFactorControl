#include "systick.h"

static __IO uint32_t TimingDelay;

void SYSTICK_Config(void)
{
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    while (1);
  }

}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

