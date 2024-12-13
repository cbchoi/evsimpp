#pragma once

#include <string>

#include "model.hpp"
#include "message_deliverer.hpp"

namespace evsim {

	class CCoupledModel : public CModel
	{
	public:
		CCoupledModel(std::string name);
		CCoupledModel(const CCoupledModel& rhs);
		virtual ~CCoupledModel();

	public:
		void insert_coupling(CModel* p_src, port& src_port,
			CModel* p_dst, port& dst_port);

	public: // Utility Functions
		

	protected:
		
	};

}