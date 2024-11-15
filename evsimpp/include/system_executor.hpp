#pragma once

#include <set>
#include <map>

#include "sim_config.hpp"
#include "definition.hpp"
#include "object.hpp"

#include "model.hpp"

namespace evsim{

class CSystemExecutor: public CModel
{
private:
	CSystemExecutor(std::string _name, SimConfig config);

public:
	static CSystemExecutor* create_system_executor(SimConfig config, std::string _name = "default");

public:
	void register_entity(CModel* model, Time itime, Time dtime);
	void destroy_entity();

	void insert_coupling(CModel* p_src, Port& src_port,
						 CModel* p_dst, Port& dst_port);

	void sim_set_up();
	void sim_tear_down();
	Time schedule(Time t);
	Time simulate(Time t);
protected:
	void create_entity();
	void output_handling(MessageDeliverer& msg_deliver);
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
};

}