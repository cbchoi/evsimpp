#pragma once

# if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#endif
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

namespace evsim{
class CModel;
typedef std::shared_ptr<CModel> Model;

class Port;

class iExecutor;
typedef std::shared_ptr<iExecutor> IExecutor;


typedef unsigned long long UNIQ;

constexpr UNIQ ENGINE_TYPE  = 0x8000000000000000;
constexpr UNIQ MODEL_TYPE   = 0x4000000000000000;
constexpr UNIQ COUPLED_TYPE = 0x0800000000000000;
constexpr UNIQ ATOMIC_TYPE  = 0x0400000000000000;
constexpr UNIQ MESSAGE_TYPE = 0x0800000000000000;

typedef enum { SIMULATION_WAIT, SIMULATION_RUN, SIMULATION_PAUSE, SIMULATION_TERMINATED } ENGINE_STATUS;
typedef enum { BLOCKING, NONBLOCKING } EXE_MODE;
typedef enum { VIRTUAL, REAL } SIM_MODE;

typedef long double Time;
constexpr long double Infinity = std::numeric_limits<long double>::max();

typedef struct CC
{
    Time create_t; Time destory_t; CModel* p_model;
    bool operator<(const CC& rhs) { return create_t - rhs.create_t < 0 ? true : false; }
} create_constraint;

inline bool operator<(const create_constraint& lhs, const create_constraint& rhs) { return lhs.create_t - rhs.create_t < 0 ? true : false; }

typedef struct DC
{
    Time destory_t; CModel* p_model;
} destory_constraint;

inline bool operator<(const destory_constraint& lhs, const destory_constraint& rhs) { return lhs.destory_t - rhs.destory_t < 0 ? true : false; }

typedef struct EI
{
    Time next_event_t; IExecutor p_executor;
} executor_item;

typedef struct CR
{
    CModel* model;
    const Port*   port;
} coupling_relation;

inline bool operator<(const executor_item& lhs, const executor_item& rhs) { return lhs.next_event_t - rhs.next_event_t < 0 ? true : false; }

inline bool DIFF_CHECK(Time first, Time second)
{
    return fabs(first - second) <= std::numeric_limits<Time>::min();
}
}