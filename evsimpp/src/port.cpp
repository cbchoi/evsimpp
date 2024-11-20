#include "port.hpp"

#include "string_info.hpp"

namespace evsim
{
	port::port(std::string name) :StringInfo(name)
	{
	}

	port::port(unsigned long hash, std::string name)
		: StringInfo(hash, name)
	{

	}

	port::~port()
	{
		
	}

}
