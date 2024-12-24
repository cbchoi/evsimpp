#pragma once

#include "coupled_model.hpp"
#include "executor.hpp"
#include "message_deliverer.hpp"
#include "string_info.hpp"
#include "executor_factory.hpp"

namespace evsim {

	class CHierarchicalCoupled:public iExecutor
	{
	public:
		CHierarchicalCoupled(Model pbehavior, CExecutorFactory* ef, iExecutor* engine, Time creation_t, Time current_t);
		virtual ~CHierarchicalCoupled();

	public:
		virtual void external_transition(const Port& _port, Message& msg);
		virtual void internal_transition();
		virtual void output_function(MessageDeliverer& msg);
		virtual Time time_advance();

		virtual void route_message(coupling_relation& cr, Message& msg);
	public:
		void set_req_time(Time global_time);
		Time get_req_time();

	protected:
		Time next_event_t; // Next event time
		Time request_t;  // Request time initialized to infinity

		CCoupledModel* behavior_object;
		CExecutorFactory* m_ef;

	protected:
		std::multiset<executor_item>		m_schedule_list;
		std::multiset<create_constraint>	m_wait_object_list;
		std::multiset<destory_constraint>	m_live_model_list;
		std::map<CModel*, IExecutor>		m_model_executor_map;
	};
}