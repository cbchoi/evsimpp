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

	CAtomicModel::CAtomicModel(const CAtomicModel& rhs)
		:CModel(ATOMIC_TYPE, rhs.get_name())
	{
		m_states = rhs.m_states;
	}

	Time CAtomicModel::time_advance()
	{
		return p_cur_state != nullptr ? p_cur_state->get_deadline(): -1;
	} 

	void CAtomicModel::register_state( State state)
	{
		m_states.insert(state);
	}

	const std::set<State>& CAtomicModel::states() const
	{
		 return m_states; 
	}

	bool CAtomicModel::is_reschedule()
	{
		return !m_cancel_reschedule_f;
	}

	void CAtomicModel::set_reschedule(bool sw)
	{
		m_cancel_reschedule_f = sw;
	}
}
