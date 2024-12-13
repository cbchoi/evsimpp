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
}

