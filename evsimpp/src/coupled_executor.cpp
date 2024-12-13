#include "coupled_executor.hpp"

evsim::CHierarchicalCoupled::CHierarchicalCoupled(CModel* pbehavior, Time creation_t,  CExecutorFactory* ef)
{
	behavior_object = dynamic_cast<CCoupledModel*>(pbehavior);
	// create every executor for internal models
	// find earlist model
	for(auto& pairs:behavior_object->get_models())
	{
		IExecutor executor = ef->create_entity(pairs.second.get(), creation_t);

		executor->set_req_time(creation_t);
		executor_item ei(executor->time_advance(), executor);
		m_schedule_list.insert(ei);
		m_model_executor_map.insert(std::make_pair(pairs.second.get(), executor));
	}
}

evsim::CHierarchicalCoupled::~CHierarchicalCoupled()
{
	delete behavior_object;
}

void evsim::CHierarchicalCoupled::external_transition(const port& _port, MessageDeliverer& msg)
{
	// TODO Implement
}

void evsim::CHierarchicalCoupled::internal_transition()
{
	// TODO Implement
}

void evsim::CHierarchicalCoupled::output_function(MessageDeliverer& msg)
{
	// TODO Implement
}

evsim::Time evsim::CHierarchicalCoupled::time_advance()
{
	return (*m_schedule_list.begin()).next_event_t;
}

void evsim::CHierarchicalCoupled::set_req_time(Time global_time)
{
	// TODO Implement
}

evsim::Time evsim::CHierarchicalCoupled::get_req_time()
{
	// TODO Implement
	return 0;
}
