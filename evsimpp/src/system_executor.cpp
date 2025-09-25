#include "system_executor.hpp"
#include "system_executor.hpp"

#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <cassert>
#include "uncaught_handler.hpp"

#ifdef EV_DBG
#define _DBG_MODEL_EXECUTOR_
#define _DBG_COUPLING_
#endif
namespace evsim
{
	UNIQ CSystemExecutor::OBJECT_ID = 0;

	CSystemExecutor::CSystemExecutor(std::string _name, SimConfig config)
	:CModel(ENGINE_TYPE, _name), iExecutor(nullptr, nullptr), m_global_t(0), m_config(config)
	{
		m_engine_status = SIMULATION_WAIT;
		m_execution_mode = BLOCKING;
		m_simulation_mode = REAL;

		if (m_config.parallel_mode != ParallelMode::Serial)
		{
			size_t workers = m_config.parallel_workers;
			if (workers == 0)
			{
				size_t hc = std::thread::hardware_concurrency();
				if (hc == 0)
				{
					hc = 2;
				}
				workers = std::min<std::size_t>(hc, 8);
			}
			m_thread_pool = std::make_unique<ThreadPool>(workers);
		}
	}

	CSystemExecutor::~CSystemExecutor()
	{
		m_schedule_list.clear();
		m_wait_object_list.clear();
		m_live_model_list.clear();
		m_model_executor_map.clear();

		m_coupling_map.clear();

		m_external_input_ports.clear();
		m_external_output_ports.clear();
	}

	CSystemExecutor* CSystemExecutor::create_system_executor(SimConfig config, std::string _name)
	{ return new CSystemExecutor(_name, config); }

	Port& CSystemExecutor::create_input_port(std::string name)
	{
		std::shared_ptr<Port> _port = std::make_shared <Port>(name);
		m_external_input_ports[name] = _port;
		m_in_port.insert(*_port);
		return *_port;
	}

	Port& CSystemExecutor::get_input_port(std::string name)
	{
		return *m_external_input_ports[name];
	}

	Port& CSystemExecutor::create_output_port(std::string name)
	{
		std::shared_ptr<Port> _port = std::make_shared <Port>(name);
		m_external_output_ports[name] = _port;
		m_out_port.insert(*_port);
		return *_port;
	}

	Port& CSystemExecutor::get_output_port(std::string name)
	{
		return *m_external_output_ports[name];
	}

	void CSystemExecutor::register_entity(CModel* model, Time itime, Time dtime)
	{
		m_wait_object_list.insert(create_constraint(itime, dtime, model));
	}

	void CSystemExecutor::external_transition(const Port& _port, Message& msg)
	{
	coupling_relation cr(msg->get_source() == nullptr?this: msg->get_source(), &_port);
		std::map<coupling_relation, std::vector<coupling_relation>>::iterator iter = m_coupling_map.find(cr);

		if(iter != m_coupling_map.end())
		{
			route_message(cr, msg);
		}
		else
		{
			int a = 0;
		}
	}

	void CSystemExecutor::output_function(MessageDeliverer& msg_deliver)
	{
		while (msg_deliver.has_contents()) {
			Message msg = *msg_deliver.get_contents().begin();
			coupling_relation cr((msg.get())->get_source(), (msg.get())->get_out_port());
			std::map<coupling_relation, std::vector<coupling_relation>>::iterator iter = m_coupling_map.find(cr);

			route_message(cr, msg);

			msg_deliver.get_contents().erase(msg_deliver.get_contents().begin());
			msg.reset();
		}
	}

	void CSystemExecutor::create_entity()
	{
		if (m_config.use_optimized_scheduler)
		{
			create_entity_optimized();
		}
		else
		{
			create_entity_legacy();
		}
	}

	void CSystemExecutor::create_entity_legacy()
	{
		if (m_wait_object_list.empty())
		{
			return;
		}

		std::vector<std::set<create_constraint>::iterator> del_list;
		for (std::set<create_constraint>::iterator iter = m_wait_object_list.begin();
			iter != m_wait_object_list.end(); ++iter)
		{
			if (iter->create_t <= m_global_t)
			{
				Time d_time = iter->destory_t;
				CModel* pModel = iter->p_model;
				m_live_model_list.insert(destory_constraint(d_time, pModel));
				IExecutor executor = m_config.ef->create_entity(pModel, this, m_global_t, m_global_t);

				//executor->set_req_time(m_global_t);
				executor_item ei(executor->time_advance(), executor);
				m_schedule_list.insert(ei);
				m_model_executor_map.insert(std::make_pair(pModel, executor));
				del_list.push_back(iter);
			}
		}

		for (std::vector<std::set<create_constraint>::iterator>::iterator iter = del_list.begin();
			iter != del_list.end(); ++iter)
		{
			m_wait_object_list.erase(*iter);
		}
	}

