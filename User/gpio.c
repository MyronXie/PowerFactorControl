#include "gpio.h"

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);

  /* LED Config */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* Button Config */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void LED_ON(u8 light)
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	
	if(light == 1) {	GPIOx = GPIOB, GPIO_Pin = GPIO_Pin_0;}
	else if(light == 2) {	GPIOx = GPIOF, GPIO_Pin = GPIO_Pin_7;}
	else if(light == 2) {	GPIOx = GPIOF, GPIO_Pin = GPIO_Pin_8;}
			
	GPIO_ResetBits(GPIOx, GPIO_Pin);

}

void LED_OFF(u8 light)
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	
	if(light == 1) {	GPIOx = GPIOB, GPIO_Pin = GPIO_Pin_0;}
	else if(light == 2) {	GPIOx = GPIOF, GPIO_Pin = GPIO_Pin_7;}
	else if(light == 2) {	GPIOx = GPIOF, GPIO_Pin = GPIO_Pin_8;}
			
	GPIO_SetBits(GPIOx, GPIO_Pin);

}

u8 Key_Scan(u8 button)
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	
	if(button == 1) {	GPIOx = GPIOA, GPIO_Pin = GPIO_Pin_0;}
	else if(button == 2) {	GPIOx = GPIOC, GPIO_Pin = GPIO_Pin_13;}

	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0)
	{
		Delay(10);
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0)
		{	
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == 0){}
			return 1;	
		}
		else return 0;
	}
	else return 0;

}
