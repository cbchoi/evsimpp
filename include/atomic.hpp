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
	void register_state(std::string name, unsigned long hash);
	const std::set<State>& states() const;

private:
	std::set<State> m_states;
};

}