#pragma once

#include <set>
#include <map>
#include <vector>

#include "sim_config.hpp"
#include "definition.hpp"
#include "object.hpp"

#include "model.hpp"
#include "message_deliverer.hpp"

namespace evsim{

class CSystemExecutor: public CModel
{
private:
	CSystemExecutor(std::string _name, SimConfig config);
	
public:
	virtual ~CSystemExecutor();
	static CSystemExecutor* create_system_executor(SimConfig config, std::string _name = "default");

public:
	port& create_input_port(std::string name);
	port& get_input_port(std::string name);
	port& create_output_port(std::string name);
	port& get_output_port(std::string name);

	void register_entity(CModel* model, Time itime, Time dtime);
	void destroy_entity();


	void insert_coupling(CModel* p_src, port& src_port,
						 CModel* p_dst, port& dst_port);

	void insert_external_event(Message msg);
	Message create_message(port& in_port, Time _time = 0);

	void sim_set_up();
	void sim_tear_down();
	Time schedule(Time t);
	Time simulate(Time t);

	Time get_global_time() { return m_global_t; }
	CExecutorFactory* get_executor_factory() { return m_config.ef; }
protected:
	void create_entity();
	void output_handling(Message& msg_deliver);
	void event_delivery_handling(MessageDeliverer& deliver);
protected:
	SimConfig m_config;
	static UNIQ OBJECT_ID;

	Time m_global_t;
		
	ENGINE_STATUS m_engine_status;
	EXE_MODE   m_execution_mode;
	SIM_MODE  m_simulation_mode;

	std::multiset<executor_item>		m_schedule_list;
	std::multiset<create_constraint>	m_wait_object_list;
	std::multiset<destory_constraint>	m_live_model_list;
	std::map<CModel*, IExecutor>		m_model_executor_map;

	std::map<coupling_relation, std::vector<coupling_relation>> m_coupling_map;

	std::map<std::string, std::shared_ptr<port>> m_external_input_ports; // TODO std::string -> StringInfo
	std::map<std::string, std::shared_ptr<port>> m_external_output_ports;
protected:
	MessageDeliverer m_external_input_event; // TODO MemoryLeak Check
	MessageDeliverer m_external_output_event;
};

}