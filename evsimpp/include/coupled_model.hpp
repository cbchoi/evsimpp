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
		void insert_coupling(CModel* p_src, Port& src_port,
			CModel* p_dst, Port& dst_port);

		void insert_external_input_coupling(Port& src_port, Port& dst_port);
		void insert_internal_coupling(Port& src_port, Port& dst_port);
		void insert_external_output_coupling(Port& src_port, Port& dst_port);

		void insert_model(CModel* pModel);
		void remove_model(CModel* pModel);

	public: // Utility Functions
		Model find_model(std::string name);
		std::map<StringInfo, Model>& get_models();

		Port* find_dyn_in_port(std::string name);
		Port* find_dyn_out_port(std::string name);

		std::map<coupling_relation, std::vector<coupling_relation>>& get_couplings();

		std::map<Port*, std::vector<Port*>>& get_eic();
		std::map<Port*, std::vector<Port*>>& get_ic();
		std::map<Port*, std::vector<Port*>>& get_eoc();

	protected:
		std::map<StringInfo, Model> internal_models;
		std::map<coupling_relation, std::vector<coupling_relation>> m_coupling_map;

		std::map<Port*, std::vector<Port*>> m_eic_map;
		std::map<Port*, std::vector<Port*>> m_ic_map;
		std::map<Port*, std::vector<Port*>> m_eoc_map;
		Time next_event_t;

	public: // Supporting Variable Structured DEVS
		Port& create_input_port(std::string name);
		Port& create_output_port(std::string name);

	protected:
		std::map<StringInfo, Port*> m_dynamic_in_ports;
		std::map<StringInfo, Port*> m_dynamic_out_ports;
	};

}