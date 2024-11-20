#include "executor.hpp"

namespace evsim
{
	iExecutor::iExecutor()
	:global_t(0)
	{

	}

	void iExecutor::set_global_time(Time _time)
	{
		global_t = _time;
	}


}