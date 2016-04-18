#include "stm32f4xx.h"                  // Device header
#include "MTSP.h"
#include "SSI.h"

#define F_TIMER 84000000
byte data1,data2;
int data;
void delay(int ms);
int main()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	InitSSI(GPIOA, GPIO_Pin_5, GPIO_Pin_6, GPIO_PinSource5, GPIO_PinSource6, SPI1);
	while(1)
	{
		if(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == SET)
		{
			//data = (int) SPI_ReceiveData(SPI1);
			data1 = SPI_ReceiveData(SPI1);
			//data2 = SPI_ReceiveData(SPI1);
		}
		//SPI_ClearFlag(SPI1, SPI_FLAG_RXNE);
		SPI_ClearITPendingBit(SPI1, SPI_IT_RXNE);
		for(int i = 0; i < 1000000; i++)
		{
			
		}
//		if(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == SET)
//		{
//			//data = (int) SPI_ReceiveData(SPI1);
//			data2 = SPI_ReceiveData(SPI1);
//			//data2 = SPI_ReceiveData(SPI1);
//		}
//		//SPI_ClearFlag(SPI1, SPI_FLAG_RXNE);
//		SPI_ClearITPendingBit(SPI1, SPI_IT_RXNE);
//		
//		for(int i = 0; i < 1000000; i++)
//		{
//			
//		}
	}

}


void delay(int ms)
{
	TIM6->PSC = F_TIMER/(3000) - 1;
	TIM6->ARR = ms*3;
	TIM6->EGR = TIM_EGR_UG;
  TIM6->CR1 = TIM_CR1_CEN|TIM_CR1_OPM;
  while ((TIM6->CR1 & TIM_CR1_CEN)!=0);
}


// Обработчик прерывания
void USART2_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		parse(USART_ReceiveData(USART2), 0);
	}
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}

void USART1_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		parse(USART_ReceiveData(USART1), 1);
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}