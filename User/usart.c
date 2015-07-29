#include "usart.h"

uint8_t RxBuffer[8] = "0";
uint8_t RxCounter = 0;
uint8_t USARTFinFlag = 0;
uint8_t channel = 0;
uint8_t multi = 0;

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	  
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

int fputc(int ch, FILE *f)
{

  USART_SendData(USART1, (uint8_t) ch);

  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)  {}

  return ch;
}

uint8_t USART_Scanf(void)
{
  uint8_t index = 0;


	if(USARTFinFlag == 0) return 0x01;
	else
	{
		//检查数据格式
		for(index=0;index<8;index++) LCD_DispChar(100+STR_WIDTH*index,200,RxBuffer[index],BLACK);
		for (index=0;index<8;index++)
		{
			switch(index)
			{
				case 0:
					if(RxBuffer[index]!='S') return 0x02; break;
				case 1:
					if(RxBuffer[index]!='E') return 0x02; break;
				case 2:
					if(RxBuffer[index]!='T') return 0x02; break;
				case 3: case 5:
					if(RxBuffer[index]!=':') return 0x02; break;
				case 4: case 6: case 7:
					if((RxBuffer[index]<'0')||(RxBuffer[index]>'9')) return 0x02; break;
				//default:
					//return 0x01; break;
			}
		}	
		//检查数据有效性
		channel = RxBuffer[4] - '0';
		multi = (RxBuffer[6] - '0')*10 + (RxBuffer[7] - '0');
		if((channel < 1)||(channel > 2)) return 0x03;
		if((multi < 2)||(multi > 10)) return 0x04;
		
		//清空标志 重启中断
			RxCounter=0;
			USARTFinFlag=0;
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		return 0x00;
	}



}

