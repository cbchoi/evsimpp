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
		if (!m_wait_object_list.empty())
		{
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

			for(std::vector<std::set<create_constraint>::iterator>::iterator iter = del_list.begin();
				iter != del_list.end(); ++iter)
			{
				m_wait_object_list.erase(*iter);
			}
		}
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
 		while (ei.next_event_t <= m_global_t) {
			create_entity();

			ei.p_executor->output_function(msg_deliverer);
			output_function(msg_deliverer);
			
			ei.p_executor->internal_transition();
			//Time prev_req_time = ei.p_executor->get_req_time();
			ei.p_executor->set_req_time(m_global_t);
			ei.next_event_t = ei.p_executor->get_req_time();

			m_schedule_list.insert(ei);

			// Move to the next object in multiset
			ei = *m_schedule_list.begin();
			m_schedule_list.erase(m_schedule_list.begin());
		}

		m_schedule_list.insert(ei);
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
