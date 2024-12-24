#include "atomic_executor.hpp"

#include <algorithm>
#include <iostream>

namespace evsim {
	CAtomicExecutor::CAtomicExecutor(
		Model pbehavior, iExecutor* parent, Time creation_t):iExecutor(pbehavior, parent)
	{
		behavior_object = dynamic_cast<CAtomicModel*>(bobject.get());
		next_event_t = request_t = behavior_object->time_advance();
		this->set_req_time(creation_t);
	}

	CAtomicExecutor::~CAtomicExecutor()
	{
		bobject.reset();
	}

	void CAtomicExecutor::external_transition(const Port& port, Message& msg)
	{
		behavior_object->external_transition(port, msg);
	}

	void CAtomicExecutor::internal_transition()
	{
		behavior_object->internal_transition();
	}

	void CAtomicExecutor::output_function(MessageDeliverer& msg)
	{
		behavior_object->output_function(msg);
	}

	Time CAtomicExecutor::time_advance()
	{
		return behavior_object->time_advance();
	}

	void CAtomicExecutor::set_req_time(Time global_time)
	{
		//set_global_time(global_time);
		if(time_advance() - (Infinity -1) > 0)
		{
			next_event_t = Infinity;
			request_t = Infinity;
		}
		else
		{
			if(behavior_object->is_cancel_reschedule())
				request_t = std::min(next_event_t, global_time + time_advance()); 
			else
				request_t = global_time + time_advance();
				
		}
	}

	Time CAtomicExecutor::get_req_time()
	{
		if(behavior_object->is_cancel_reschedule())
			behavior_object->set_cancel_reschedule(false);

		return next_event_t = request_t;
	}
}
