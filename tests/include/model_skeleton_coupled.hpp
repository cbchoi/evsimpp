#pragma once

#include <string>
#include <iostream>

#include "skeleton_coupled_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "message_deliverer.hpp"

#include "model_wait_and_go.hpp"

using namespace evsim;

class CDummySkeletonCoupled :public CSkeletonCoupledModel
{
public:
	CDummySkeletonCoupled(std::string name) :evsim::CSkeletonCoupledModel(name)
	{

	}
};
