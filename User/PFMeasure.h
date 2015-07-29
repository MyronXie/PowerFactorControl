#ifndef __PF_MEASURE_H
#define __PF_MEASURE_H

#include "stm32f10x.h"
#include "stm32_dsp.h"
#include "math.h"
#include "usart.h"

#define PI		3.14159265358979
#define PI2 	6.28318530717959
#define Fs		12800
#define NPT		256

void Init_Iin(uint32_t* ArrayIn);
void Init_Vin(uint32_t* ArrayIn);
void GetPowerMag(uint32_t* ArrayOut, uint32_t* ArrayIn);
float GetBasePercent(uint32_t* ArrayIn);
float GetPhase(uint32_t* ArrayIn);
float GetPFValue(float percent, float phase1, float phase2);

#endif /*__PF_MEASURE_H*/

