#pragma once

#include "executor.hpp"
#include "model.hpp"

namespace evsim {
class CExecutorFactory
{
public:
	CExecutorFactory() = default;

	IExecutor* create_entity(CModel* model);
};
}
