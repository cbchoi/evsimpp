#include "message_deliverer.hpp"

namespace evsim
{
	MessageDeliverer::~MessageDeliverer()
	{
		m_data_list.clear();
	}

	void MessageDeliverer::insert_message(Message msg)
	{// TODO: consider to use smart pointer
		m_data_list.insert(msg);
	}
}