	void CSystemExecutor::create_entity_optimized()
	{
		for (auto iter = m_wait_object_list.begin();
			iter != m_wait_object_list.end() && iter->create_t <= m_global_t;)
		{
			Time d_time = iter->destory_t;
			CModel* pModel = iter->p_model;
			m_live_model_list.insert(destory_constraint(d_time, pModel));
			IExecutor executor = m_config.ef->create_entity(pModel, this, m_global_t, m_global_t);

			executor_item ei(executor->time_advance(), executor);
			m_schedule_list.insert(ei);
			m_model_executor_map.insert(std::make_pair(pModel, executor));

			iter = m_wait_object_list.erase(iter);
		}
	}

	void CSystemExecutor::process_events_serial(MessageDeliverer& msg_deliverer, executor_item& ei)
	{
		while (ei.p_executor && ei.next_event_t <= m_global_t)
		{
			create_entity();
			ei.p_executor->output_function(msg_deliverer);
			output_function(msg_deliverer);

			ei.p_executor->internal_transition();
			ei.p_executor->set_req_time(m_global_t);
			ei.next_event_t = ei.p_executor->get_req_time();

			m_schedule_list.insert(ei);

			if (m_schedule_list.empty())
			{
				ei = executor_item();
				break;
			}

			ei = *m_schedule_list.begin();
			m_schedule_list.erase(m_schedule_list.begin());
		}
	}

	void CSystemExecutor::process_events_parallel_batch(MessageDeliverer& msg_deliverer, executor_item& ei)
	{
		if (!ei.p_executor)
		{
			return;
		}

		if (ei.next_event_t > m_global_t)
		{
			return;
		}

		if (!m_thread_pool)
		{
			process_events_serial(msg_deliverer, ei);
			return;
		}

		std::vector<executor_item> current_batch;
		current_batch.push_back(ei);

		while (!m_schedule_list.empty() && m_schedule_list.begin()->next_event_t <= m_global_t)
		{
			current_batch.push_back(*m_schedule_list.begin());
			m_schedule_list.erase(m_schedule_list.begin());
		}

		while (!current_batch.empty())
		{
			for (auto& item : current_batch)
			{
				create_entity();
				item.p_executor->output_function(msg_deliverer);
				output_function(msg_deliverer);
			}

			std::vector<executor_item> updated_items = current_batch;
			std::size_t concurrency = std::min<std::size_t>(m_thread_pool->worker_count(), updated_items.size());
			if (concurrency == 0)
			{
				concurrency = 1;
			}
			std::size_t chunk = (updated_items.size() + concurrency - 1) / concurrency;

			if (updated_items.size() > 1)
			{
				for (std::size_t c = 0; c < concurrency; ++c)
				{
					std::size_t start = c * chunk;
					if (start >= updated_items.size())
					{
						break;
					}
					std::size_t end = std::min(start + chunk, updated_items.size());

					m_thread_pool->enqueue([&, start, end]() {
						for (std::size_t idx = start; idx < end; ++idx)
						{
							auto executor = updated_items[idx].p_executor;
							executor->internal_transition();
							executor->set_req_time(m_global_t);
							updated_items[idx].next_event_t = executor->get_req_time();
						}
					});
				}

				m_thread_pool->wait();
			}
			else
			{
				for (auto& item : updated_items)
				{
					auto executor = item.p_executor;
					executor->internal_transition();
					executor->set_req_time(m_global_t);
					item.next_event_t = executor->get_req_time();
				}
			}

			for (auto& item : updated_items)
			{
				m_schedule_list.insert(item);
			}

			current_batch.clear();
			while (!m_schedule_list.empty() && m_schedule_list.begin()->next_event_t <= m_global_t)
			{
				current_batch.push_back(*m_schedule_list.begin());
				m_schedule_list.erase(m_schedule_list.begin());
			}
		}

		if (m_schedule_list.empty())
		{
			ei = executor_item();
			return;
		}

		ei = *m_schedule_list.begin();
		m_schedule_list.erase(m_schedule_list.begin());
	}

