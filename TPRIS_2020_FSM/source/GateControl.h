/*
 * FSM.h
 *
 *  Created on: 10. 11. 2020
 *      Author: martin
 */

#ifndef GATECONTROL_H_
#define GATECONTROL_H_
#include <fsl_debug_console.h>
#include "etl/state_chart.h"
#include "etl/array.h"

#include <stdio.h>

struct Events {
	enum {
		S1, S2, CMD_CLOSE, CMD_OPEN, TIME_OUT, INIT
	};
};

const char *const EventName[] = { "S1", "S2", "CMD_CLOSE", "CMD_OPEN",
		"TIME_OUT", "INIT" };

//***************************************************************************
// States
//***************************************************************************
struct States {
	enum {
		CLOSE, OPEN, IDLE, ERROR
	};
};

const char *const StateName[] = { "CLOSE", "OPEN", "IDLE", "ERROR" };

//***********************************
// The Gate control FSM.
//***********************************
class GateControl: public etl::state_chart<GateControl> {
public:
	GateControl() :
			etl::state_chart<GateControl>(*this, transitionTable.data(),
					transitionTable.data() + transitionTable.size(),
					stateTable.data(), stateTable.data() + stateTable.size(),
					States::IDLE) {
	}
//***********************************
	void OnTranToClose() {
		PRINTF("%s\n", __func__);

	}
//***********************************
	void OnTranToIdle() {
		PRINTF("%s\n", __func__);

	}
//***********************************
	void OnTranToOpen() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	void OnTranToError() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	void OnEnterIdle() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	void OnExitIdle() {
		PRINTF("%s\n", __func__);
	}
//**********************************
	void OnEnterOpen() {
		PRINTF("%s\n", __func__);
	}
//**********************************
	void OnExitOpen() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	void OnEnterClose() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	void OnExitClose() {
		PRINTF("%s\n", __func__);
	}
//***********************************
	bool Guard() {
		PRINTF("%s\n", __func__);
		return guard;
	}
	//***********************************
	bool GuardNotS1AndNotS2() {
		PRINTF("%s\n", __func__);

		return !S1&&!S2;
	}
	//***********************************
	bool GuardS1OrS2() {
		PRINTF("%s\n", __func__);
		return S1 || S2 ;
	}
//***********************************
	bool NotGuard() {
		PRINTF("%s\n", __func__);
		return !guard;
	}
//***********************************
	void Null() {
//		std::cout << "Action: Null\n";
	}
	bool guard;
	bool S1 = false;
	bool S2 = false;
	static const etl::array<GateControl::transition, 6> transitionTable;
	static const etl::array<GateControl::state, 3> stateTable;
};
#endif /* GATECONTROL_H_ */
