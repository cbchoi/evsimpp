#pragma once

#include <string>

#include "model.hpp"
#include "state.hpp"
#include "system_message.hpp"

namespace evsim{

class CAtomicModel : public CModel
{
public:
	CAtomicModel(std::string name);
	CAtomicModel(const CAtomicModel& rhs);
	virtual ~CAtomicModel();

public:
	virtual void external_transition(const Port& port, const SystemMessage& msg){};
	virtual void internal_transition(){};
	virtual void output_function(SystemMessage& msg){};
	virtual Time time_advance();;

public: // Utility Functions
	void register_state(State state); 
	const std::set<State>& states() const;
	State& get_current_state() const { return *p_cur_state; }

	bool is_reschedule();
	void set_reschedule(bool sw);

protected:
	std::set<State> m_states;
	State* p_cur_state;
	bool m_cancel_reschedule_f; // cancel reschedule flag
};

}