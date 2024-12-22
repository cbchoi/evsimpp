#include "coupled_executor.hpp"
#include "system_executor.hpp"

evsim::CHierarchicalCoupled::CHierarchicalCoupled(Model pbehavior, CExecutorFactory* ef, iExecutor* parent, Time creation_t,  Time current_t)
	:iExecutor(pbehavior, parent), request_t(current_t), next_event_t(current_t), m_ef(ef)
{
	behavior_object = dynamic_cast<CCoupledModel*>(bobject.get());
	// create every executor for internal models
	// find earlist model
	for(auto& pairs:behavior_object->get_models())
	{
		IExecutor executor = ef->create_entity(pairs.second.get(), this, creation_t, current_t);

		executor->set_req_time(creation_t);
		executor_item ei(executor->time_advance(), executor);
		m_schedule_list.insert(ei);
		m_model_executor_map.insert(std::make_pair(pairs.second.get(), executor));
	}

	time_advance();
}

evsim::CHierarchicalCoupled::~CHierarchicalCoupled()
{
	for(executor_item ei : m_schedule_list)
	{
		ei.p_executor.reset();
	}
	bobject.reset();
}

void evsim::CHierarchicalCoupled::external_transition(const port& _port, Message& msg)
{
	// if msg->get_source() == parent => EIC
	CModel* source = dynamic_cast<CModel*>(parent_executor);
	if(msg->get_source() == source)
	{// EIC
		coupling_relation cr = coupling_relation(behavior_object, &_port);
		route_message(cr, msg);
	}
	else
	{// EOC
		coupling_relation cr = coupling_relation(behavior_object, &_port);
		route_message(cr, msg);
	}
}

void evsim::CHierarchicalCoupled::internal_transition()
{
	// TODO Implement
	executor_item ei = *m_schedule_list.begin();
	m_schedule_list.erase(m_schedule_list.begin());

	ei.p_executor->internal_transition();
	//Time prev_req_time = ei.p_executor->get_req_time();
	ei.p_executor->set_req_time(request_t);
	ei.next_event_t = ei.p_executor->get_req_time();

	m_schedule_list.insert(ei);
}

void evsim::CHierarchicalCoupled::output_function(MessageDeliverer& msg_deliver)
{
	if(!msg_deliver.has_contents())
	{
		// TODO Implement
		// 1. invoke output_function of internal model
		IExecutor iexec = (*m_schedule_list.begin()).p_executor;
		iexec->output_function(msg_deliver);
	}

	while (msg_deliver.has_contents()) {
		Message msg = *msg_deliver.get_contents().begin();
		coupling_relation cr((msg.get())->get_source(), (msg.get())->get_out_port());
		std::map<coupling_relation, std::vector<coupling_relation>>::iterator iter = behavior_object->get_couplings().find(cr);

		route_message(cr, msg);

		msg_deliver.get_contents().erase(msg_deliver.get_contents().begin());
		msg.reset();
	}
	
}

evsim::Time evsim::CHierarchicalCoupled::time_advance()
{
	return (*m_schedule_list.begin()).p_executor->time_advance();
}

void evsim::CHierarchicalCoupled::route_message(coupling_relation& cr, Message& msg)
{

	std::map<coupling_relation, std::vector<coupling_relation>>::iterator iter = behavior_object->get_couplings().find(cr);

	if (iter != behavior_object->get_couplings().end())
	{
		for (coupling_relation cr : iter->second)
		{
			// External Output Coupling Handling
			if (cr.model == behavior_object)
			{
				msg->set_source((CModel*)(cr.model));
				msg->set_port(cr.port);

				MessageDeliverer msg_deliver;
				msg_deliver.insert_message(msg);
				parent_executor->output_function(msg_deliver);
			}
			else
			{
				// internal coupling handling
				std::map<CModel*, IExecutor>::iterator dst = m_model_executor_map.find(cr.model);
				if (dst != m_model_executor_map.end())
				{
					m_schedule_list.erase(executor_item(dst->second->get_req_time(), dst->second));

					dst->second->external_transition(*cr.port, msg);
					dst->second->set_req_time(request_t);

					m_schedule_list.insert(executor_item(dst->second->get_req_time(), dst->second));
				}
			}
		}
	}
}

void evsim::CHierarchicalCoupled::set_req_time(Time global_time)
{
	if (time_advance() - (Infinity - 1) > 0)
	{
		next_event_t = Infinity;
		request_t = Infinity;
	}
	else
	{
		request_t = global_time + time_advance();
	}
}

evsim::Time evsim::CHierarchicalCoupled::get_req_time()
{
	// TODO Implement
return next_event_t = request_t;
}
