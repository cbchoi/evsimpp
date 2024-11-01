#pragma once

#include "definition.hpp"

namespace evsim {

class Executor
{
public:
	Executor(
		Time itime,
		Time dtime,
		std::string ename)
	{
		
	}

private:
	Time instantiation_time;
	Time destruction_time;
	std::string engine_name;
};
}
