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
		if(port == one)
		{
			std::cout << "one" << std::endl;
		}
		else if (port == two)
		{
			std::cout << "two" << std::endl;
		}
		else if (port == three)
		{
			std::cout << "three" << std::endl;
		}
	};
	virtual void internal_transition()
	{
		std::cout << std::string(cur_state) << std::endl;
		if(cur_state == IDLE1)
		{
			cur_state = IDLE2;
		}
		else
		{
			cur_state = IDLE1;
		}
		std::cout << std::string(cur_state) << std::endl;
	};
	virtual void output_function(evsim::SystemMessage& msg) {};
public:
	DECLARE_STATE(IDLE1, evsim::Infinity);
	DECLARE_STATE(IDLE2, evsim::Infinity);

	DECLARE_INPUT_PORT(one);
	DECLARE_INPUT_PORT(two);
	DECLARE_INPUT_PORT(three);
};
