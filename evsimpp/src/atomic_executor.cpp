#include "atomic_executor.hpp"

#include <algorithm>

namespace evsim {
	CAtomicExecutor::CAtomicExecutor(
		Time itime,
		Time dtime,
		StringInfo ename,
		CAtomicModel* pbehavior)
			:IExecutor(itime, dtime, ename)
	{

	}

	void CAtomicExecutor::external_transition(const Port& port, const SystemMessage& msg)
	{
		behavior_object->external_transition(port, msg);
	}

	void CAtomicExecutor::internal_transition()
	{
		behavior_object->internal_transition();
	}

	void CAtomicExecutor::output_function(SystemMessage& msg)
	{
		behavior_object->output_function(msg);
	}

	Time CAtomicExecutor::time_advance()
	{
		return behavior_object->time_advance();;
	}

	void CAtomicExecutor::set_req_time(Time global_time)
	{
		
		set_global_time(global_time);
		if(time_advance() - (Infinity -1) > 0)
		{
			next_event_t = Infinity;
			request_t = Infinity;
		}
		else
		{
			if(behavior_object->is_reschedule())
				request_t = global_time + time_advance();
			else
				request_t = std::min(next_event_t, global_time + time_advance());
		}
	}

	Time CAtomicExecutor::get_req_time()
	{
		if(behavior_object->is_reschedule())
			behavior_object->set_reschedule(false);
		
		next_event_t = request_t;
		return request_t;
	}
}
