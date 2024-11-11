#pragma once

#include <string>
#include <iostream>

#include "atomic_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
//#include "message_deliverer.hpp"

using namespace evsim;

class CProcessor :public CAtomicModel
{
public:
	CProcessor(std::string name) :evsim::CAtomicModel(name), elem_count(0)
	{
		REGISTER_INPUT_PORT(input);

		SET_INIT_STATE(IDLE);
	}

	virtual void external_transition(const Port& port, MessageDeliverer& msg)
	{
		if(port == input)
		{
			elem_count++;
		}
	}

	virtual void internal_transition()
	{
		// TODO: Handle Dynamic Generation of States
		//if (CUR_STATE == IDLE3)
		//{
		//	CUR_STATE = IDLE3;
		//}
	};

	virtual void output_function(evsim::MessageDeliverer& msg)
	{
		
	};

public:
	DECLARE_STATE(IDLE, Infinity);
	DECLARE_INPUT_PORT(input);

	int elem_count;
};
