#include "SSI.h"

struct SSI
{
	int timeout;
}objSSI;

void setTimeoutSSI(int milliSec)
{
	objSSI.timeout = milliSec;  
}

void InitSSI(GPIO_TypeDef *GPIOx, uint16_t CLK, uint16_t MISO, byte pinSourceCLK, byte pinSourceMISO, SPI_TypeDef *SPIx)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Pin = CLK | MISO;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &gpio);
	
	GPIO_PinAFConfig(GPIOx, pinSourceCLK, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOx, pinSourceMISO, GPIO_AF_SPI1);
	
	  SPI_I2S_DeInit(SPIx);
		SPI_InitTypeDef spi;
		SPI_StructInit(&spi);
		spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
		spi.SPI_DataSize = SPI_DataSize_16b;
		spi.SPI_Direction = SPI_Direction_2Lines_RxOnly;
		spi.SPI_CPOL = SPI_CPOL_High;
		spi.SPI_CPHA = SPI_CPHA_1Edge;
		spi.SPI_FirstBit = SPI_FirstBit_MSB;
		spi.SPI_Mode = SPI_Mode_Master;
		spi.SPI_NSS = SPI_NSS_Soft;
		SPI_Init(SPIx, &spi);
  	SPI_Cmd(SPIx, ENABLE);
}