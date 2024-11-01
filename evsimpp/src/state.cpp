#include "state.hpp"

namespace evsim
{
	State::State()
		:StringInfo(""), m_deadline(-1)
	{
		m_hash = 0;
	}
	State::State(std::string name, Time deadline)
	:StringInfo(name), m_deadline(deadline)
	{
		hash(m_name);
	}

	State::State(unsigned long hash, std::string name, Time deadline)
		: StringInfo(hash, name), m_deadline(deadline)
	{

	}

	State::~State()
	{
		
	}

}