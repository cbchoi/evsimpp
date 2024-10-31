#include "atomic.hpp"

#include "definition.hpp"

namespace evsim
{
	CAtomic::CAtomic(std::string name)
		:CModel(ATOMIC_TYPE, name)
	{
		
	}

	void CAtomic::register_state(std::string name, unsigned long hash)
	{
		m_states.insert(State(hash,name));
	}

	const std::set<State>& CAtomic::states() const
	{
		 return m_states; 
	}

}
