#pragma once
#include "atomic_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "message_deliverer.hpp"

namespace evsim
{
	class CUncaughtEventHandler:public CAtomicModel
	{
	public:
		CUncaughtEventHandler(std::string name) :evsim::CAtomicModel(name)
		{
			REGISTER_INPUT_PORT(input);
			REGISTER_STATE(IDLE);

			SET_INIT_STATE(IDLE);
		}


		virtual void external_transition(const port& port, Message& msg)
		{
			std::cout << "Uncaught Message:"
				<< "From: " << (msg.get())->get_source()->get_name() << "\n"
				<< "Port: " << (*(msg.get())->get_out_port()).m_name << std::endl;
		};

		
	public:
		DECLARE_STATE(IDLE, evsim::Infinity);
		DECLARE_INPUT_PORT(input);
		
	};
}