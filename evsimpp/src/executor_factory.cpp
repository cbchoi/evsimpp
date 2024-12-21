#include "executor_factory.hpp"

#include "atomic_executor.hpp"
#include "coupled_executor.hpp"

namespace evsim
{
	IExecutor CExecutorFactory::create_entity(CModel* model, iExecutor* parent, Time created_t, Time current_t)
	{
		if((model->obj_type() & ATOMIC_TYPE) == ATOMIC_TYPE)
		{
			iExecutor* exec = new CAtomicExecutor(Model(model), created_t);
			return IExecutor(exec);
		}
		else if((model->obj_type() & COUPLED_TYPE) == COUPLED_TYPE)
		{
			iExecutor* exec = new CHierarchicalCoupled(Model(model), this, parent, created_t, current_t);
			return IExecutor(exec);
		}

		return nullptr;
	}
}
