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
	virtual ~CSystemExecutor();;
public:
	static CSystemExecutor* create_system_executor(SimConfig config, std::string _name = "default");

public:
	Port& create_input_port(std::string name);
	Port& get_input_port(std::string name);
	Port& create_output_port(std::string name);
	Port& get_output_port(std::string name);

	void register_entity(CModel* model, Time itime, Time dtime);
	void destroy_entity();


	void insert_coupling(CModel* p_src, Port& src_port,
						 CModel* p_dst, Port& dst_port);

	void insert_external_event( Message msg);
	Message& create_message(Port& in_port, Time _time = 0);

	void sim_set_up();
	void sim_tear_down();
	Time schedule(Time t);
	Time simulate(Time t);
protected:
	void create_entity();
	void output_handling(MessageDeliverer& msg_deliver);
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
	std::map<CModel*, IExecutor*>		m_model_executor_map;

	std::map<coupling_relation, std::vector<coupling_relation>> m_coupling_map;

	std::map<std::string, std::shared_ptr<Port>> m_external_input_ports;
	std::map<std::string, std::shared_ptr<Port>> m_external_output_ports;
protected:
	MessageDeliverer m_external_input_event;
	MessageDeliverer m_external_output_event;
};

}