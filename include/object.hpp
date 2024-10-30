#pragma once

#include "definition.hpp"

namespace evsim{
class CObject
{
public:
	CObject(UNIQ _id, UNIQ _type) { m_id = _id; m_type = _type; }

public:
	UNIQ get_object_id()   const { return m_id; }
	UNIQ get_object_type() const { return m_type; }

private:
	UNIQ m_id;
	UNIQ m_type;
};
}