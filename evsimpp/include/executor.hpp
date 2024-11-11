#pragma once

#include "definition.hpp"
#include "message_deliverer.hpp"
#include <string>
#include "port.hpp"
namespace evsim {

class IExecutor
{
public:
	IExecutor();

public:
    virtual void external_transition(const Port& port, MessageDeliverer& msg) = 0;
    virtual void internal_transition() = 0;
    virtual void output_function(MessageDeliverer& msg) = 0;
    virtual Time time_advance() = 0;

public:
    virtual void set_req_time(Time global_time) = 0;
    virtual Time get_req_time() = 0;
    void set_global_time(Time _time);

private:
    Time global_t;
};

}
