#include "stm32f10x.h"
#include <stdio.h>
#include "gpio.h"
#include "systick.h"
#include "lcd.h"
#include "usart.h"
#include "adc.h"
#include "tim.h"
#include "dac.h"
#include "PFMeasure.h"

extern __IO uint16_t ADCConvertedValue[5*256];
float ADC_ConvertedValueLocal;

uint32_t Vin[NPT];
uint32_t Vin_FFT[NPT];
uint32_t Vin_Mag[NPT];
uint32_t Iin[NPT];
uint32_t Iin_FFT[NPT];
uint32_t Iin_Mag[NPT];
uint32_t Iout[NPT];
uint16_t i;
float BasePercent;
float Iin_Phase,Vin_Phase;
float PowerFactor;

uint32_t Vin_total;
uint32_t Iin_total;
uint32_t Iout_total;


int main(void)
{
	GPIO_Config();
	SYSTICK_Config();
	LCD_Init();
	ADC_Config();
	USART_Config();
	DAC_Mode_Init();


	LCD_Clear(0, 0, 240, 320, BACKGROUND);
	LCD_DispStr(40, 60, (uint8_t*)"Iin_Base:", BLACK);
	LCD_DispStr(40, 90, (uint8_t*)"Iin_Phase:", BLACK);
	LCD_DispStr(40, 110, (uint8_t*)"Vin_Phase:", BLACK);
	LCD_DispStr(40, 140, (uint8_t*)"PF:", BLACK);
//	
//	Init_Iin(Iin);
//	Init_Vin(Vin);
//	
//	cr4_fft_256_stm32(Iin_FFT,Iin,NPT);
//	cr4_fft_256_stm32(Vin_FFT,Vin,NPT);
//	
//	GetPowerMag(Iin_Mag,Iin_FFT);
//	
//	BasePercent = GetBasePercent(Iin_Mag);
//	
//	Iin_Phase = GetPhase(Iin_FFT);
//	
//	Vin_Phase = GetPhase(Vin_FFT);
//	
//	PowerFactor = GetPFValue(BasePercent, Iin_Phase, Vin_Phase);
//	
//	LCD_DispADC(120, 60, BasePercent, BLACK);
//	LCD_DisNum(120, 90, (uint16_t)Iin_Phase, BLACK);
//	LCD_DisNum(120, 110, (uint16_t)Vin_Phase, BLACK);
//	LCD_DispADC(120, 140, PowerFactor, BLACK);
	
	while(1)
	{
		Vin_total=0;
		Iin_total=0;
		Iout_total=0;
		for(i=0;i<NPT;i++)
		{
				Vin[i]=ADCConvertedValue[i*5];
				//Vin_total+=Vin[i];
				Iin[i]=ADCConvertedValue[i*5+1];
				//Iin_total+=Iin[i];
				Iout[i]=ADCConvertedValue[i*5+2];
				//Iout_total+=Iout[i];
		}
//		Vin_total=Vin_total/256;
//		Iin_total=Iin_total/256;
//		Iout_total=Iout_total/256;		

//		LCD_DisNum(120, 90, Vin_total, BLACK);
//		LCD_DisNum(120, 110, Iin_total, BLACK);
//		LCD_DisNum(120, 130, Iout_total, BLACK);
		
	cr4_fft_256_stm32(Iin_FFT,Iin,NPT);
	cr4_fft_256_stm32(Vin_FFT,Vin,NPT);
	
	GetPowerMag(Iin_Mag,Iin_FFT);
	
	BasePercent = GetBasePercent(Iin_Mag);
	
	Iin_Phase = GetPhase(Iin_FFT);
	
	Vin_Phase = GetPhase(Vin_FFT);
	
	PowerFactor = GetPFValue(BasePercent, Iin_Phase, Vin_Phase);
	
	LCD_DispADC(120, 60, BasePercent, BLACK);
	LCD_DisNum(120, 90, (uint16_t)Iin_Phase, BLACK);
	LCD_DisNum(120, 110, (uint16_t)Vin_Phase, BLACK);
	LCD_DispADC(120, 140, PowerFactor, BLACK);
	
	
	
	}


}	//main




