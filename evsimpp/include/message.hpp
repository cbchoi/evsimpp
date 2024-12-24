#pragma once

#include "object.hpp"
//#include "p_model.hpp"

namespace evsim
{
	class CModel;
	class CAtomicModel;
	class CSystemExecutor;

	class message :public CObject
	{
	private:
		message(CModel* src, Port& port, Time _time = 0);
	public:
		virtual ~message() = default;
	public:
		virtual CModel* get_source() const { return msg_source; }
		virtual const Port* get_out_port() const { return out_port; }
		virtual Time get_scheduled_time() const { return scheduled_time; }

		virtual void set_source(CModel* model) { msg_source = model; }
		virtual void set_port(const Port* port) { out_port = port; }

	protected:
		CModel* msg_source;
		const Port*	out_port;

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