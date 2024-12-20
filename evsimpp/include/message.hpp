#pragma once

#include "object.hpp"
#include "model.hpp"

namespace evsim
{
	class CAtomicModel;
	class CSystemExecutor;

	class message :public CObject
	{
	private:
		message(CModel* src, port& port, Time _time = 0);
	public:
		virtual ~message() = default;
	public:
		virtual CModel* get_source() const { return msg_source; }
		virtual port* get_out_port() const { return out_port; }
		virtual Time get_scheduled_time() const { return scheduled_time; }

	protected:
		CModel* msg_source;
		port*	out_port;

		Time	scheduled_time;
		static UNIQ OBJECT_ID;

		friend CAtomicModel;
		friend CSystemExecutor;
	public:
		bool operator<(const message& rhs) const;
	};

	typedef std::shared_ptr<message> Message;
	inline bool operator<(const Message& lhs, const Message& rhs) { return lhs.get()->get_scheduled_time() - rhs.get()->get_scheduled_time() < 0 ? true : false; }
}