#pragma once

#include "model.hpp"
namespace evsim
{
	class Message
	{
	public:
		Message(CModel* src, Port& port);

		virtual CModel* get_source() const { return msg_source; }
		virtual Port* get_out_port() const { return out_port; }
	private:
		CModel* msg_source;
		Port*	out_port;
	};
}