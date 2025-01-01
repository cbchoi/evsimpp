#include "coupled_model.hpp"

namespace evsim
{
	CCoupledModel::CCoupledModel(std::string name)
		:CModel(COUPLED_TYPE, name), next_event_t(Infinity)
	{

	}

	CCoupledModel::CCoupledModel(const CCoupledModel& rhs)
		:CModel(COUPLED_TYPE, rhs.get_name()), next_event_t(Infinity)
	{// TODO
	}

	CCoupledModel::~CCoupledModel()
	{
		for (auto& iter : internal_models)
		{
			iter.second.reset();
		}

		for (std::map<StringInfo, Port*>::iterator iter = m_dynamic_in_ports.begin();
			iter != m_dynamic_in_ports.end(); ++iter)
		{
			delete iter->second;
		}

		for (std::map<StringInfo, Port*>::iterator iter = m_dynamic_out_ports.begin();
			iter != m_dynamic_out_ports.end(); ++iter)
		{
			delete iter->second;
		}
	}

	evsim::Port& evsim::CCoupledModel::create_input_port(std::string name)
	{
		if (m_dynamic_in_ports.find(StringInfo(name)) == m_dynamic_in_ports.end())
		{
			Port* in_port = new Port(name);
			m_dynamic_in_ports[StringInfo(name)] = in_port;
			register_input_port(*in_port);
			return *in_port;
		}
		else
		{// Exception
			return INVAILD_PORT;
		}
	}

	evsim::Port& evsim::CCoupledModel::create_output_port(std::string name)
	{
		if (m_dynamic_out_ports.find(StringInfo(name)) == m_dynamic_out_ports.end())
		{
			Port* output_port = new Port(name);
			m_dynamic_out_ports[StringInfo(name)] = output_port;
			register_output_port(*output_port);
			return *output_port;
		}
		else
		{// Exception
			return INVAILD_PORT;
		}
	}

	void CCoupledModel::insert_coupling(CModel* p_src, Port& src_port, CModel* p_dst, Port& dst_port)
	{
		coupling_relation src(p_src, &src_port);
		coupling_relation dst(p_dst, &dst_port);

		if (m_coupling_map.find(src) == m_coupling_map.end())
			m_coupling_map[src] = std::vector<coupling_relation>();
		m_coupling_map[src].push_back(dst);

		if (p_dst == this)
		{
			insert_external_output_coupling(src_port, dst_port);
		}
		else if (p_src == this)
		{
			insert_external_input_coupling(src_port, dst_port);
		}
		else
		{
			insert_internal_coupling(src_port, dst_port);
		}
	}

	void CCoupledModel::insert_external_input_coupling(Port& src, Port& dst)
	{
		if (m_eic_map.find(&src) == m_eic_map.end())
			m_eic_map[&src] = std::vector<Port*>();
		m_eic_map[&src].push_back(&dst);
	}

	void CCoupledModel::insert_internal_coupling(Port& src, Port& dst)
	{
		if (m_ic_map.find(&src) == m_ic_map.end())
			m_ic_map[&src] = std::vector<Port*>();
		m_ic_map[&src].push_back(&dst);
	}

	void CCoupledModel::insert_external_output_coupling(Port& src, Port& dst)
	{
		if (m_eoc_map.find(&src) == m_eoc_map.end())
			m_eoc_map[&src] = std::vector<Port*>();
		m_eoc_map[&src].push_back(&dst);
	}

	void CCoupledModel::insert_model(CModel* pModel)
	{
		internal_models.insert(std::make_pair(pModel->get_name(), Model(pModel)));
	}

	void CCoupledModel::remove_model(CModel* pModel)
	{
		internal_models.erase(pModel->get_name());
	}

	Model CCoupledModel::find_model(std::string name)
	{
		StringInfo siName(name);
		std::map<StringInfo, Model>::iterator iter = internal_models.find(siName);
		if (iter != internal_models.end())
		{
			return internal_models[siName];
		}
		return nullptr;
	}

	std::map<StringInfo, Model>& CCoupledModel::get_models()
	{
		return internal_models;
	}

	Port* CCoupledModel::find_dyn_in_port(std::string name)
	{
		StringInfo spName(name);
		std::map<StringInfo, Port*>::iterator iter = m_dynamic_in_ports.find(spName);
		if (iter != m_dynamic_in_ports.end())
		{
			return m_dynamic_in_ports[spName];
		}
		return nullptr;
	}

	Port* CCoupledModel::find_dyn_out_port(std::string name)
	{
		StringInfo spName(name);
		std::map<StringInfo, Port*>::iterator iter = m_dynamic_out_ports.find(spName);
		if (iter != m_dynamic_out_ports.end())
		{
			return m_dynamic_out_ports[spName];
		}
		return nullptr;
	}

	std::map<coupling_relation, std::vector<coupling_relation>>& CCoupledModel::get_couplings()
	{
		return m_coupling_map;
	}

	std::map<Port*, std::vector<Port*>>& CCoupledModel::get_eic()
	{
		return m_eic_map;
	}

	std::map<Port*, std::vector<Port*>>& CCoupledModel::get_ic()
	{
		return m_ic_map;
	}

	std::map<Port*, std::vector<Port*>>& CCoupledModel::get_eoc()
	{
		return m_eoc_map;
	}
}

