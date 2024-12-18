#pragma once

#include <string>
#include <iostream>

#include "coupled_model.hpp"
#include "definition.hpp"
#include "state.hpp"
#include "port.hpp"
#include "message_deliverer.hpp"

#include "model_wait_and_go.hpp"

using namespace evsim;

class CDummyCoupled :public CCoupledModel
{
public:
	CDummyCoupled(std::string name) :evsim::CCoupledModel(name)
	{
		REGISTER_INPUT_PORT(one);
		REGISTER_INPUT_PORT(two);
		REGISTER_OUTPUT_PORT(output);

		CWaitGEN* pModel1 = new CWaitGEN("Model1");
		CWaitGEN* pModel2 = new CWaitGEN("Model2");

		insert_model(pModel1);
		insert_model(pModel2);

		insert_coupling(this, one, pModel1, pModel1->input);
		insert_coupling(this, two, pModel2, pModel2->input);
		insert_coupling(pModel1, pModel1->output, this, output);
		insert_coupling(pModel2, pModel2->output, this, output);
	}

public:
	DECLARE_INPUT_PORT(one);
	DECLARE_INPUT_PORT(two);
	DECLARE_OUTPUT_PORT(output);
};
