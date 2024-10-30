#pragma once

#include "definition.hpp"

class CObject
{
public:
	CObject(UNIQ_TYPE _id, UNIQ_TYPE _type) { m_id = _id; m_type = _type; }

public:
	UNIQ_TYPE get_object_id()   const { return m_id; }
	UNIQ_TYPE get_object_type() const { return m_type; }

private:
	UNIQ_TYPE m_id;
	UNIQ_TYPE m_type;
};
