#pragma once

#include <string>
#include <ostreams>

#include "definition.hpp"
#include "object.h"

class CModel : public CObject
{
public:
	CModel(UNIQ_TYPE _id):CObject(_id, MODEL_TYPE){}

public:
	const std::string& get_name() const { return m_model_name; }
	void set_name(const std::string& _name) { m_model_name = _name; }

private:
	std::string m_model_name;

protected:
	static UNIQ_TYPE OBJECT_ID;
};
UNIQ_TYPE CModel::OBJECT_ID = ENGINE_OBJECT;
