#pragma once

#include "object.hpp"
#include "model.hpp"
#include <map>

class CSystemExecutor: public CObject
{
private:
	CSystemExecutor():CObject(OBJECT_ID++){}

public:
	static CSystemExecutor* create_system_executor() { return new CSystemExecutor(); }

public:
	void register_entity(CModel* _mode);
};