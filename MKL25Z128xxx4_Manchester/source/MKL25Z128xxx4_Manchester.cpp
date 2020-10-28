/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MKL25Z128xxx4_Manchester.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

// Project includes
#include "fsl_pit.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

enum class STATE {
	START_BIT_A, START_BIT_B, RX_INT, TIMER_INT

};
#define LOGIC_ANALYZER_DEBUG 0

#define _3_4OFBIT  68U //19200 //104 * 0.75 //22U 57600

volatile STATE next_state = STATE::START_BIT_A;
volatile uint8_t bit_index = 0;
volatile uint8_t inputByte = 0;
volatile bool newByte = false;
volatile bool newEvent = false;

#pragma GCC push_options
#pragma GCC optimize ("O3")

extern "C" void PIT_IRQHandler() {
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	PIT_StopTimer(PIT, kPIT_Chnl_0);
	newEvent = true;
#if LOGIC_ANALYZER_DEBUG
	GPIO_SetPinsOutput(BOARD_INITPINS_OUT_PIN_GPIO,
	BOARD_INITPINS_OUT_PIN_GPIO_PIN_MASK);
	__asm volatile ("nop");
	GPIO_ClearPinsOutput(BOARD_INITPINS_OUT_PIN_GPIO,
	BOARD_INITPINS_OUT_PIN_GPIO_PIN_MASK);
#endif

}

extern "C" void PORTA_IRQHandler() {
	PORT_ClearPinsInterruptFlags(BOARD_INITPINS_RX_PIN_PORT,
	BOARD_INITPINS_RX_PIN_PIN_MASK);
	newEvent = true;
#if LOGIC_ANALYZER_DEBUG
	GPIO_SetPinsOutput(BOARD_INITPINS_OUT2_PIN_GPIO,
	BOARD_INITPINS_OUT2_PIN_GPIO_PIN_MASK);
	__asm volatile ("nop");
	GPIO_ClearPinsOutput(BOARD_INITPINS_OUT2_PIN_GPIO,
	BOARD_INITPINS_OUT2_PIN_GPIO_PIN_MASK);
#endif

}
#pragma GCC pop_options

int main(void) {
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif

	/* Structure of initialize PIT */
	pit_config_t pitConfig;

	PIT_GetDefaultConfig(&pitConfig);

	/* Init pit module */
	PIT_Init(PIT, &pitConfig);

	/* Set timer period for channel 0 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0,
			USEC_TO_COUNT(_3_4OFBIT, CLOCK_GetBusClkFreq()));

	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	PORT_SetPinInterruptConfig(BOARD_INITPINS_RX_PIN_PORT,
	BOARD_INITPINS_RX_PIN_PIN, kPORT_InterruptFallingEdge);

	EnableIRQ(PIT_IRQn);
	EnableIRQ(PORTA_IRQn);

	/* Enter an infinite loop, just incrementing a counter. */
	while (1) {

		while (!newEvent) {
			__asm volatile ("nop");
		}

		newEvent = false;

		switch (next_state) {
		case STATE::START_BIT_A:
			inputByte = 0;
			bit_index = 0;
			next_state = STATE::TIMER_INT;
			PIT_StartTimer(PIT, kPIT_Chnl_0);
			break;
		case STATE::START_BIT_B:
			next_state = STATE::TIMER_INT;
			PIT_StartTimer(PIT, kPIT_Chnl_0);
			break;
		case STATE::RX_INT:
			if (bit_index == 4) {
				next_state = STATE::START_BIT_B;
				PORT_SetPinInterruptConfig(BOARD_INITPINS_RX_PIN_PORT,
				BOARD_INITPINS_RX_PIN_PIN, kPORT_InterruptFallingEdge);
			} else if (bit_index == 8) {
				/*
				 * inputByte obsahuje novy bajt
				 * */
				PRINTF("%c", inputByte);
				next_state = STATE::START_BIT_A;
				PORT_SetPinInterruptConfig(BOARD_INITPINS_RX_PIN_PORT,
				BOARD_INITPINS_RX_PIN_PIN, kPORT_InterruptFallingEdge);

			} else {
				PORT_SetPinInterruptConfig(BOARD_INITPINS_RX_PIN_PORT,
				BOARD_INITPINS_RX_PIN_PIN, kPORT_InterruptOrDMADisabled);
				next_state = STATE::TIMER_INT;
				PIT_StartTimer(PIT, kPIT_Chnl_0);
			}

			break;
		case STATE::TIMER_INT:
			/* Ulozenie noveho bitu */
		{
			auto bit = GPIO_ReadPinInput(BOARD_INITPINS_RX_PIN_GPIO,
			BOARD_INITPINS_RX_PIN_PIN);
			inputByte |= (bit << bit_index);
			bit_index++;
		}

			next_state = STATE::RX_INT;
			/* Interrupt configuration on PORTA1 (pin 27): Interrupt on either edge */
			PORT_SetPinInterruptConfig(BOARD_INITPINS_RX_PIN_PORT,
			BOARD_INITPINS_RX_PIN_PIN, kPORT_InterruptEitherEdge);
			break;


		default:
			next_state = STATE::START_BIT_A;
			break;
		}

	}
	return 0;
}

