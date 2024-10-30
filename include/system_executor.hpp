#pragma once

#include "object.hpp"
#include "model.hpp"
#include <map>

namespace evsim{
class CSystemExecutor: public CObject
{
private:
	static UNIQ OBJECT_ID;
	CSystemExecutor():CObject(OBJECT_ID++, ENGINE_TYPE){}

public:
	static CSystemExecutor* create_system_executor() { return new CSystemExecutor(); }

public:
	void register_entity(CModel* _mode);
};
UNIQ CSystemExecutor::OBJECT_ID = 0;
}