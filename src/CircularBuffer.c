/*
 * CircularBuffer.c
 *
 *  Created on: 16 nov. 2022
 *      Author: sebri
 */

#include "CircularBuffer.h"

volatile unsigned int item_in_buffer8b = 0;

int CBuffer8b_write(uint8_t p_data)
{
	if(item_in_buffer8b >= BUFFER_SIZE) return -1;

	circular_buffer8b[buffer8b_head] = p_data;		// mettre data dans le buffer

	buffer8b_head++;								// incrementer pos head
	item_in_buffer8b++;

	if(buffer8b_head >= BUFFER_SIZE){				// si tete = ou > que taille buffer
		buffer8b_head = 0;						// alors retour a 0
	}

	return 0;
}

int CBuffer8b_read(uint8_t* r_data)
{
	if(CBuffer8b_isEmpty()) return -1;				// retourne 0 si buffer est vide

	*r_data = circular_buffer8b[buffer8b_queue];	// mettre valeur a pos head dans r_data

	buffer8b_queue++;								// incrementer pos queue
	item_in_buffer8b--;

	if(buffer8b_queue >= BUFFER_SIZE){			// si tete = ou > que taille buffer
		buffer8b_queue = 0;						// alors retour a 0
	}

	return 0;
}

int CBuffer8b_isEmpty()
{
	return buffer8b_head == buffer8b_queue;
}


volatile unsigned int item_in_buffer16b = 0;

int CBuffer16b_write(uint16_t p_data)
{
	if(item_in_buffer16b >= BUFFER_SIZE) return 0;

	circular_buffer16b[buffer16b_head] = p_data;		// mettre data dans le buffer

	buffer16b_head++;								// incrementer pos head
	item_in_buffer16b++;

	if(buffer16b_head >= BUFFER_SIZE){				// si tete = ou > que taille buffer
		buffer16b_head = 0;						// alors retour a 0
	}

	return 0;
}

int CBuffer16b_read(uint16_t* r_data)
{
	if(CBuffer16b_isEmpty()) return -1;				// retourne 0 si buffer est vide

	*r_data = circular_buffer16b[buffer16b_queue];	// mettre valeur a pos head dans r_data

	buffer16b_queue++;								// incrementer pos queue
	item_in_buffer16b--;

	if(buffer16b_queue >= BUFFER_SIZE){			// si tete = ou > que taille buffer
		buffer16b_queue = 0;						// alors retour a 0
	}

	return 0;
}

int CBuffer16b_isEmpty()
{
	return buffer16b_head == buffer16b_queue;
}

// *******************************************************************
// UART buffer
// *******************************************************************

volatile unsigned int item_in_bufferUART= 0;

int CBufferUART_write(UART_item p_data)
{
	if(item_in_bufferUART>= BUFFER_SIZE) return 0;

	circular_bufferUART[bufferUART_head] = p_data;		// mettre data dans le buffer

	bufferUART_head++;								// incrementer pos head
	item_in_bufferUART++;

	if(bufferUART_head >= BUFFER_SIZE){				// si tete = ou > que taille buffer
		bufferUART_head = 0;						// alors retour a 0
	}

	return 0;
}

int CBufferUART_read(UART_item* r_data)
{
	if(CBufferUART_isEmpty()) return -1;				// retourne 0 si buffer est vide

	*r_data = circular_bufferUART[bufferUART_queue];	// mettre valeur a pos head dans r_data

	bufferUART_queue++;								// incrementer pos queue
	item_in_bufferUART--;

	if(bufferUART_queue >= BUFFER_SIZE){			// si tete = ou > que taille buffer
		bufferUART_queue = 0;						// alors retour a 0
	}

	return 0;
}

int CBufferUART_isEmpty()
{
	return bufferUART_head == bufferUART_queue;
}
