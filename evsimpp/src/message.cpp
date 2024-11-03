#include "message.hpp"

namespace evsim
{
	Message::Message(CModel* src, Port& port)
		:msg_source(src), out_port(&port)
	{

	}
}
