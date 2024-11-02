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
		CModel(UNIQ _type,
			std::string name,
			Time itime = -1,
			Time dtime = -1,
			std::string ename = "");

public:
	const std::string& get_name() const;
	void set_name(const std::string& _name);

	const std::set<Port>& in_port() const;
	const std::set<Port>& out_port() const;

	virtual void register_input_port(Port& port);
	virtual void register_output_port(Port& port);

	virtual Time get_first_event_time() = 0;
private:
	std::string m_model_name;

	std::set<Port> m_in_port;
	std::set<Port> m_out_port;

protected:
	static UNIQ OBJECT_ID;

	Time instantiation_t;
	Time destruction_t;
	std::string engine_name;
};

	inline bool operator<(const coupling_relation& lhs, const coupling_relation& rhs)
	{
		if (lhs.model->obj_id() == rhs.model->obj_id())
			return lhs.port->m_hash - rhs.port->m_hash < 0 ? true : false;
		else
			return lhs.model->obj_id() - rhs.model->obj_id() < 0 ? true : false;
	}
}