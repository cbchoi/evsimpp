#pragma once

#include "definition.hpp"
#include "message_deliverer.hpp"
#include <string>
#include "port.hpp"
namespace evsim {

class iExecutor
{
public:
    iExecutor();
    virtual ~iExecutor() = default;

public:
    virtual void external_transition(const port& port, Message& msg) = 0;
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
