#pragma once

#include <string>
#include <iostream>

#include "atomic_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "message_deliverer.hpp"
#include "message.hpp"

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

	virtual ~CPEG()
	{
		
	}

	virtual void external_transition(const port& _port, Message& msg)
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

	virtual void output_function(evsim::MessageDeliverer& msg_deliver)
	{
		Message msg = create_message(output);
		msg_deliver.insert_message(msg);
		elem_count++;
	};

public:
	//DECLARE_STATE(IDLE3, 1);

	DECLARE_INPUT_PORT(input);
	DECLARE_OUTPUT_PORT(output);

	int elem_count;
};
