//**************************************************************************************************
// Filename							MT_STM32F10x_USART.h
// Platform     				STM32F10x
// Author								Aveal (http://microtechnics.ru/)
// Current version  		1.0.0
// Date         				06.03.2013
//**************************************************************************************************

#ifndef MT_STM32F10x_USART_H
#define MT_STM32F10x_USART_H



//**************************************************************************************************
// Includes
//**************************************************************************************************

#include "stm32f10x.h"

// Get RCC functions from Standard Peripheral Library
#include "stm32f10x_rcc.h"

// Get GPIO functions from Standard Peripheral Library
#include "stm32f10x_gpio.h"

// Get USART functions from Standard Peripheral Library
#include "stm32f10x_usart.h"



//**************************************************************************************************
// Declarations and definitions
//**************************************************************************************************

// USART module numbers
#define USART_MODULE1												(1)
#define USART_MODULE2												(2)
#define USART_MODULE3												(3)

// Library configuration
// Select USART number
#define MT_USART_MODULE_IN_USE							(USART_MODULE2)

//Set USART ba  udrate
#define MT_USART_BAUDRATE										(9600)

// Global variables


// MT_USART functions prototypes
extern void MT_USART_Init(void);
extern void MT_USART_SendData(uint8_t *pSendData, uint8_t nNumOfDataToSend);
extern void MT_USART_ReceiveData(uint8_t* pReceivedData, uint8_t nNumOfDataToReceive);

#if (MT_USART_MODULE_NUMBER == USART_MODULE1)
extern void USART1_IRQHandler(void);
#endif // #if (MT_USART_MODULE_NUMBER == USART_MODULE1)

#if (MT_USART_MODULE_NUMBER == USART_MODULE2)
extern void USART2_IRQHandler(void);
#endif // #if (MT_USART_MODULE_NUMBER == USART_MODULE2)

#if (MT_USART_MODULE_NUMBER == USART_MODULE3)
extern void USART3_IRQHandler(void);
#endif // #if (MT_USART_MODULE_NUMBER == USART_MODULE3)



#endif // #ifndef MT_STM32F10x_USART_H

//****************************************** end of file *******************************************
