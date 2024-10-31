#include "port.hpp"

namespace evsim
{
	Port::Port(std::string name) :m_name(name)
	{
		hash(m_name);
	}

	Port::Port(unsigned long hash, std::string name)
		: m_name(name), m_hash(hash)
	{

	}

	Port::operator unsigned long() const
	{
		return m_hash;
	}

	Port::operator std::string() const
	{
		return m_name;
	}

	void Port::hash(const std::string& str)
	{
		m_hash = 2166136261u;
		for (char c : str)
		{
			m_hash ^= c; m_hash *= 16777619;
		}
	}
}
