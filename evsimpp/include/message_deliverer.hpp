#pragma once

#include <set>
#include "message.hpp"
#include "definition.hpp"

namespace evsim
{
	class MessageDeliverer
	{
	public:
		MessageDeliverer() {}
		virtual ~MessageDeliverer();

		void insert_message(Message msg);
		bool has_contents() const { return !m_data_list.empty(); }
		std::multiset<Message>& get_contents() { return m_data_list; }
		Time get_first_event_time() const
		{
			return m_data_list.empty() ? Infinity : (*m_data_list.begin())->get_scheduled_time();
		}

	private:
		std::multiset<Message> m_data_list;
	};
}
