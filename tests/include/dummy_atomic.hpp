#pragma once

#include <string>
#include <iostream>

#include "atomic.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "system_message.hpp"

using namespace evsim;

class CDummyAtomic :public evsim::CAtomic
{
public:
	CDummyAtomic(std::string name);

	virtual void external_transition(const evsim::Port& port, const evsim::SystemMessage& msg)
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
		else
		{
			CUR_STATE = IDLE1;
		}
	};
	virtual void output_function(evsim::SystemMessage& msg) {};
public:
	DECLARE_STATE(IDLE1, evsim::Infinity);
	DECLARE_STATE(IDLE2, evsim::Infinity);


	DECLARE_INPUT_PORT(one);
	DECLARE_INPUT_PORT(two);
	DECLARE_INPUT_PORT(three);
};
