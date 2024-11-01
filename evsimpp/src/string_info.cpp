#include "string_info.hpp"

namespace evsim
{
	StringInfo::StringInfo(std::string name) :m_name(name)
	{
		hash(m_name);
	}

	StringInfo::StringInfo(unsigned long hash, std::string name)
		: m_name(name), m_hash(hash)
	{

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

}
