#include "message_deliverer.hpp"

namespace evsim
{
	void MessageDeliverer::insert_message(Message msg)
	{// TODO: consider to use smart pointer
		m_data_list.push_back(msg);
	}
}
