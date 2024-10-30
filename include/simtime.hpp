#pragma once

#include "definition.hpp"
template<typename X>
class Time
{
public:
	Time() :m_val(0) {}
	Time(const Time& rhs) { m_val = rhs.m_val; }
	Time(X _t) :m_val(_t) {}

public:
	Time operator=(X _v) { m_val = _v; return m_val; }
	bool operator==(const Time& rhs)
	{
		return fabs(m_val - rhs.m_val) < LDBL_EPSILON;
	}

	//friend bool operator==(const X& lhs, const Time& rhs) {
	//	return fabs(rhs.m_val - lhs) < LDBL_EPSILON;
	//}

	friend bool operator==(const Time& lhs, const X& rhs) {
		return fabs(lhs.m_val - rhs) < LDBL_EPSILON;
	}

	friend std::ostream& operator<<(std::ostream& os, const Time& rhs)
	{
		return os << rhs.m_val;
	}
public:
	long double m_val;
};