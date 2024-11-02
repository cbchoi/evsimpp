#include "executor_factory.hpp"

#include "atomic_executor.hpp"

namespace evsim
{
	IExecutor* CExecutorFactory::create_entity(CModel* model)
	{
		if((model->obj_type() & ATOMIC_TYPE) == ATOMIC_TYPE)
		{
			return new CAtomicExecutor(model);
		}
		else if((model->obj_type() & COUPLED_TYPE) == COUPLED_TYPE)
		{
			return nullptr;
		}

		return nullptr;
	}
}
