#pragma once

#include <string>
#include "definition.hpp"
#include "string_info.hpp"

namespace evsim
{
    class State: public StringInfo
    {
    public:
        State();
        State(std::string name, Time deadline = Infinity);
        State(unsigned long hash, std::string name, Time deadline = Infinity);
        virtual ~State();
    public:
        Time get_deadline() const { return m_deadline; }
        void udpate_deadline(Time td) { m_deadline = td; }

        Time m_deadline;
    };

}

#define DECLARE_STATE(X, Y) \
class State_##X:public evsim::State{\
public: State_##X(): evsim::State(#X, Y) { }\
    bool operator==(const State_##X& rhs) const{\
        return rhs.m_hash == m_hash ? true : false;}} X

#define REGISTER_STATE(X) \
    register_state(X)

#define SET_INIT_STATE(X) p_cur_state = &X
#define SET_NEXT_STATE(X) p_cur_state = &X

#define CUR_STATE *p_cur_state