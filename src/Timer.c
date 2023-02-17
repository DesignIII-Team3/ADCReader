/*
 * Timer.c
 *
 *  Created on: 13 févr. 2023
 *      Author: sebri
 */
#include "Timer.h"
#include "stm32f4xx_tim.h"
#include "macros_utiles.h"

#include "ADCReader.h"

void Timer_configure()
{
	/*
	 * f_apb1 = 53.76 MHz
	 * freq_tim = f_APB1 / [(PSC+1)(ARR+1)]
	 * <=> PSC = (f_APB1 / [freq_tim(ARR+1)] - 1
	 */

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// Seulement overflow/underflow genere interruption
	TIM6->CR1 |= TIM_CR1_URS;
	TIM6->CR1 |= TIM_CR1_CEN;

	TIM6->DIER |= TIM_DIER_UIE;

	TIM6->PSC = 2687;
	TIM6->ARR = 9999;

	// reinitialise timer et met a jour registre
	TIM6->EGR |= TIM_EGR_UG;


	/*
	TIM_TimeBaseInitTypeDef timer_initStruct;
	timer_initStruct.TIM_Prescaler = 2687;
	timer_initStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timer_initStruct.TIM_Period = 9999;						// val auto-reload register
	timer_initStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	timer_initStruct.TIM_RepetitionCounter = ENABLE;				// N.A pour TIM6
	TIM_TimeBaseInit(TIM6, &timer_initStruct);
	*/

	NVIC_EnableIRQ(TIM6_DAC_IRQn);							// active les interruption du timer

	TIM_Cmd(TIM6, ENABLE);
}

void TIM6_DAC_IRQHandler()
{
	TIM6->SR &= ~TIM_SR_UIF;

	ADC_startConversion();
}
