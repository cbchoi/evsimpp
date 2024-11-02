#pragma once

#include <string>
#include <iostream>

#include "atomic_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "message_delivery.hpp"

using namespace evsim;

class CPEG :public CAtomicModel
{
public:
	CPEG(std::string name) :evsim::CAtomicModel(name), elem_count(0)
	{
		REGISTER_INPUT_PORT(input);
		REGISTER_OUTPUT_PORT(output);

		//SET_INIT_STATE(IDLE3);
	}

	virtual void external_transition(const Port& port, const MessageDelivery& msg)
	{
		
	}

	virtual void internal_transition()
	{
		// TODO: Handle Dynamic Generation of States
		//if (CUR_STATE == IDLE3)
		//{
		//	CUR_STATE = IDLE3;
		//}
	};

	virtual void output_function(evsim::MessageDelivery& msg)
	{
		elem_count++;
	};

public:
	//DECLARE_STATE(IDLE3, 1);

	DECLARE_INPUT_PORT(input);
	DECLARE_OUTPUT_PORT(output);

	int elem_count;
};
