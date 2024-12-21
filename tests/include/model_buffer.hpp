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

//#define MODEL_DEBUG

class CBuffer :public CAtomicModel
{
public:
	CBuffer(std::string name) :evsim::CAtomicModel(name), elem_count(0)
	{
		REGISTER_INPUT_PORT(input);
		REGISTER_OUTPUT_PORT(output);

		SET_INIT_STATE(IDLE);
	}

	~CBuffer() override
	{

	}

	virtual void external_transition(const port& _port, Message& msg) override
	{
#ifdef MODEL_DEBUG
		std::cout << get_name().m_name << std::endl;
		std::cout << "[DEBUG] EXT_TRANS" << std::endl;
#endif
		if (CUR_STATE == IDLE)
		{
			SET_NEXT_STATE(GEN);
		}
	}

	virtual void internal_transition() override
	{
		// TODO: Handle Dynamic Generation of States
#ifdef MODEL_DEBUG
		std::cout << "[DEBUG] INT_TRANS: Element Count" << elem_count << std::endl;
#endif		
		if (CUR_STATE == GEN)
			SET_NEXT_STATE(IDLE);
	};

	virtual void output_function(evsim::MessageDeliverer& msg_deliver) override
	{
		if (CUR_STATE == GEN)
		{
			Message msg = create_message(output);
			msg_deliver.insert_message(msg);
			elem_count++;
		}
#ifdef MODEL_DEBUG
		std::cout << "[DEBUG] OUTPUT" << elem_count << std::endl;
#endif		
	};

public:
	DECLARE_STATE(IDLE, Infinity);
	DECLARE_STATE(GEN, 0);

	DECLARE_INPUT_PORT(input);
	DECLARE_OUTPUT_PORT(output);

	int elem_count;
};
