#include "executor.hpp"

namespace evsim
{
	iExecutor::iExecutor(Model model, iExecutor* parent)
	:global_t(0), bobject(model), parent_executor(parent)
	{

	}

	void iExecutor::set_global_time(Time _time)
	{
		global_t = _time;
	}


}