#pragma once

#include "executor_factory.hpp"
#include <string>
#include "definition.hpp"

namespace evsim
{
struct SimConfig
{
	std::string engine_name;
	Time time_resolution;
	EXE_MODE execution_mode;
	SIM_MODE simulation_mode;

	CExecutorFactory* ef;
};
}