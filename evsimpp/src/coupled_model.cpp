#include "coupled_model.hpp"

namespace evsim
{
	CCoupledModel::CCoupledModel(std::string name)
		:CModel(COUPLED_TYPE, name)
	{

	}

	CCoupledModel::CCoupledModel(const CCoupledModel& rhs)
		:CModel(COUPLED_TYPE, rhs.get_name())
	{// TODO
	}

	CCoupledModel::~CCoupledModel()
	{
	}

	void CCoupledModel::insert_coupling(CModel* p_src, port& src_port, CModel* p_dst, port& dst_port)
	{

	}
}

