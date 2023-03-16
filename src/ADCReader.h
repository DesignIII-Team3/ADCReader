/*
 * ADCReader.h
 *
 *  Created on: 12 févr. 2023
 *      Author: sebri
 */

#ifndef ADCREADER_H_
#define ADCREADER_H_

#include <stdint.h>

#define NB_MESURE 100

volatile uint16_t data[NB_MESURE];
volatile uint16_t* data_head;

void ADC_configure();

void ADC_startConversion();

#endif /* ADCREADER_H_ */
