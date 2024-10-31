#pragma once

#include <string>

#include "model.hpp"
#include "state.hpp"
#include "system_message.hpp"

namespace evsim{

class CAtomic : public CModel
{
public:
	CAtomic(std::string name);

public:
	void register_state( unsigned long hash, std::string name, Time deadline);
	const std::set<State>& states() const;

	virtual void external_transition(const Port& port, const SystemMessage& msg){};
	virtual void internal_transition(){};
	virtual void output_function(SystemMessage& msg){};
private:
	std::set<State> m_states;

protected:
	State cur_state;
};

}