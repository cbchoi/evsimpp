#include "message_delivery.hpp"

namespace evsim
{
	void MessageDelivery::insert_message(Message* msg)
	{// TODO: consider to use smart pointer
		m_data_list.push_back(msg);
	}
}
