#pragma once

#pragma once

#include <string>
#include "definition.hpp"

namespace evsim
{
    class State
    {
    public:
        State(std::string name, Time deadline = Infinity);
        State(unsigned long hash, std::string name, Time deadline = Infinity);

    public:
        operator unsigned long() const;
        operator std::string() const;

        void hash(const std::string& str);

    public:
        std::string m_name;
        unsigned long m_hash;
        Time m_deadline;
    };

}

#define DECLARE_STATE(X, Y) \
class State_##X:public evsim::State{\
public: State_##X(): evsim::State(#X, Y) { }\
    bool operator==(const State_##X& rhs) const{\
        return rhs.m_hash == m_hash ? true : false;}} X

#define REGISTER_STATE(X) \
    register_state(X.m_hash, X.m_name, X.m_deadline)