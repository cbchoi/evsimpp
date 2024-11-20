#include "string_info.hpp"

#include <iostream>

//#include "../../out/build/x64-Debug/_deps/googletest-src/googletest/include/gtest/internal/gtest-string.h"

namespace evsim
{
	StringInfo::StringInfo(std::string name)
	:CObject(OBJECT_ID++, ENGINE_TYPE), m_name(name)
	{
		hash(m_name);

		//std::cout << "C" << obj_id() << std::endl;
	}

	StringInfo::StringInfo(unsigned long hash, std::string name)
		: CObject(OBJECT_ID++, ENGINE_TYPE), m_name(name), m_hash(hash)
	{

	}

	StringInfo::~StringInfo()
	{
		//std::cout << "D" << obj_id() << std::endl;
	}


	StringInfo::operator unsigned long() const
	{
		return m_hash;
	}

	StringInfo::operator std::string() const
	{
		return m_name;
	}
	
	void StringInfo::hash(const std::string& str)
	{
		m_hash = 2166136261u;
		for (char c : str)
		{
			m_hash ^= c; m_hash *= 16777619;
		}
	}

	bool StringInfo::operator==(const StringInfo& rhs) const
	{
		return rhs.m_hash == m_hash ? true : false;
	}

	UNIQ StringInfo::OBJECT_ID = 0;
}
