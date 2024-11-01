#include "model.hpp"

namespace evsim
{
	UNIQ evsim::CModel::OBJECT_ID = 0;

	CModel::CModel(UNIQ _type, std::string name) :CObject(++OBJECT_ID, MODEL_TYPE | _type), m_model_name(name) 
	{
	}

	const std::string& CModel::get_name() const
	{
		return m_model_name;
	}

	void CModel::set_name(const std::string& _name)
	{
		m_model_name = _name;
	}

	const std::set<Port>& CModel::in_port() const
	{
		return m_in_port;
	}

	const std::set<Port>& CModel::out_port() const
	{
		return m_out_port;
	}

	void CModel::register_input_port(Port& port)
	{
		m_in_port.insert(port);
	}

	void CModel::register_output_port(Port& port)
	{
		m_out_port.insert(port);
	}
}
