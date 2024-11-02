#include "executor.hpp"

namespace evsim
{
	IExecutor::IExecutor()
	:global_t(0)
	{

	}

	void IExecutor::set_global_time(Time _time)
	{
		global_t = _time;
	}
}