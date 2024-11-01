#include "port.hpp"

#include "string_info.hpp"

namespace evsim
{
	Port::Port(std::string name) :StringInfo(name)
	{
	}

	Port::Port(unsigned long hash, std::string name)
		: StringInfo(hash, name)
	{

	}

	Port::~Port()
	{
		
	}

}
