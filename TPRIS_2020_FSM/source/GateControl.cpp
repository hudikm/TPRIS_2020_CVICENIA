/*
 * FSM.cpp
 *
 *  Created on: 10. 11. 2020
 *      Author: martin
 */

#include <GateControl.h>

//***************************************************************************
const etl::array<GateControl::transition, 6> GateControl::transitionTable = {
		GateControl::transition(
				States::IDLE,   //Current state
				Events::CMD_OPEN, // Event
				States::OPEN,  // New state
				&GateControl::OnTranToOpen,
				&GateControl::Guard),
		GateControl::transition(
				States::IDLE,   //Current state
				Events::CMD_CLOSE, // Event
				States::CLOSE,  // New state
				&GateControl::OnTranToClose,
				&GateControl::Guard),
		GateControl::transition(
				States::CLOSE,   //Current state
				Events::S1, // Event
				States::IDLE,  // New state
				&GateControl::OnTranToIdle,
				&GateControl::Guard),
		GateControl::transition(
				States::OPEN,   //Current state
				Events::S2, // Event
				States::IDLE,  // New state
				&GateControl::OnTranToIdle,
				&GateControl::Guard),
		GateControl::transition(
				Events::INIT, // Event
				States::CLOSE,  // New state
				&GateControl::OnTranToClose,
				&GateControl::GuardNotS1AndNotS2),
		GateControl::transition(
				Events::INIT, // Event
				States::IDLE,  // New state
				&GateControl::OnTranToIdle,
				&GateControl::GuardS1OrS2)
		 };
//***************************************************************************

const etl::array<GateControl::state, 3> GateControl::stateTable = {
		GateControl::state(
				States::IDLE,
				&GateControl::OnEnterIdle,
				&GateControl::OnExitIdle)
	    ,GateControl::state(
	    		States::OPEN,
				&GateControl::OnEnterOpen,
				&GateControl::OnExitOpen
				)
		,GateControl::state(States::CLOSE,
				&GateControl::OnEnterClose,
				&GateControl::OnExitClose)
};
