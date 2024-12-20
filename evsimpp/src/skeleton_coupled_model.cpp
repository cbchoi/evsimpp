#include "skeleton_coupled_model.hpp"

evsim::CSkeletonCoupledModel::CSkeletonCoupledModel(std::string name)
	:CCoupledModel(name)
{
}

evsim::CSkeletonCoupledModel::CSkeletonCoupledModel(const CCoupledModel& rhs)
	:CCoupledModel(rhs.get_name())
{
}
evsim::CSkeletonCoupledModel::~CSkeletonCoupledModel()
{
	for (std::map<StringInfo, port*>::iterator iter = m_skel_in_ports.begin();
		iter != m_skel_in_ports.end(); ++iter)
	{
		delete iter->second;
	}

	for (std::map<StringInfo, port*>::iterator iter = m_skel_out_ports.begin();
		iter != m_skel_out_ports.end(); ++iter)
	{
		delete iter->second;
	}
}
evsim::port& evsim::CSkeletonCoupledModel::create_input_port(std::string name)
{
	if (m_skel_in_ports.find(StringInfo(name)) == m_skel_in_ports.end())
	{
		port* in_port = new port(name);
		m_skel_in_ports[StringInfo(name)] = in_port;
		register_input_port(*in_port);
		return *in_port;
	}
	else
	{// Exception
		return INVAILD_PORT;
	}
}

evsim::port& evsim::CSkeletonCoupledModel::create_output_port(std::string name)
{
	if (m_skel_out_ports.find(StringInfo(name)) == m_skel_out_ports.end())
	{
		port* output_port = new port(name);
		m_skel_out_ports[StringInfo(name)] = output_port;
		register_output_port(*output_port);
		return *output_port;
	}
	else
	{// Exception
		return INVAILD_PORT;
	}
}