#pragma once

#include <string>
#include "string_info.hpp"

namespace evsim
{
    class port:public StringInfo
    {
    public:
        port(std::string name);
        port(unsigned long hash, std::string name);
        virtual ~port();

    };

}

#define DECLARE_INPUT_PORT(X) \
class InPort_##X:public evsim::port{\
public: InPort_##X(): evsim::port(#X) { }\
    bool operator==(const port& rhs) const{\
        return rhs.m_hash == m_hash ? true : false;}} X

#define REGISTER_INPUT_PORT(port_instance) \
    register_input_port(port_instance)

#define DECLARE_OUTPUT_PORT(X) \
class OutPort_##X:public evsim::port{\
public: OutPort_##X(): evsim::port(#X) { }\
    bool operator==(const StringInfo& rhs) const{\
        return rhs.m_hash == m_hash ? true : false;}} X

#define REGISTER_OUTPUT_PORT(port_instance) \
    register_output_port(port_instance)
