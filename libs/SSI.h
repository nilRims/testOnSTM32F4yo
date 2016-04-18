#include "defines.h"

void setTimeoutSSI(int milliSec);

void InitSSI(GPIO_TypeDef *GPIOx, uint16_t CLK, uint16_t MISO, byte pinSourceCLK, byte pinSourceMISO, SPI_TypeDef *SPIx);