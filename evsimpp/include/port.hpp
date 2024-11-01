#pragma once

#include <string>

namespace evsim
{
    class Port
    {
    public:
        Port(std::string name);;
        Port(unsigned long hash, std::string name);

    public:
        operator unsigned long() const;
        operator std::string() const;
        virtual bool operator==(const Port& rhs) const { return false; };
        void hash(const std::string& str);
    public:
        std::string m_name;
        unsigned long m_hash;
    };

}

#define DECLARE_INPUT_PORT(X) \
class InPort_##X:public evsim::Port{\
public: InPort_##X(): evsim::Port(#X) { }\
    bool operator==(const Port& rhs) const{\
        return rhs.m_hash == m_hash ? true : false;}} X

#define REGISTER_INPUT_PORT(port_instance) \
    register_input_port(port_instance)

#define DECLARE_OUTPUT_PORT(X) \
class OutPort_##X:public evsim::Port{\
public: OutPort_##X(): evsim::Port(#X) { }\
    bool operator==(const Port& rhs) const{\
        return rhs.m_hash == m_hash ? true : false;}} X

#define REGISTER_OUTPUT_PORT(port_instance) \
    register_output_port(port_instance)
