#include "model.hpp"

namespace evsim
{
	UNIQ CModel::OBJECT_ID = 0;

	CModel::CModel(UNIQ _type, 
	               std::string name, 
	               Time itime, Time dtime, std::string ename)
	:CObject(++OBJECT_ID, MODEL_TYPE | _type),
			 m_model_name(name),
			instantiation_t(itime),
			destruction_t(dtime),
			engine_name(ename)
	{
	}

	const StringInfo& CModel::get_name() const
	{
		return m_model_name;
	}

	void CModel::set_name(const std::string& _name)
	{
		m_model_name = _name;
	}

	const std::set<port>& CModel::in_port() const
	{
		return m_in_port;
	}

	const std::set<port>& CModel::out_port() const
	{
		return m_out_port;
	}

	void CModel::register_input_port(port& _port)
	{
		m_in_port.insert(_port);
	}

	void CModel::register_output_port(port& _port)
	{
		m_out_port.insert(_port);
	}

	
}
