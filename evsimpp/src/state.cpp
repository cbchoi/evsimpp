#include "state.hpp"

namespace evsim
{
	State::State()
		:m_name(""), m_deadline(-1)
	{
		m_hash = 0;
	}
	State::State(std::string name, Time deadline) :m_name(name), m_deadline(deadline)
	{
		hash(m_name);
	}

	State::State(unsigned long hash, std::string name, Time deadline)
		: m_name(name), m_hash(hash), m_deadline(deadline)
	{

	}

	State::operator unsigned long() const
	{
		return m_hash;
	}

	State::operator std::string() const
	{
		return m_name;
	}

	void State::hash(const std::string& str)
	{
		m_hash = 2166136261u;
		for (char c : str)
		{
			m_hash ^= c; m_hash *= 16777619;
		}
	}
}