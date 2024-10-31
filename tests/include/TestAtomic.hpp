#pragma once

#include "atomic.hpp"
#include "definition.hpp"

class CTestAtomic :public evsim::CAtomic
{
public:
	CTestAtomic(std::string name);

public:
	DECLARE_STATE(IDLE, evsim::Infinity);

	DECLARE_INPUT_PORT(one);
	DECLARE_INPUT_PORT(two);
	DECLARE_INPUT_PORT(three);
};