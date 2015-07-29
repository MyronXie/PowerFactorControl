/**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   adc1 Ӧ��bsp / DMA ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "dac.h"

#define DAC_DHR12RD_Address      0x40007420 //0x40007420

/* �������� ---------------------------------------------------------*/
const uint16_t Sine12bit[256] = {
	2098,2148,2199,2249,2299,2349,2398,2448,2497,2546,2594,2643,2690,2738,2785,2832,
	2878,2924,2969,3013,3057,3101,3144,3186,3227,3268,3308,3347,3386,3423,3460,3496,
	3531,3565,3599,3631,3663,3693,3722,3751,3778,3805,3830,3854,3877,3899,3920,3940,
	3959,3976,3993,4008,4022,4035,4046,4057,4066,4074,4081,4086,4090,4094,4095,4095,
	4095,4094,4090,4086,4081,4074,4066,4057,4046,4035,4022,4008,3993,3976,3959,3940,
	3920,3899,3877,3854,3830,3805,3778,3751,3722,3693,3663,3631,3599,3565,3531,3496,
	3460,3423,3386,3347,3308,3268,3227,3186,3144,3101,3057,3013,2969,2924,2878,2832,
	2785,2738,2690,2643,2594,2546,2497,2448,2398,2349,2299,2249,2199,2148,2098,2048,
	1998,1948,1897,1847,1797,1747,1698,1648,1599,1550,1502,1453,1406,1358,1311,1264,
	1218,1172,1127,1083,1039,995,952,910,869,828,788,749,710,673,636,600,565,531,497,
	465,433,403,374,345,318,291,266,242,219,197,176,156,137,120,103,88,74,61,50,39,30,
	22,15,10,6,2,1,0,1,2,6,10,15,22,30,39,50,61,74,88,103,120,137,156,176,197,219,242,
	266,291,318,345,374,403,433,465,497,531,565,600,636,673,710,749,788,828,869,910,952,
	995,1039,1083,1127,1172,1218,1264,1311,1358,1406,1453,1502,1550,1599,1648,1698,1747,
	1797,1847,1897,1948,1998,2048

};


uint32_t DualSine12bit[256];


/**
  * @brief  ʹ��DAC��ʱ�ӣ���ʼ��GPIO
  * @param  ��
  * @retval ��
  */
static void DAC_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

  /* ʹ��GPIOAʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
	/* ʹ��DACʱ�� */	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
  /* DAC��GPIO���ã�ģ������ */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	


  /* ����DAC ͨ��1 */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;						//ʹ��TIM2��Ϊ����Դ
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//��ʹ�ò��η�����
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//��ʹ��DAC�������
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* ����DAC ͨ��2 */
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* ʹ��ͨ��1 ��PA4��� */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  /* ʹ��ͨ��2 ��PA5��� */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /* ʹ��DAC��DMA���� */
  DAC_DMACmd(DAC_Channel_2, ENABLE);
}


/**
  * @brief  ����TIM
  * @param  ��
  * @retval ��
  */
static void DAC_TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* ʹ��TIM2ʱ�ӣ�TIM2CLK Ϊ72M */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
  /* TIM2������ʱ������ */
 // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 24;       									//��ʱ���� 20  
  TIM_TimeBaseStructure.TIM_Prescaler = 225;       							//Ԥ��Ƶ������Ƶ 72M / (0+1) = 72M
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//ʱ�ӷ�Ƶϵ��
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* ����TIM2����Դ */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* ʹ��TIM2 */
  TIM_Cmd(TIM2, ENABLE);

}

/**
  * @brief  ����DMA
  * @param  ��
  * @retval ��
  */
static void DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* ʹ��DMA2ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	/* ����DMA2 */
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_Address;					//�������ݵ�ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DualSine12bit ;				//�ڴ����ݵ�ַ DualSine12bit
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//���ݴ��䷽���ڴ�������
  DMA_InitStructure.DMA_BufferSize = 256;																	//�����СΪ32�ֽ�	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//�������ݵ�ַ�̶�	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//�ڴ����ݵ�ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//������������Ϊ��λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//�ڴ���������Ϊ��λ	
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//ѭ��ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//��DMAͨ�����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//���ڴ����ڴ�ģʽ	

  DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	
  /* ʹ��DMA2-14ͨ�� */
  DMA_Cmd(DMA2_Channel4, ENABLE);
}


/**
  * @brief  DAC��ʼ������
  * @param  ��
  * @retval ��
  */
void DAC_Mode_Init(void)
{
	uint32_t Idx = 0;  

	DAC_Config();
	DAC_TIM_Config();
	DAC_DMA_Config();
	
	/* ������Ҳ������ݣ�˫ͨ���Ҷ���*/
  for (Idx = 0; Idx < 256; Idx++)
  {
    DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
  }
}






























