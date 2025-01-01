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
	};

}