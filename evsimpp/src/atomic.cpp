#include "atomic.hpp"

#include "definition.hpp"

namespace evsim
{
	CAtomic::CAtomic(std::string name)
		:CModel(ATOMIC_TYPE, name), p_cur_state(nullptr)
	{
		
	}

	void CAtomic::register_state( unsigned long hash, std::string name, Time deadline)
	{
		m_states.insert(State(hash,name, deadline));
	}

	const std::set<State>& CAtomic::states() const
	{
		 return m_states; 
	}

}
