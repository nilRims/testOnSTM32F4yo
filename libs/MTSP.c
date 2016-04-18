#include "MTSP.h"

// Сделать массив структур, если и мастер и слейв на одном мк, чтобы можно было разные старт и стоп байты задавать
struct MTSP
{
	byte _isSlaveInited;
	byte _isMasterInited;
	byte _slaveAddress;
	byte _startByte;
	byte localMessage[MTSP_MESSAGE_LENGTH];
	byte inputMessage[MTSP_SENSOR_MESSAGE_LENGTH];								      // входной буфер для всех пакетов, кроме пакета с данными о датчиках
	int countData;																				      // вспомогательная переменная для парсинга
	uint32_t usartNumber;
} objMTSP[OBJECTS_COUNT];


void InitMaster(GPIO_TypeDef *GPIOx, uint16_t Tx, uint16_t Rx, byte pinSourceRx, byte pinSourceTx,int USART_IRQn, USART_TypeDef *USARTx, byte startByte, int baudRate, int objectNumber)
{
	objMTSP[objectNumber]._isSlaveInited = FALSE;
	objMTSP[objectNumber]._isMasterInited = TRUE;
	objMTSP[objectNumber]._startByte = startByte;
	objMTSP[objectNumber].countData = 0;
	InitPortsForUSART(GPIOx, Tx, Rx, pinSourceRx, pinSourceTx);
	InitUSART(USARTx, baudRate);
	InitNVIC(USARTx, USART_IRQn);
	objMTSP[objectNumber].usartNumber = (uint32_t) USARTx;
}

void InitSlave(GPIO_TypeDef *GPIOx, uint16_t Tx, uint16_t Rx,byte pinSourceRx, byte pinSourceTx,int USART_IRQn, USART_TypeDef *USARTx, byte startByte, byte slaveAddress, int baudRate, int objectNumber)
{
	objMTSP[objectNumber]._isMasterInited = FALSE;
	objMTSP[objectNumber]._isSlaveInited = TRUE;
	objMTSP[objectNumber]._startByte = startByte;
	objMTSP[objectNumber]._slaveAddress = slaveAddress;
	objMTSP[objectNumber].countData = 0;
	InitPortsForUSART(GPIOx, Tx, Rx, pinSourceRx, pinSourceTx);
	InitUSART(USARTx, baudRate);
	InitNVIC(USARTx, USART_IRQn);
	objMTSP[objectNumber].usartNumber = (uint32_t) USARTx;
}

byte* makeMessage(byte *message, int objectNumber)
{
	objMTSP[objectNumber].localMessage[START_BYTE_OFFSET] = objMTSP[objectNumber]._startByte;
	for(int i = 0; i < 3; i++)
	{
		objMTSP[objectNumber].localMessage[i+1] = message[i];
	}
	objMTSP[objectNumber].localMessage[CRC8_OFFSET] = crc8(objMTSP[objectNumber].localMessage, MTSP_MESSAGE_LENGTH - 1);
	return objMTSP[objectNumber].localMessage;
}

void SendRequest(byte *message, int objectNumber)
{
	if(objMTSP[objectNumber]._isMasterInited == TRUE)
	{
		Send((USART_TypeDef*) objMTSP[objectNumber].usartNumber, makeMessage(message, objectNumber), MTSP_MESSAGE_LENGTH);
	}
}

