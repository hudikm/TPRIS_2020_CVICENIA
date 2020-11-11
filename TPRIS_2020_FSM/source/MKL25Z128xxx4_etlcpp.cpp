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
 * @file    MKL25Z128xxx4_etlcpp.cpp
 * @brief   Application entry point.
 */

#include <board.h>
#include <clock_config.h>
#include <etl/state_chart.h>
#include <fsl_debug_console.h>
#include <GateControl.h>
#include <peripherals.h>
#include <pin_mux.h>
#include "fsl_lpsci.h"

/* TODO: insert other definitions and declarations here. */

int main(void) {

	GateControl gateControl { };

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	/* Inicializacia UART0 (LPSCI)*/
	CLOCK_SetLpsci0Clock(0x1U); // Zapnutie hodin tu alebo cez GUI clock wizzard

	lpsci_config_t user_config;
	LPSCI_GetDefaultConfig(&user_config);
	user_config.baudRate_Bps = 19200U;
	LPSCI_Init(UART0, &user_config, CLOCK_GetPllFllSelClkFreq());
	LPSCI_EnableRx(UART0, true);
	LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);
	EnableIRQ(UART0_IRQn);

	PRINTF("Etlcpp FSM\n");
	gateControl.start(false);
	gateControl.guard = true;
	gateControl.S1 = true;
	gateControl.S2 = false;
	gateControl.process_event(Events::INIT);

	gateControl.process_event(Events::S1);

	while (1) {
		if (newChar_flag) {
			newChar_flag = false;
			switch (newChar) {
			case 'c':
				gateControl.process_event(Events::CMD_CLOSE);
				break;
			case 'o':
				gateControl.process_event(Events::CMD_OPEN);
				break;
			case '1':
				gateControl.process_event(Events::S1);
				break;
			case '2':
				gateControl.process_event(Events::S2);
				break;
			}
		}

	}

	return 0;
}
