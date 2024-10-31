#pragma once

#include "definition.hpp"
#include <sstream>
#include <string>

namespace evsim{
class CObject
{
public:
	CObject(UNIQ _id, UNIQ _type) { m_id = _id; m_type = _type; }

public:
	UNIQ obj_id()   const { return m_id; }
	UNIQ obj_type() const { return m_type; }

public:
	
private:
	UNIQ m_id;
	UNIQ m_type;
};
}