/*
 * CircularBuffer.h
 *
 *  Created on: 16 nov. 2022
 *      Author: sebri
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <stdint.h>

#define BUFFER_SIZE 20

uint8_t circular_buffer8b[BUFFER_SIZE];
volatile unsigned int buffer8b_head;
volatile unsigned int buffer8b_queue;

volatile unsigned int item_in_buffer8b;

/**
 * Ecrit une donnee dans le buffer circulaire
 * et retourne 1 en cas de succes
 */
int CBuffer8b_write(uint8_t p_data);

/**
 * Lit la donnee a la tete du buffer et met la valeur
 * dans le pointeur de retour
 * retourne 1 en cas de succes
 */
int CBuffer8b_read(uint8_t* r_data);

/**
 * REtourne 1 si le buffer est vide
 */
int CBuffer8b_isEmpty();


uint16_t circular_buffer16b[BUFFER_SIZE];
volatile unsigned int buffer16b_head;
volatile unsigned int buffer16b_queue;

volatile unsigned int item_in_buffer16b;

/**
 * Ecrit une donnee dans le buffer circulaire
 * et retourne 1 en cas de succes
 */
int CBuffer16b_write(uint16_t p_data);

/**
 * Lit la donnee a la tete du buffer et met la valeur
 * dans le pointeur de retour
 * retourne 1 en cas de succes
 */
int CBuffer16b_read(uint16_t* r_data);

/****************
 * circular buffer pour uart
 */

enum type_size {
	type_int 		= sizeof(int),
	type_uint8_t	= sizeof(uint8_t),
	type_uint16_t	= sizeof(uint16_t),
	type_uint32_t	= sizeof(uint32_t),
	type_uint64_t	= sizeof(uint64_t),
	type_char		= sizeof(char),
	type_float		= sizeof(float),
	type_double		= sizeof(double)
};

typedef struct {
	uint8_t cmd;
	uint64_t data;
	enum type_size size;
}UART_item;

/**
 * REtourne 1 si le buffer est vide
 */
int CBufferUART_isEmpty();

UART_item circular_bufferUART[BUFFER_SIZE];
volatile unsigned int bufferUART_head;
volatile unsigned int bufferUART_queue;

volatile unsigned int item_in_bufferUART;

/**
 * Ecrit une donnee dans le buffer circulaire
 * et retourne 1 en cas de succes
 */
int CBufferUART_write(UART_item p_data);

/**
 * Lit la donnee a la tete du buffer et met la valeur
 * dans le pointeur de retour
 * retourne 1 en cas de succes
 */
int CBufferUART_read(UART_item* r_data);

/**
 * REtourne 1 si le buffer est vide
 */
int CBufferUART_isEmpty();

#endif /* CIRCULARBUFFER_H_ */
