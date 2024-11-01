#include "atomic_model.hpp"

#include "definition.hpp"

namespace evsim
{
	CAtomicModel::CAtomicModel(std::string name)
		:CModel(ATOMIC_TYPE, name), p_cur_state(nullptr)
	{
		
	}

	CAtomicModel::~CAtomicModel()
	{
		
	}

	void CAtomicModel::register_state( unsigned long hash, std::string name, Time deadline)
	{
		m_states.insert(State(hash,name, deadline));
	}

	const std::set<State>& CAtomicModel::states() const
	{
		 return m_states; 
	}

}
