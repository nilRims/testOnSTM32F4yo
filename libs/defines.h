#include "stm32f4xx_spi.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
typedef uint8_t byte;

#define TRUE 1
#define FALSE 0
#define MESSAGE_LENGTH 5


#define MTSP_MESSAGE_LENGTH 5
#define MTSP_SENSOR_MESSAGE_LENGTH 9

#define START_BYTE_OFFSET 0
#define COMAND_NUMBER_OFFSET 2
#define SLAVE_ADDRESS_OFFSET 1
#define CRC8_OFFSET 4
#define CRC8_SENSOR_OFFSET 8

#define SENSOR_COMAND 0x03

#define ERROR_BYTE 0xEE

#define OBJECTS_COUNT 5