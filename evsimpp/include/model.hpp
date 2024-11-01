#pragma once

#include <string>
#include <ostream>
#include <set>

#include "object.hpp"
#include "port.hpp"

namespace evsim{
	class CModel : public CObject
	{
	public:
		CModel(UNIQ _type, std::string name);

public:
	const std::string& get_name() const;
	void set_name(const std::string& _name);

	const std::set<Port>& in_port() const;
	const std::set<Port>& out_port() const;

	virtual void register_input_port(Port& port);
	virtual void register_output_port(Port& port);

private:
	std::string m_model_name;

	std::set<Port> m_in_port;
	std::set<Port> m_out_port;

protected:
	static UNIQ OBJECT_ID;
};
}