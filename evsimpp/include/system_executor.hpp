#pragma once

#include <set>
#include <map>

#include "sim_config.hpp"
#include "definition.hpp"
#include "object.hpp"

#include "model.hpp"

namespace evsim{

class CSystemExecutor: public CObject
{
private:
	CSystemExecutor(SimConfig config);

public:
	static CSystemExecutor* create_system_executor(SimConfig config);

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
protected:
	SimConfig m_config;
	static UNIQ OBJECT_ID;

	Time m_global_t;

	
	ENGINE_STATUS m_engine_status;
	EXE_MODE   m_execution_mode;
	SIM_MODE  m_simulation_mode;

	std::multiset<executor_item>		m_schedule_list;
	std::multiset<create_constraint>	m_wait_object_list;
	std::multiset<destory_constraint>	m_active_model_list;

	std::map<coupling_relation, coupling_relation> m_coupling_map;
};

}