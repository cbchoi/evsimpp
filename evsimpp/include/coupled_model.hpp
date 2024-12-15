#pragma once

#include <string>
#include <map>
#include <vector>

#include "definition.hpp"
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

		void insert_model(CModel* pModel);

	public: // Utility Functions
		Model find_model(std::string name);
		std::map<StringInfo, Model>& get_models();

		std::map<coupling_relation, std::vector<coupling_relation>>& get_couplings();
	protected:
		std::map<StringInfo, Model> internal_models;
		std::map<coupling_relation, std::vector<coupling_relation>> m_coupling_map;
		Time next_event_t;
	};

}