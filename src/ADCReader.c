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

void ADC_configure()
{
	// Clock configure
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//ADC3 is connected to the APB2 peripheral bus

	// GPIO configure
	GPIO_InitTypeDef GPIO_initStruct;
	GPIO_initStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;	// GPIOC 4 et 5
	GPIO_initStruct.GPIO_Mode = GPIO_Mode_AN;			// mode analogique
	GPIO_initStruct.GPIO_Speed = GPIO_Speed_100MHz; 	// very high speed
	GPIO_initStruct.GPIO_OType = GPIO_OType_PP;			// mode push pull
	GPIO_initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;		// No pull-up, no pull down
	GPIO_Init(GPIOC, &GPIO_initStruct);

	// ADC configure
	ADC_InitTypeDef ADC_initStruct;
	memset(&ADC_initStruct, 0, sizeof(ADC_initStruct));							// init a 0 de ADC_initStruct
	ADC_initStruct.ADC_Resolution = ADC_Resolution_12b;							// valeur sur 12bits
	ADC_initStruct.ADC_ScanConvMode = ENABLE;									// lecture sur 2 canaux
	ADC_initStruct.ADC_ContinuousConvMode = DISABLE;							// pas de lecture en continue
	ADC_initStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 	// pas de trigger pour debut de la lecture
	ADC_initStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;		// pas de signal de trigger
	ADC_initStruct.ADC_DataAlign = ADC_DataAlign_Right;							// data alignees a droite
	ADC_initStruct.ADC_NbrOfConversion = 2;										// 1conversion / 16 dispos
	ADC_Init(ADC1, &ADC_initStruct);

	// liaison ADC - GPIO
	ADC_RegularChannelConfig(ADC1,
							 ADC_Channel_15,	// ADC1 channel 15 = PC5
							 1,
							 ADC_SampleTime_56Cycles);

	// liaison ADC - GPIO
	ADC_RegularChannelConfig(ADC1,
							 ADC_Channel_14,	// ADC1 channel 14 = PC4
							 2,
							 ADC_SampleTime_56Cycles);

	//interruption a la fin de la conversion
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);

	ADC1->CR2 |= BIT10; // EOC set apres chaque conversion de la sequence

	ADC_Cmd(ADC1, ENABLE);	// activer ADC1
}

void ADC_startConversion()
{
	ADC_SoftwareStartConv(ADC1);

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	uint16_t adcValueChannel14 = ADC_GetConversionValue(ADC1);

	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	uint16_t adcValueChannel15 = ADC_GetConversionValue(ADC1);

	printf("");

}

void ADC_startContinuousConversion()
{
	// TODO : modifier reg pour mettre la conversion en continue

	// TODO : lancer la conversion en continu
}

void ADC_stopContinuousConversion()
{
	// TOD : desactiver conversion en continu
}

void ADC_IRQHandler(void)
{
	// TODO : traitement
	uint16_t conversion_value = ADC_GetConversionValue(ADC1);
	CBuffer16b_write(conversion_value);
}
