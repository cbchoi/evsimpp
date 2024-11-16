#pragma once

#include "object.hpp"
#include "model.hpp"

namespace evsim
{
	class CAtomicModel;
	class CSystemExecutor;

	class Message:public CObject
	{
	private:
		Message(CModel* src, Port& port, Time _time = 0);
	public:
		virtual ~Message() = default;
	public:
		virtual CModel* get_source() const { return msg_source; }
		virtual Port* get_out_port() const { return out_port; }
		virtual Time get_scheduled_time() const { return scheduled_time; }

	protected:
		CModel* msg_source;
		Port*	out_port;

		Time	scheduled_time;
		static UNIQ OBJECT_ID;

		friend CAtomicModel;
		friend CSystemExecutor;
	public:
		bool operator<(const Message& rhs) const;
	};
}