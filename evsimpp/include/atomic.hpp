#pragma once

#include <string>

#include "model.hpp"
#include "state.hpp"

namespace evsim{

class CAtomic : public CModel
{
public:
	CAtomic(std::string name);

public:
	void register_state( unsigned long hash, std::string name, Time deadline);
	const std::set<State>& states() const;

private:
	std::set<State> m_states;
};

}