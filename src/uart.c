/*
 * uart.c
 *
 *  Created on: 15 nov. 2022
 *      Author: andrea
 */

#include "stm32f4xx.h"
#include <stdlib.h>
#include <string.h>
#include "macros_utiles.h"
#include "uart.h"
#include "CircularBuffer.h"

 void  uart_configure()
{
	  RCC->APB2ENR |= BIT4; // activer la clock pour le uart1
	  RCC->AHB1ENR |= BIT1; // GPIOB enable

	  GPIO_InitTypeDef gpioInitStruct;
	  memset(&gpioInitStruct, 0, sizeof(gpioInitStruct));
	  gpioInitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;	// PB7 : Rx, PB6 : Tx
	  gpioInitStruct.GPIO_Mode = GPIO_Mode_AF;				// alternate function
	  gpioInitStruct.GPIO_Speed = GPIO_Speed_100MHz;		// very high speed
	  gpioInitStruct.GPIO_OType = GPIO_OType_PP;			// push pull
	  gpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;				// pull up
	  GPIO_Init(GPIOB, &gpioInitStruct);

	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);		// bind PB7 to AF - usart1
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);		// bind PB6 to AF - usart1

	  USART_InitTypeDef usartInitStruct;
	  memset(&usartInitStruct, 0, sizeof(usartInitStruct));
	  usartInitStruct.USART_BaudRate = 19200;										// baudrate = 19200
	  usartInitStruct.USART_WordLength = USART_WordLength_8b;						// send 8 bits
	  usartInitStruct.USART_StopBits = USART_StopBits_1;							// stopbit 1
	  usartInitStruct.USART_Parity = USART_Parity_No;								// no parity
	  usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// receive + transmit
	  usartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// no cts / rts
	  USART_Init(USART1, &usartInitStruct);

	  // enable uart interrupt
	  USART1->CR1 |= BIT5;
	  NVIC_EnableIRQ(USART1_IRQn);

	  USART_Cmd(USART1, ENABLE);	// enable usart (UE = 1)

	  //USART1->CR1  |= BIT13; 				// enable usart (UE = 1)
}

 void USART1_IRQHandler()
 {
	 if( !(USART1->SR & USART_SR_RXNE) ) return;

	 char data = (char) USART1->DR;
	 CBuffer8b_write(data);
 }

int uart_sendString(char* p_str)
{
	while(*p_str != '\0'){
		USART_SendData(USART1, (uint16_t)*p_str);
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET );

		p_str++;
	}

	return 0;
}

int uart_sendCommande(uint8_t p_cmd,
		 	 	 	  void* p_data,
					  unsigned int p_size){

	uint64_t* data = malloc(sizeof(uint64_t));
	memcpy(data, p_data, p_size);

	 USART_SendData(USART1, p_cmd);
	 while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET );

	 USART_SendData(USART1, p_size);
	 while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET );

	 uint8_t to_send_byte;
	 for(int i=0; i < p_size; i++){

		 to_send_byte = (*data >> 8*i);	// keep next byte to send

		 USART_SendData(USART1, to_send_byte);
		 while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET );
	 }

	 USART_SendData(USART1, 0xFF);
	 while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET );

	 free(data);
	 return 0;
 }
