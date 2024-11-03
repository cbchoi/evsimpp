#pragma once

#include <string>

#include "model.hpp"
#include "state.hpp"
#include "message_deliverer.hpp"

namespace evsim{

class CAtomicModel : public CModel
{
public:
	CAtomicModel(std::string name);
	CAtomicModel(const CAtomicModel& rhs);
	virtual ~CAtomicModel();

public:
	virtual void external_transition(const Port& port, const MessageDeliverer& msg){};
	virtual void internal_transition(){};
	virtual void output_function(MessageDeliverer& msg){};
	virtual Time time_advance();

public: // Utility Functions
	void register_state(State state); 
	const std::set<State>& states() const;
	State& get_current_state() const { return *p_cur_state; }
	void set_current_state(State* state) { p_cur_state = state; }

	Time get_first_event_time() override;
	bool is_cancel_reschedule() const;
	void set_cancel_reschedule(bool sw);

protected:
	std::set<State> m_states;
	State* p_cur_state;
	bool m_cancel_reschedule_f; // cancel reschedule flag
};

}