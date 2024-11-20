#include "atomic_model.hpp"

#include "definition.hpp"

namespace evsim
{
	CAtomicModel::CAtomicModel(std::string name)
		:CModel(ATOMIC_TYPE, name), p_cur_state(nullptr), m_cancel_reschedule_f(false)
	{
		
	}

	CAtomicModel::~CAtomicModel()
	{
		
	}

	CAtomicModel::CAtomicModel(const CAtomicModel& rhs)
		:CModel(ATOMIC_TYPE, rhs.get_name()),
		 p_cur_state(rhs.p_cur_state),
		 m_cancel_reschedule_f(rhs.m_cancel_reschedule_f)
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

	//Time CAtomicModel::get_first_event_time()
	//{
	//	return time_advance();
	//}

	bool CAtomicModel::is_cancel_reschedule() const
	{
		return m_cancel_reschedule_f;
	}

	void CAtomicModel::set_cancel_reschedule(bool sw)
	{
		m_cancel_reschedule_f = sw;
	}

	Message CAtomicModel::create_message(Port& port, Time _time)
	{
		message* pMessage = new message(this, port, _time);
		return Message(pMessage);
	}
}
