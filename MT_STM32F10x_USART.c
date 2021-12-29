//**************************************************************************************************
// Filename							MT_STM32F10x_USART.c
// Platform     				STM32F10x
// Author								Aveal (http://microtechnics.ru/)
// Current version  		1.0.0   
// Date         				06.03.2013
//**************************************************************************************************



//**************************************************************************************************
// Includes
//**************************************************************************************************

#include "MT_STM32F10x_USART.h"



//**************************************************************************************************
// Declarations and definitions
//**************************************************************************************************

// Variables for GPIO and USART configuration
GPIO_InitTypeDef MT_USART_CfgGPIO;
USART_InitTypeDef MT_USART_CfgUSART;

// Ready to transmit and ready to receive flags
_Bool MT_USART_ReadyToExchange;

// Number of bytes to send
uint8_t MT_USART_NumOfDataToSend;

// Number of bytes to receive
uint8_t MT_USART_NumOfDataToReceive;

// Pointer to the data that will be sent
uint8_t *MT_USART_SendDataPtr;

// Pointer to the buffer where received data will be saved
uint8_t *MT_USART_ReceivedDataPtr;

// Number of sended bytes
uint8_t MT_USART_NumOfDataSended;

// Number of received bytes
uint8_t MT_USART_NumOfDataReceived;



//**************************************************************************************************
// Global functions
//**************************************************************************************************



//**************************************************************************************************
// Function      MT_USART_Init()
// Description   USART and GPIO initialization
// Parameters    None
// RetVal	       None
//**************************************************************************************************
void MT_USART_Init()
{
	__enable_irq();
	#if (MT_USART_MODULE_IN_USE == USART_MODULE1)

   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_StructInit(&MT_USART_CfgGPIO);
	MT_USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	MT_USART_CfgGPIO.GPIO_Pin = GPIO_Pin_9;
	MT_USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &MT_USART_CfgGPIO);
	
	MT_USART_CfgGPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	MT_USART_CfgGPIO.GPIO_Pin = GPIO_Pin_10;
	MT_USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &MT_USART_CfgGPIO);
	
	USART_StructInit(&MT_USART_CfgUSART);
	MT_USART_CfgUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	MT_USART_CfgUSART.USART_BaudRate = MT_USART_BAUDRATE;
	USART_Init(USART1, &MT_USART_CfgUSART);	
	
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1, ENABLE);
	
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE1)
	
	#if (MT_USART_MODULE_IN_USE == USART_MODULE2)
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_StructInit(&MT_USART_CfgGPIO);
	MT_USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	MT_USART_CfgGPIO.GPIO_Pin = GPIO_Pin_2;
	MT_USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &MT_USART_CfgGPIO);
	
	MT_USART_CfgGPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	MT_USART_CfgGPIO.GPIO_Pin = GPIO_Pin_3;
	MT_USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &MT_USART_CfgGPIO);
	
	USART_StructInit(&MT_USART_CfgUSART);
	MT_USART_CfgUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	MT_USART_CfgUSART.USART_BaudRate = MT_USART_BAUDRATE;
	USART_Init(USART2, &MT_USART_CfgUSART);	
	
	NVIC_EnableIRQ(USART2_IRQn);
	USART_Cmd(USART2, ENABLE);
	
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE2)
	
	#if (MT_USART_MODULE_IN_USE == USART_MODULE3)
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_StructInit(&MT_USART_CfgGPIO);
	MT_USART_CfgGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	MT_USART_CfgGPIO.GPIO_Pin = GPIO_Pin_10;
	MT_USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &MT_USART_CfgGPIO);
	
	MT_USART_CfgGPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	MT_USART_CfgGPIO.GPIO_Pin = GPIO_Pin_11;
	MT_USART_CfgGPIO.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &MT_USART_CfgGPIO);
	
	USART_StructInit(&MT_USART_CfgUSART);
	MT_USART_CfgUSART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	MT_USART_CfgUSART.USART_BaudRate = MT_USART_BAUDRATE;
	USART_Init(USART3, &MT_USART_CfgUSART);	
	
	NVIC_EnableIRQ(USART3_IRQn);
	USART_Cmd(USART3, ENABLE);
	
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE3)
	
} // End of function MT_USART_Init()



//**************************************************************************************************
// Function      MT_USART_SendData()
// Description   Sends data to the USART peripheral
// Parameters    pSendData - pointer to the data buffer
//							 nNumOfDataToSend - number of bytes to send
// RetVal	       None
//**************************************************************************************************
void MT_USART_SendData(uint8_t *pSendData, uint8_t nNumOfDataToSend)
{
	MT_USART_SendDataPtr = pSendData;
	MT_USART_NumOfDataToSend = nNumOfDataToSend;
	MT_USART_NumOfDataSended = 0;
	MT_USART_ReadyToExchange = 0;
	
	#if (MT_USART_MODULE_IN_USE == USART_MODULE1)
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE1)
	
	#if (MT_USART_MODULE_IN_USE == USART_MODULE2)
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE2)
	
	#if (MT_USART_MODULE_IN_USE == USART_MODULE3)
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE3)
} // end of MT_USART_SendData()



