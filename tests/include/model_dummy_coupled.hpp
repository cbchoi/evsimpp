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
		//REGISTER_INPUT_PORT(three);

		CWaitGEN* pModel1 = new CWaitGEN("Model1");
		CWaitGEN* pModel2 = new CWaitGEN("Model2");
		//CWaitGEN* pModel3 = new CWaitGEN("Model3");

		insert_model(pModel1);
		insert_model(pModel2);
		//insert_model(pModel3);

		insert_coupling(this, one, pModel1, pModel1->input);
		insert_coupling(this, two, pModel2, pModel2->input);
		//insert_coupling(this,three, pModel3, pModel3->input);
	}

public:
	DECLARE_INPUT_PORT(one);
	DECLARE_INPUT_PORT(two);
	//DECLARE_INPUT_PORT(three);
};
