#include "system_executor.hpp"

#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <cassert>
#include "uncaught_handler.hpp"

namespace evsim
{
	UNIQ CSystemExecutor::OBJECT_ID = 0;

	CSystemExecutor::CSystemExecutor(SimConfig config)
	:CObject(CSystemExecutor::OBJECT_ID++, ENGINE_TYPE), m_global_t(0), m_config(config)
	{
		m_engine_status = SIMULATION_WAIT;
		m_execution_mode = BLOCKING;
		m_simulation_mode = REAL;
	}

	CSystemExecutor* CSystemExecutor::create_system_executor(SimConfig config)
	{ return new CSystemExecutor(config); }

	void CSystemExecutor::register_entity(CModel* model, Time itime, Time dtime)
	{
		m_wait_object_list.insert(create_constraint(itime, dtime, model));
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
					IExecutor* executor = m_config.ef->create_entity(pModel);
					
					executor->set_req_time(m_global_t);
					executor_item ei(pModel->get_first_event_time(), executor);
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

	void CSystemExecutor::output_handling(MessageDeliverer& msg_deliver)
	{
		for( Message msg : msg_deliver.get_contents())
		{
			coupling_relation cr(msg.get_source(), msg.get_out_port());
			std::map<coupling_relation, std::vector<coupling_relation>>::iterator iter = m_coupling_map.find(cr);
			if (iter != m_coupling_map.end())
			{
				// external output coupling handling
				//if destination[0] is self :
				//self.output_event_queue.append((self.global_time, msg[1].retrieve()))
				// internal coupling handling
				for(coupling_relation cr : iter->second)
				{
					std::map<CModel*, IExecutor*>::iterator dst = m_model_executor_map.find(cr.model);
					if (dst != m_model_executor_map.end())
					{
						dst->second->external_transition(*cr.port, msg_deliver);
						dst->second->set_req_time(m_global_t);
					}
				}
				
			}
			else
			{
				// msg uncaught execption
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
		create_entity();
		//handle_external_input_event()

		executor_item ei = *m_schedule_list.begin();
		auto before = std::chrono::high_resolution_clock::now();

		m_schedule_list.erase(m_schedule_list.begin());

		// Main processing loop
		MessageDeliverer msg_deliverer;
 		while (ei.next_event_t <= m_global_t) {
			
			ei.p_executor->output_function(msg_deliverer);
			if (msg_deliverer.has_contents()) {
				output_handling(msg_deliverer);
			}
			
			ei.p_executor->internal_transition();
			//Time prev_req_time = ei.p_executor->get_req_time();
			ei.p_executor->set_req_time(m_global_t);
			ei.next_event_t = ei.p_executor->get_req_time();

			m_schedule_list.insert(ei);

			// Move to the next object in deque
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
