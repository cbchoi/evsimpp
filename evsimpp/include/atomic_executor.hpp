#pragma once

namespace evsim {
#include "atomic_model.hpp"
#include "executor.hpp"
#include "string_info.hpp"
#include "state.hpp"

	class CAtomicExecutor:public CAtomicModel
	{
	public:
		CAtomicExecutor(
			Time itime,
			Time dtime,
			StringInfo ename,
			CAtomicModel* pbehavior);
	private:
		Time next_event_t; // Next event time
		Time request_t;  // Request time initialized to infinity

		CAtomicModel* behavior_object;
	};
}