	void CSystemExecutor::route_message(coupling_relation& cr, Message& msg)
	{


#ifdef _DBG_MODEL_EXECUTOR_
		std::cout << "Message:";
		std::cout << msg.get_source() << ":" << msg.get_out_port() << std::endl;
		std::cout << "---m_model_executor_map---" << std::endl;
		for (std::map<CModel*, IExecutor*>::iterator iter = m_model_executor_map.begin();
			iter != m_model_executor_map.end(); ++iter)
		{

			std::cout << "p_model|executor : " << iter->first->get_name() << "("
				<< iter->first << "):"
				<< iter->second << std::endl;
		}
		std::cout << "---" << std::endl;
#endif
		std::map<coupling_relation, std::vector<coupling_relation>>::iterator iter = m_coupling_map.find(cr);

#ifdef _DBG_COUPLING_
		std::cout << "====m_coupling_map====" << std::endl;
		for (std::map<coupling_relation, std::vector<coupling_relation>>::iterator iter = m_coupling_map.begin();
			iter != m_coupling_map.end(); ++iter)
		{

			std::cout << "(src,dest) : " << iter->first.p_model->get_name()
				<< "(" << iter->first.p_model;
			std::cout << "):" << iter->first.p_port->m_name << "(" << iter->first.p_port << ")->" << std::endl;
			for (coupling_relation cr : iter->second)
			{
				std::cout << "\t";
				std::cout << cr.p_model->get_name()
					<< "(" << cr.p_model;
				std::cout << "):" << cr.p_port->m_name << "(" << cr.p_port << ")";
				std::cout << std::endl;
			}

		}
		std::cout << "====" << std::endl;
#endif
		if (iter != m_coupling_map.end())
		{
			for (coupling_relation scr : iter->second)
			{
				if(scr.model == this)
				{
					std::set<Port>::iterator iter = out_port().find(*scr.port);
					if (iter != out_port().end())
					{
						m_external_output_event.insert_message(msg);
					}
				}
				// internal coupling handling
				std::map<CModel*, IExecutor>::iterator dst = m_model_executor_map.find(scr.model);
				if (dst != m_model_executor_map.end())
				{
					m_schedule_list.erase(executor_item(dst->second->get_req_time(), dst->second));

					dst->second->external_transition(*scr.port, msg);
					dst->second->set_req_time(m_global_t);

					m_schedule_list.insert(executor_item(dst->second->get_req_time(), dst->second));
				}
			}

		}
		else
		{
			int a = 0;
		}
	}

	void CSystemExecutor::external_event_routing(MessageDeliverer& deliver)
	{
		// TODO:
		// Check Scheduled Event
		while (deliver.has_contents()) {
			if (deliver.get_first_event_time() <= m_global_t)
			{
				Message msg = *deliver.get_contents().begin();
				coupling_relation cr((msg.get())->get_source(), (msg.get())->get_out_port());
				route_message(cr, msg);

				deliver.get_contents().erase(deliver.get_contents().begin());
				msg.reset();
			}else
			{
				break;
			}
		}
	}

	void CSystemExecutor::destroy_entity()
	{
	}

	void CSystemExecutor::insert_coupling(CModel* p_src, Port& src_port, CModel* p_dst, Port& dst_port)
	{
		coupling_relation src(p_src, &src_port);
		coupling_relation dst(p_dst, &dst_port);

		if (m_coupling_map.find(src) == m_coupling_map.end())
			m_coupling_map[src] = std::vector<coupling_relation>();
		m_coupling_map[src].push_back(dst);
	}

	void CSystemExecutor::insert_external_event(Message msg)
	{
		m_external_input_event.insert_message(msg);
	}

	Message CSystemExecutor::create_message(Port& port, Time _time)
	{
		message* pMessage = new message(this, port, _time);
		return Message(pMessage);
	}

	void CSystemExecutor::sim_set_up()
	{
		m_engine_status = SIMULATION_RUN;
		if (m_wait_object_list.empty())
		{
			std::cerr << "[Engine] No Model registered" << std::endl;
			assert(m_wait_object_list.size());
		}

		m_global_t = 0;
	}


	void CSystemExecutor::sim_tear_down()
	{
	}

	Time CSystemExecutor::schedule(Time t)
	{
		m_engine_status = SIMULATION_RUN;
		//handle_external_input_event()
		create_entity();
		executor_item ei = *m_schedule_list.begin();
		auto before = std::chrono::high_resolution_clock::now();

		m_schedule_list.erase(m_schedule_list.begin());

		external_event_routing(m_external_input_event);

		// Main processing loop
		MessageDeliverer msg_deliverer;
		if (m_config.parallel_mode == ParallelMode::Serial)
		{
			process_events_serial(msg_deliverer, ei);
		}
		else
		{
			process_events_parallel_batch(msg_deliverer, ei);
		}

		if (ei.p_executor)
		{
			m_schedule_list.insert(ei);
		}
		m_global_t += m_config.time_resolution;

		// Call the entity destruction method
		//destroy_active_entity();

		// Handling real-time execution mode
		if (m_config.simulation_mode == REAL) {
			auto after = std::chrono::high_resolution_clock::now();
			Time elapsed = std::chrono::duration<Time>(after - before).count();
			Time delta = m_config.time_resolution - elapsed;

			if (delta > 0) {
				std::this_thread::sleep_for(std::chrono::duration<Time>(delta));
			}
		}

		m_engine_status = SIMULATION_PAUSE;
		return m_global_t;
	}

	Time CSystemExecutor::simulate(Time t)
	{
		if(m_engine_status == SIMULATION_WAIT)
			sim_set_up();

		Time target_t = m_global_t + t;
		for(; m_global_t <= target_t;
			m_global_t = schedule(target_t))
		{
			if(m_simulation_mode == VIRTUAL
				&& DIFF_CHECK((m_schedule_list.begin()->p_executor)->get_req_time(), Infinity))
			{
				m_engine_status = SIMULATION_TERMINATED;
				return Infinity;
			}
		}
		if (m_engine_status == SIMULATION_TERMINATED)
			sim_tear_down();

		return target_t;
	}
}
