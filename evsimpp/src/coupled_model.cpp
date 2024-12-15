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
		for(auto& iter: internal_models)
		{
			iter.second.reset();
		}
	}

	void CCoupledModel::insert_coupling(CModel* p_src, port& src_port, CModel* p_dst, port& dst_port)
	{
		coupling_relation src(p_src, &src_port);
		coupling_relation dst(p_dst, &dst_port);

		if (m_coupling_map.find(src) == m_coupling_map.end())
			m_coupling_map[src] = std::vector<coupling_relation>();
		m_coupling_map[src].push_back(dst);
	}

	void CCoupledModel::insert_model(CModel* pModel)
	{
		internal_models.insert(std::make_pair(pModel->get_name(), Model(pModel)));
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

	std::map<coupling_relation, std::vector<coupling_relation>>& CCoupledModel::get_couplings()
	{
		return m_coupling_map;
	}
}

