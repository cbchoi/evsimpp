#pragma once

#include "definition.hpp"

namespace evsim {

class IExecutor
{
public:
	IExecutor(
		Time itime,
		Time dtime,
		std::string ename);

	/* TODO
	 * def ext_trans(self, port, msg):
        """Handles external transition based on port and message"""
        if self.behavior_model.get_cancel_flag():
            self._cancel_reschedule_f = True

        self.behavior_model.ext_trans(port, msg)

    # Internal Transition
    def int_trans(self):
        """Handles internal transition"""
        self.behavior_model.int_trans()

    # Output Function
    def output(self):
        """Executes the output function of the behavior model"""
        return self.behavior_model.output()

    # Time Advance Function
    def time_advance(self):
        """Returns the time advance value for the current state"""
        if self.behavior_model._cur_state in self.behavior_model._states:
            return self.behavior_model._states[self.behavior_model._cur_state]

        return -1


    def get_name(self) :
        """Returns the name of SnapshotExecutor.

        Returns :
        str : Name
        """
        return self.behavior_executor.get_name()

        def get_engine_name(self) :
        """Returns SysExecutor name of SnapshotExecutor.

        Returns :
        str : SysExecutor name
        """
        return self.behavior_executor.get_engine_name()

        def set_engine_name(self, engine_name) :
        """Sets SysExecutor name of SnapshotExecutor.

        Args :
        engine_name(str) : SysExecutor name
        """
        self.behavior_executor.set_engine_name(engine_name)

        def get_create_time(self) :
        """Returns the creation time of SnapshotExecutor.

        Returns :
        float : Creation time
        """
        return self.behavior_executor.get_create_time()

        def get_destruct_time(self) :
        """Returns the destruction time of SnapshotExecutor.

        Returns :
        float : Destruction time
        """
        return self.behavior_executor.get_destruct_time()
    */

public:
    virtual void external_transition(const Port& port, const SystemMessage& msg) = 0;
    virtual void internal_transition() = 0;
    virtual void output_function(SystemMessage& msg) = 0;

public:
    virtual void set_req_time(Time global_time) = 0;
    virtual Time get_req_time() = 0;
    void set_global_time(Time _time);
private:
	Time instantiation_t;
	Time destruction_t;
	std::string engine_name;

private:
    Time global_t;
};

}
