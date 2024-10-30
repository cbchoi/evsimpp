#pragma once

# if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#endif
#include <cmath>

namespace evsim{
typedef unsigned long long UNIQ;

constexpr UNIQ ENGINE_TYPE  = 0x8000000000000000;
constexpr UNIQ MODEL_TYPE   = 0x4000000000000000;
constexpr UNIQ COUPLED_TYPE = 0x0800000000000000;
constexpr UNIQ ATOMIC_TYPE  = 0x0400000000000000;
constexpr UNIQ MESSAGE_TYPE = 0x0800000000000000;

typedef struct {
    double x;
    double y;
} Pos;

inline double DEG2RAD(double deg)
{
    return deg * M_PI/180.0;
}

inline double CALC_DIST(const Pos& pos1, const Pos& pos2)
{
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

}