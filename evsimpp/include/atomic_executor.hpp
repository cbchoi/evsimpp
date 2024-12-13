#pragma once

#include "atomic_model.hpp"
#include "executor.hpp"
#include "message_deliverer.hpp"
#include "string_info.hpp"
#include "state.hpp"

namespace evsim {

	class CAtomicExecutor:public iExecutor
	{
	public:
		CAtomicExecutor(CModel* pbehavior, Time creation_t);
		virtual ~CAtomicExecutor();

	public:
		virtual void external_transition(const port& _port, MessageDeliverer& msg);
		virtual void internal_transition();
		virtual void output_function(MessageDeliverer& msg);
		virtual Time time_advance();
	public:
		void set_req_time(Time global_time);
		Time get_req_time();

	private:
		Time next_event_t; // Next event time
		Time request_t;  // Request time initialized to infinity

		CAtomicModel* behavior_object;
	};
}
