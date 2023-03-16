/*
 * ADCReader.c
 *
 *  Created on: 12 févr. 2023
 *      Author: sebri
 */

#include "ADCReader.h"
#include "stm32f4xx_adc.h"
#include <string.h> /* memset */
#include "CircularBuffer.h"
#include "macros_utiles.h"
#include "uart.h"
#include <stdlib.h>
#include "Filter.h"

int compare( const void* a, const void* b);

uint16_t mesures_courant[NB_MEASURE] = {0};
uint16_t* ptr_mesure_courant = mesures_courant;

void ADC_configure()
{
	// Clock configure
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1| RCC_APB2Periph_ADC2, ENABLE);

	// GPIO configure
	GPIO_InitTypeDef GPIO_initStruct;
	GPIO_initStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;	// GPIOC 4 et 5
	GPIO_initStruct.GPIO_Mode = GPIO_Mode_AN;			// mode analogique
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_100MHz; 	// very high speed
	GPIO_initStruct.GPIO_OType = GPIO_OType_PP;			// mode push pull
	GPIO_initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		// No pull-up, no pull down
	GPIO_Init(GPIOC, &GPIO_initStruct);

	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	memset(&ADC_CommonInitStruct, 0, sizeof(ADC_CommonInitStruct));
	ADC_CommonInitStruct.ADC_Mode = ADC_DualMode_RegSimult_InjecSimult;			// on souhiate faire les mesures sur l'ADC1 et 2 en meme temps
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		// pas de DMA
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;					// divise la clk /2
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	// 5 cycles entre 2 mesures
	ADC_CommonInit(&ADC_CommonInitStruct);

	// ADC configure
	ADC_InitTypeDef ADC_initStruct;
	memset(&ADC_initStruct, 0, sizeof(ADC_initStruct));							// init a 0 de ADC_initStruct
	ADC_initStruct.ADC_Resolution = ADC_Resolution_12b;							// valeur sur 12bits
	ADC_initStruct.ADC_ScanConvMode = DISABLE;									// lecture sur 1 seul canal
	ADC_initStruct.ADC_ContinuousConvMode = DISABLE;							// pas de lecture en continue
	ADC_initStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 	// pas de trigger pour debut de la lecture
	ADC_initStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;//ADC_ExternalTrigConv_T1_CC1;			// trigger par le canal 1 du timer 1
	ADC_initStruct.ADC_DataAlign = ADC_DataAlign_Right;							// data alignees a droite
	ADC_initStruct.ADC_NbrOfConversion = 1;										// 1conversion / 16 dispos
	ADC_Init(ADC1, &ADC_initStruct);
	ADC_Init(ADC2, &ADC_initStruct);

	// liaison ADC - GPIO
	ADC_RegularChannelConfig(ADC1,						// mesure tension
							 ADC_Channel_15,			// ADC1 channel 15 = PC5
							 1,
							 ADC_SampleTime_56Cycles);

	// liaison ADC - GPIO
	ADC_RegularChannelConfig(ADC2,						// mesure courant
							 ADC_Channel_14,			// ADC1 channel 14 = PC4
							 1,
							 ADC_SampleTime_56Cycles);

	//interruption a la fin de la conversion
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);

	ADC_Cmd(ADC1, ENABLE);	// activer ADC1
	ADC_Cmd(ADC2, ENABLE);	// activer ADC2
}

void ADC_startConversion()
{
	ADC_SoftwareStartConv(ADC1);	// demarrage de la conversion pour la prise de mesure sur les deux ADCs
}

volatile uint16_t data[NB_MESURE] = {0};
volatile uint16_t* data_head = data;

void ADC_IRQHandler(void)
{

	if(data_head > &data[NB_MESURE-1]){
		ADC_Cmd(ADC1, DISABLE);
		ADC_Cmd(ADC2, DISABLE);

		char* test;
		sprintf(test, "head = %i, end = %i", data_head, &data[NB_MESURE-1]);
		uart_sendString(test);

		return;	// ignorer si toutes es mesures sont faites
	}

	GPIOD->ODR ^= (1 << 12);
	*data_head = (uint16_t)ADC_GetConversionValue(ADC2);
	data_head++;

	ADC_GetConversionValue(ADC1);


	/*
	static unsigned int count = 0;											// fait la moyenne sur NB_MEASURE mesures
	static UART_item uart_data_tension = {CMD_VMEASURE, 0, type_uint16_t};	// pour envoyer mesures de tension
	static UART_item uart_data_courant = {CMD_AMEASURE, 0, type_uint16_t};	// pour envoyer mesures de courant

	uart_data_tension.data += (uint64_t)ADC_GetConversionValue(ADC1);	// recuperer mesure tension et sommer pour la moyenne
	uart_data_courant.data += (uint64_t)ADC_GetConversionValue(ADC2);	// recuperer mesure de courant et sommer pour la moyenne
	//uint16_t courant = (uint64_t)ADC_GetConversionValue(ADC2);
	//mesures_courant[count] = courant;

	count++; // incrementer compteur pour la moyenne

	// si compteur atteint le nombre de mesures desiré pour la moyenne
	// alors envoyer les données
	if(count == NB_MEASURE){
		uart_data_tension.data = (int)(uart_data_tension.data / NB_MEASURE);	// calcul de la moyenne de la tension
		uart_data_courant.data = (int)(uart_data_courant.data / NB_MEASURE);	// calcul de la moyenne du courant


		//uart_data_courant.data = (uint64_t)mesures_courant[MEDIANE];

		CBufferUART_write(uart_data_tension);	// donnees mise dans le buffer pour envoie
		CBufferUART_write(uart_data_courant);	// donnees mise dans le buffer pour envoie

		// remise des compteurs a 0 pour les mesures
		uart_data_tension.data = 0;
		uart_data_courant.data = 0;

		count = 0;	// retour du compteur a 0 pour reprendre de nouvelles mesures
	}
	*/
}
