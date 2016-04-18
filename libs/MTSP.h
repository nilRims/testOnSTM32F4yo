#include "IProtocol.h"
/*
	MTSP - Master-To-Slave протокол;
	Пример посылки:
	[<старт байт> <адрес slave> <номер команды> <информационный байт> <проверочная сумма>];
	[0xFF 0x01 0x02 0xBB 0xCA], подсчёт crc8 можно производить с помощью CRCCalculator;
	Протокол разработан : Акулов Олег, Надежин Михаил;
*/



/*
	Функция InitMaster - служит для инициализации мастера сети;
	Аргументы:
		GPIO_TypeDef *GPIOx - указатель на структуру с характеристиками GPIO, пример GPIOA, GPIOB и т.д.(SPL);
		uint16_t Tx - номер пина, на котором Tx, пример GPIO_Pin_10;
		uint16_t Rx - номер пина, на котором Rx, пример GPIO_Pin_9;
		byte pinSourceRx - пин, который надо настроить как альтернативную функцию (RX), пример GPIO_PinSource10 или 0x0A;
		byte pinSourceTx - пин, который надо настроить как альтернативную функцию (RX), пример GPIO_PinSource9 или 0x09;
		int USART_IRQn - enum из SPL, который соответствует прерыванию по заданному USART, например USART1_IRQn;
		USART_TypeDef* USARTx - указатель на структуру c характеристиками USART, пример USART1, USART2 и т.д.(SPL);
		byte startByte - стартовый байт, использующийся в протоколе;
		int baudRate - скорость передачи данных;
		int objectNumber - номер объекта, использующийся для инициализации структуры, каждый UART должен быть под разным номером,
										максимальное количество USART на одном мк, использующих данный протокол - 5;
*/
void InitMaster(GPIO_TypeDef *GPIOx, uint16_t Tx, uint16_t Rx, byte pinSourceRx, byte pinSourceTx,int USART_IRQn, USART_TypeDef *USARTx, byte startByte, int baudRate, int objectNumber);


/*
	Функция InitSlave - служит для инициализации slave сети;
	Аргументы:
		GPIO_TypeDef *GPIOx - указатель на структуру с характеристиками GPIO, пример GPIOA, GPIOB и т.д.(SPL);
		uint16_t Tx - номер пина, на котором Tx, пример GPIO_Pin_10;
		uint16_t Rx - номер пина, на котором Rx, пример GPIO_Pin_9;
		byte pinSourceRx - пин, который надо настроить как альтернативную функцию (RX), пример GPIO_PinSource10 или 0x0A;
		byte pinSourceTx - пин, который надо настроить как альтернативную функцию (RX), пример GPIO_PinSource9 или 0x09;
		int USART_IRQn - enum из SPL, который соответствует прерыванию по заданному USART, например USART1_IRQn;
		USART_TypeDef* USARTx - указатель на структуру c характеристиками USART, пример USART1, USART2 и т.д.(SPL);
		byte startByte - стартовый байт, использующийся в протоколе;
		byte slaveAddress - адрес slave, мк принимает запросы, только с этим адрессом;
		int baudRate - скорость передачи данных;
		int objectNumber - номер объекта, использующийся для инициализации структуры, каждый UART должен быть под разным номером,
										максимальное количество USART на одном мк, использующих данный протокол - 5;
*/
void InitSlave(GPIO_TypeDef *GPIOx, uint16_t Tx, uint16_t Rx,byte pinSourceRx, byte pinSourceTx,int USART_IRQn, USART_TypeDef *USARTx, byte startByte, byte slaveAddress, int baudRate, int objectNumber);


/*
	Функция SendRequest - служит для отправки запроса, может быть вызвана только мастером сети;
	Аргументы:
		byte *message - указатель на массив, который надо передать, без startByte и crc8;
		int objectNumber - номер объекта, который будет использован для передачи;	
*/
void SendRequest(byte *message, int objectNumber);

void parse(byte localByte, int objectNumber);
	
void shiftLeft(byte amount, int objectNumber);

void onRightMessage(byte *rightMessage, int isDataRightFromSensor, int objectNumber);

byte* makeMessage(byte *message, int objectNumber);