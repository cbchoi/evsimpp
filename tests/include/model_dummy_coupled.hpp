#pragma once

#include <string>
#include <iostream>

#include "coupled_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "message_deliverer.hpp"

#include "model_dummy_atomic.hpp"

using namespace evsim;

class CDummyCoupled :public CCoupledModel
{
public:
	CDummyCoupled(std::string name) :evsim::CCoupledModel(name)
	{
		REGISTER_INPUT_PORT(one);
		REGISTER_INPUT_PORT(two);
		REGISTER_INPUT_PORT(three);

		CDummyAtomic* pModel1 = new CDummyAtomic("Model1");
		CDummyAtomic* pModel2 = new CDummyAtomic("Model2");
		CDummyAtomic* pModel3 = new CDummyAtomic("Model3");

		insert_model(pModel1);
		insert_model(pModel2);
		insert_model(pModel3);
	}

public:
	DECLARE_INPUT_PORT(one);
	DECLARE_INPUT_PORT(two);
	DECLARE_INPUT_PORT(three);
};
