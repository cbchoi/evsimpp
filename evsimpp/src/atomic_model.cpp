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

	void CAtomicModel::register_state( State state)
	{
		m_states.insert(state);
	}

	const std::set<State>& CAtomicModel::states() const
	{
		 return m_states; 
	}

}
