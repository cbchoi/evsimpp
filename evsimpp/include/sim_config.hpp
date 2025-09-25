#pragma once

#include "executor_factory.hpp"
#include <string>
#include "definition.hpp"

namespace evsim
{
enum class ParallelMode
{
	Serial = 0,
	ParallelEventBatch,
	ParallelRouting
};

struct SimConfig
{
	std::string engine_name;
	Time time_resolution;
	EXE_MODE execution_mode;
	SIM_MODE simulation_mode;
	bool use_optimized_scheduler = false;
	ParallelMode parallel_mode = ParallelMode::Serial;
    std::size_t parallel_workers = 0;

	CExecutorFactory* ef;
};
}