//**************************************************************************************************
// Function      MT_USART_ReceiveData()
// Description   Receives data from the USART peripheral
// Parameters    pReceivedData - pointer to the data buffer
//							 nNumOfDataToReceive - number of bytes to receive
// RetVal	       None
//**************************************************************************************************
void MT_USART_ReceiveData(uint8_t* pReceivedData, uint8_t nNumOfDataToReceive)
{
	MT_USART_ReceivedDataPtr = pReceivedData;
	MT_USART_NumOfDataToReceive = nNumOfDataToReceive;
	MT_USART_NumOfDataReceived = 0;
	MT_USART_ReadyToExchange = 0;

	#if (MT_USART_MODULE_IN_USE == USART_MODULE1)
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE1)
	
	#if (MT_USART_MODULE_IN_USE == USART_MODULE2)
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE2)
	
	#if (MT_USART_MODULE_IN_USE == USART_MODULE3)
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE3)
} // end of MT_USART_ReceiveData()



//**************************************************************************************************
// Local functions
//**************************************************************************************************

// None.



//**************************************************************************************************
// Interrupts
//**************************************************************************************************



//**************************************************************************************************
// Function      USARTx_IRQHandler()
// Description   Interrupt services for USART peripheral
// Parameters    None
// RetVal	       None
//**************************************************************************************************
#if (MT_USART_MODULE_IN_USE == USART_MODULE1)
void USART1_IRQHandler()
{
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		*MT_USART_ReceivedDataPtr = USART_ReceiveData(USART1);	
		MT_USART_ReceivedDataPtr++;	
		MT_USART_NumOfDataReceived++;
		if (MT_USART_NumOfDataReceived == MT_USART_NumOfDataToReceive)
		{
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
			MT_USART_ReadyToExchange = 1;
		}
	}
	
	if (USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_SendData(USART1, (uint8_t)*MT_USART_SendDataPtr);	
		MT_USART_SendDataPtr++;
		MT_USART_NumOfDataSended++;
		if (MT_USART_NumOfDataSended == MT_USART_NumOfDataToSend)
		{
			USART_ITConfig(USART1, USART_IT_TC, DISABLE);
			MT_USART_ReadyToExchange = 1;
		}		
	}
	
} // End of USART1_IRQHandler() 
#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE1)

#if (MT_USART_MODULE_IN_USE == USART_MODULE2)
void USART2_IRQHandler()
{
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		*MT_USART_ReceivedDataPtr = USART_ReceiveData(USART2);	
		MT_USART_ReceivedDataPtr++;	
		MT_USART_NumOfDataReceived++;
		if (MT_USART_NumOfDataReceived == MT_USART_NumOfDataToReceive)
		{
			USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
			MT_USART_ReadyToExchange = 1;
		}
	}
	
	if (USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		USART_SendData(USART2, (uint8_t)*MT_USART_SendDataPtr);	
		MT_USART_SendDataPtr++;
		MT_USART_NumOfDataSended++;
		if (MT_USART_NumOfDataSended == MT_USART_NumOfDataToSend)
		{
			USART_ITConfig(USART2, USART_IT_TC, DISABLE);
	  	MT_USART_ReadyToExchange = 1;
		}		
	}
	
} // End of USART2_IRQHandler() 
#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE2)

#if (MT_USART_MODULE_IN_USE == USART_MODULE3)
void USART3_IRQHandler()
{
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{	
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		*MT_USART_ReceivedDataPtr = USART_ReceiveData(USART3);	
		MT_USART_ReceivedDataPtr++;	
		MT_USART_NumOfDataReceived++;
		if (MT_USART_NumOfDataReceived == MT_USART_NumOfDataToReceive)
		{
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
			MT_USART_ReadyToExchange = 1;
		}
	}
	
	if (USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		USART_SendData(USART3, (uint8_t)*MT_USART_SendDataPtr);	
		MT_USART_SendDataPtr++;
		MT_USART_NumOfDataSended++;
		if (MT_USART_NumOfDataSended == MT_USART_NumOfDataToSend)
		{
			USART_ITConfig(USART3, USART_IT_TC, DISABLE);
			MT_USART_ReadyToExchange = 1;
		}		
	}
	
} // End of USART3_IRQHandler() 
#endif // #if (MT_USART_MODULE_IN_USE == USART_MODULE3)



//****************************************** end of file *******************************************
