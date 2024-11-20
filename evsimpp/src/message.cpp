#include "message.hpp"

namespace evsim
{
	message::message(CModel* src, port& port, Time _time)
		:CObject(OBJECT_ID++, ENGINE_TYPE|MESSAGE_TYPE),
		 msg_source(src), out_port(&port),
		 scheduled_time(_time)
	{
		
	}

	bool message::operator<(const message& rhs) const
	{
		return scheduled_time < rhs.scheduled_time ? true : false;
	}

	UNIQ message::OBJECT_ID = 0;
}
