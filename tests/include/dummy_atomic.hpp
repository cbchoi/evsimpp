#pragma once

#include <string>
#include <iostream>

#include "atomic_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "message_delivery.hpp"

using namespace evsim;

class CDummyAtomic :public CAtomicModel
{
public:
	CDummyAtomic(std::string name) :evsim::CAtomicModel(name)
	{
		REGISTER_INPUT_PORT(one);
		REGISTER_INPUT_PORT(two);
		REGISTER_INPUT_PORT(three);

		REGISTER_STATE(IDLE1);
		REGISTER_STATE(IDLE2);

		SET_INIT_STATE(IDLE1);
	}


	virtual void external_transition(const Port& port, const MessageDelivery& msg_delivery)
	{
		if(port == one && CUR_STATE == IDLE1)
		{
			std::cout << static_cast<std::string>(IDLE1) << " x " << static_cast<std::string>(one)
					  << " -> " << static_cast<std::string>(IDLE2) << std::endl;
			SET_NEXT_STATE(IDLE2);
		}
		else if (port == two && CUR_STATE == IDLE1)
		{
			std::cout << static_cast<std::string>(IDLE1) << " x " << static_cast<std::string>(two)
				<< " -> " << static_cast<std::string>(IDLE2) << std::endl;
			SET_NEXT_STATE(IDLE2);
		}
		else 
		{
			std::cout << (int)(port == two) << int(CUR_STATE == IDLE1) << std::endl;
			std::cout << "return to IDLE1" << std::endl;
			SET_NEXT_STATE(IDLE1);
		}
	};
	virtual void internal_transition()
	{
		if(CUR_STATE == IDLE1)
		{
			CUR_STATE = IDLE2;
		}
		else if(CUR_STATE == IDLE3)
		{
			CUR_STATE = IDLE3;
		}
	};
	virtual void output_function(evsim::MessageDelivery& msg)
	{
		std::cout << "Hello" << std::endl;
	};
public:
	DECLARE_STATE(IDLE1, 1);
	DECLARE_STATE(IDLE2, evsim::Infinity);
	DECLARE_STATE(IDLE3, 1);


	DECLARE_INPUT_PORT(one);
	DECLARE_INPUT_PORT(two);
	DECLARE_INPUT_PORT(three);
};
