#pragma once

#include "message.hpp"
#include <vector>

namespace evsim
{
	class MessageDelivery
	{
	public:
		MessageDelivery() {}

		void insert_message(Message* msg);
		bool has_contents() const { return m_data_list.empty(); }
		std::vector<Message*>& get_contents() { return m_data_list; }

	private:
		std::vector<Message*> m_data_list;
	};
}
