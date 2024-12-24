#pragma once

#include <string>
#include <map>
#include <vector>

#include "definition.hpp"
#include "coupled_model.hpp"
#include "message_deliverer.hpp"

namespace evsim {

	class CSkeletonCoupledModel : public CCoupledModel
	{
	public:
		CSkeletonCoupledModel(std::string name);
		CSkeletonCoupledModel(const CCoupledModel& rhs);
		virtual ~CSkeletonCoupledModel();

	public:
		Port& create_input_port(std::string name);
		Port& create_output_port(std::string name);
		
	protected:
		std::map<StringInfo, Port*> m_skel_in_ports;
		std::map<StringInfo, Port*> m_skel_out_ports;
	};

}