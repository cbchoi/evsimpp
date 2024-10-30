#pragma once

#include <map>

#include "definition.hpp"
#include "object.hpp"
#include "model.hpp"

namespace evsim{

class CSystemExecutor: public CObject
{
private:
	CSystemExecutor():CObject(CSystemExecutor::OBJECT_ID++, ENGINE_TYPE){}

public:
	static CSystemExecutor* create_system_executor() { return new CSystemExecutor(); }

public:
	void register_entity(CModel* _mode);

	//Time schedule(Time t);
protected:
	static UNIQ OBJECT_ID;
};

}