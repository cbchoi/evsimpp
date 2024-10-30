#pragma once

#include <string>
#include <ostream>

#include "definition.hpp"
#include "object.hpp"

namespace evsim{
	class CModel : public CObject
	{
	public:
		CModel(UNIQ _id, UNIQ _type) :CObject(_id, MODEL_TYPE|_type) {}

public:
	const std::string& get_name() const { return m_model_name; }
	void set_name(const std::string& _name) { m_model_name = _name; }

private:
	std::string m_model_name;

protected:
	static UNIQ OBJECT_ID;
};

UNIQ CModel::OBJECT_ID = 0;

}