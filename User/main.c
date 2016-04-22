#include "stm32f4xx.h"                  // Device header
#include "MTSP.h"
#include "SSI.h"
#include "timer.h"

int data1 = 10;
int data2 = 100;
void delay(int ms);
int main()
{
	// НЕ ЗАБЫВАЕМ ТАКТИРОВАНИЕ!!!
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	InitSSI(GPIOB, GPIO_Pin_3, GPIO_Pin_4, GPIO_PinSource3, GPIO_PinSource4, SPI1);
	InitTimerForSSI();
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	while(1)
	{		
		//while(SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
    //SPI_I2S_SendData(SPI1, 0xFF);
		//while(SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};
		//data1 = SPI_I2S_ReceiveData(SPI1);
		//data1 = SPI_I2S_ReceiveData(SPI1);
		//while(SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){};
    //SPI_I2S_SendData(SPI1, 0xFF);
		//while(SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){};
		//data2 = SPI_I2S_ReceiveData(SPI1);
		//for(int i = 0; i < 100000; i++)
		//{
		//}
		//SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Reset);

	}

}

void TIM6_DAC_IRQHandler()
{
	data1++;

}

// Обработчик прерывания
//void USART2_IRQHandler(void)
//{
//	
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
//	{
//		parse(USART_ReceiveData(USART2), 0);
//	}
//	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//}

//void USART1_IRQHandler(void)
//{
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		parse(USART_ReceiveData(USART1), 1);
//	}
//	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif
/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/