void parse(byte localByte, int objectNumber)
{
	
	if(objMTSP[objectNumber]._isMasterInited == TRUE)					// Парсер для мастера
	{
		objMTSP[objectNumber].inputMessage[objMTSP[objectNumber].countData] = localByte;
		switch(objMTSP[objectNumber].countData)
		{
			case START_BYTE_OFFSET:
				if(localByte == objMTSP[objectNumber]._startByte) objMTSP[objectNumber].countData++; 
				break;
			case SLAVE_ADDRESS_OFFSET:
				objMTSP[objectNumber].countData++;
				break;
			case COMAND_NUMBER_OFFSET:
				objMTSP[objectNumber].countData++;
				break;
			default:
				if(objMTSP[objectNumber].inputMessage[COMAND_NUMBER_OFFSET] == SENSOR_COMAND)
				{
					if(objMTSP[objectNumber].countData < MTSP_SENSOR_MESSAGE_LENGTH - 1)
					{
						objMTSP[objectNumber].countData++;
						break;
					}
					if(objMTSP[objectNumber].inputMessage[CRC8_SENSOR_OFFSET] == crc8(objMTSP[objectNumber].inputMessage, MTSP_SENSOR_MESSAGE_LENGTH - 1))
					{
						byte localRightMessage[MTSP_SENSOR_MESSAGE_LENGTH - 2];
						for(int i = 0; i<MTSP_SENSOR_MESSAGE_LENGTH - 1;i++)
						{
							localRightMessage[i] = objMTSP[objectNumber].inputMessage[i+1];
						}
						onRightMessage(localRightMessage, TRUE, objectNumber);
						objMTSP[objectNumber].countData = 0;
					}
					else
					{
						for(byte i = 0; i < MTSP_SENSOR_MESSAGE_LENGTH;i++)
						{
							if(objMTSP[objectNumber].inputMessage[i] == objMTSP[objectNumber]._startByte)
							{
								objMTSP[objectNumber].countData = MTSP_SENSOR_MESSAGE_LENGTH - i;
								shiftLeft(i, objectNumber);
								return;
							}
						}
					}
									
				}
				else
				{
					if(objMTSP[objectNumber].countData < MTSP_MESSAGE_LENGTH - 1)
					{
						objMTSP[objectNumber].countData++;
						break;
					}
					if(objMTSP[objectNumber].inputMessage[CRC8_OFFSET] == crc8(objMTSP[objectNumber].inputMessage, MTSP_MESSAGE_LENGTH - 1))
					{
						
						byte localRightMessage[MTSP_MESSAGE_LENGTH - 2];
						for(int i = 0; i<MTSP_MESSAGE_LENGTH - 1;i++)
						{
							localRightMessage[i] = objMTSP[objectNumber].inputMessage[i+1];
						}
						onRightMessage(localRightMessage, FALSE, objectNumber);
						objMTSP[objectNumber].countData = 0;
					}
					else
					{
						for(byte i = 0; i < MTSP_MESSAGE_LENGTH;i++)
						{
							if(objMTSP[objectNumber].inputMessage[i] == objMTSP[objectNumber]._startByte)
							{
								objMTSP[objectNumber].countData = MTSP_MESSAGE_LENGTH - i;
								shiftLeft(i, objectNumber);
								return;
							}
						}
					}
				}
				
				break;
		}
	}
	else
	{
		if(objMTSP[objectNumber]._isSlaveInited == TRUE)		// Парсер на slave
		{
			objMTSP[objectNumber].inputMessage[objMTSP[objectNumber].countData] = localByte;
			switch(objMTSP[objectNumber].countData)
			{
				case START_BYTE_OFFSET:
					if(localByte == objMTSP[objectNumber]._startByte) objMTSP[objectNumber].countData++; 
					break;
				case SLAVE_ADDRESS_OFFSET:
					if(localByte == objMTSP[objectNumber]._slaveAddress) objMTSP[objectNumber].countData++;
					else objMTSP[objectNumber].countData = 0;
					break;
				case COMAND_NUMBER_OFFSET:
					objMTSP[objectNumber].countData++;
					break;
				default:
					if(objMTSP[objectNumber].inputMessage[COMAND_NUMBER_OFFSET] == SENSOR_COMAND)
					{
						// SLAVE если пришла команда на SENSOR
					}
					else
					{
						if(objMTSP[objectNumber].countData < MTSP_MESSAGE_LENGTH - 1)
						{
							objMTSP[objectNumber].countData++;
							break;
						}
						if(objMTSP[objectNumber].inputMessage[CRC8_OFFSET] == crc8(objMTSP[objectNumber].inputMessage, MTSP_MESSAGE_LENGTH - 1))
						{
							byte localRightMessage[MTSP_MESSAGE_LENGTH - 2];
							for(int i = 0; i<MTSP_MESSAGE_LENGTH - 1;i++)
							{
								localRightMessage[i] = objMTSP[objectNumber].inputMessage[i+1];
							}
							onRightMessage(localRightMessage, FALSE, objectNumber);
							// Ответ slave, где вместо dataByte 0x00
							objMTSP[objectNumber].inputMessage[CRC8_OFFSET - 1] = 0x00;
							objMTSP[objectNumber].inputMessage[CRC8_OFFSET] = crc8(objMTSP[objectNumber].inputMessage, MTSP_MESSAGE_LENGTH - 1);
							Send((USART_TypeDef*) objMTSP[objectNumber].usartNumber, objMTSP[objectNumber].inputMessage, MTSP_MESSAGE_LENGTH);							
							objMTSP[objectNumber].countData = 0;
						}
						else
						{
							for(byte i = 0; i < MTSP_MESSAGE_LENGTH;i++)
							{
								if(objMTSP[objectNumber].inputMessage[i] == objMTSP[objectNumber]._startByte)
								{
									objMTSP[objectNumber].countData = MTSP_MESSAGE_LENGTH - i;
									shiftLeft(i, objectNumber);
									return;
								}
							}
						}
					}
					break;
				
			}
		}
	
	}
}

  


void shiftLeft(byte amount, int objectNumber)
{
	for(byte i=0, j=amount; i<MTSP_MESSAGE_LENGTH-amount; i++, j++)
  {
    objMTSP[objectNumber].inputMessage[i] = objMTSP[objectNumber].inputMessage[j];  
  }
}


void onRightMessage(byte *rightMessage, int isDataFromSensor, int objectNumber)
{
	if(objMTSP[objectNumber]._isMasterInited == TRUE)
	{
		if(isDataFromSensor == TRUE)
		{
			// если с датчиков инфа
			byte localBuf[MTSP_MESSAGE_LENGTH - 2] = {0x01, 0x02, 0x02};
			SendRequest(localBuf, objectNumber);
		}
		else
		{
			SendRequest(rightMessage, objectNumber);
		}
	}
	else
	{
		if(objMTSP[objectNumber]._isSlaveInited == TRUE)
		{
			byte localBuf[MTSP_MESSAGE_LENGTH - 2] = {0xdd, 0xdd, 0xdd};
			SendRequest(localBuf, 1);
		
		}
	}
	
}