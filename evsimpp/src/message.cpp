#include "message.hpp"

namespace evsim
{
	Message::Message(CModel* src, Port& port, Time _time)
		:CObject(OBJECT_ID, ENGINE_TYPE|MESSAGE_TYPE),
		 msg_source(src), out_port(&port),
		 scheduled_time(_time)
	{

	}

	bool Message::operator<(const Message& rhs) const
	{
		return scheduled_time < rhs.scheduled_time ? true : false;
	}

	UNIQ Message::OBJECT_ID = 0;
}
