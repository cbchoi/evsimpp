#include "executor.hpp"

namespace evsim
{
	Executor::Executor(Time itime, Time dtime, std::string ename) :instantiation_time(itime),
		destruction_time(dtime),
		engine_name(ename)
	{

	}

	inline void Executor::set_global_time(Time _time)
	{
		global_t = _time;
	}
}