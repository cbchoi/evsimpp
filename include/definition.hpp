#pragma once

# if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#endif
#include <cmath>

typedef unsigned long long UNIQ_TYPE;
inline unsigned int TYPE_IDX = 0;
constexpr UNIQ_TYPE ENGINE_TYPE  = (1 << (TYPE_IDX++));
constexpr UNIQ_TYPE MESSAGE_TYPE = (1 << (TYPE_IDX++));
constexpr UNIQ_TYPE MODEL_TYPE   = (1 << (TYPE_IDX++));
constexpr UNIQ_TYPE ATOMIC_TYPE  = (1 << (TYPE_IDX++));
constexpr UNIQ_TYPE COUPLED_TYPE = (1 << (TYPE_IDX++));
constexpr UNIQ_TYPE GENERAL_TYPE = (1 << (TYPE_IDX++));

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