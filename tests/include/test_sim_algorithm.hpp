#include "system_executor.hpp"
#include "periodic_event_generator.hpp"
#include "msg_processor.hpp"

#include "sim_config.hpp"
#include "executor_factory.hpp"
#include "gtest/gtest.h"


namespace evsim {
    // The fixture for testing class Foo.
    class SimAlgorithm : public testing::Test {
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        SimAlgorithm()
        {
            // You can do set-up work for each test here.
            sim_config.engine_name = "evsimpp_first";
            sim_config.execution_mode = BLOCKING;
            sim_config.simulation_mode = VIRTUAL;
            sim_config.ef = new CExecutorFactory();
            sim_config.time_resolution = 1;
        }

        ~SimAlgorithm() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Class members declared here can be used by all tests in the test suite
        SimConfig sim_config;
    };

    TEST_F(SimAlgorithm, single_model)
    {
        CSystemExecutor* se = CSystemExecutor::create_system_executor(sim_config);

        CPEG* pPEG = new CPEG("dummy");
        se->register_entity(pPEG, 0, Infinity);
        State state("IDLE3", 2);
        pPEG->set_current_state(&state);
        se->simulate(4);
        EXPECT_EQ(pPEG->elem_count, 2);
    }

    TEST_F(SimAlgorithm, multiple_model)
    {
        CSystemExecutor* se = CSystemExecutor::create_system_executor(sim_config);

        CPEG* pPEG1 = new CPEG("dummy1|");
        se->register_entity(pPEG1, 0, Infinity);
        State state1("IDLE3", 1);
        pPEG1->set_current_state(&state1);

    	CPEG* pPEG2 = new CPEG("dummy2|");
        State state2("IDLE3", 2);
    	se->register_entity(pPEG2, 0, Infinity);
        pPEG2->set_current_state(&state2);

        se->simulate(10);
        EXPECT_EQ(pPEG1->elem_count, 10);
        EXPECT_EQ(pPEG2->elem_count, 5);
    }

    TEST_F(SimAlgorithm, coupling)
    {
        CSystemExecutor* se = CSystemExecutor::create_system_executor(sim_config);

        CPEG* pPEG = new CPEG("dummy1|");
        se->register_entity(pPEG, 0, Infinity);
        State state1("IDLE3", 1);
        pPEG->set_current_state(&state1);

        CProcessor* pProcessor = new CProcessor("proc");
        se->register_entity(pProcessor, 0, Infinity);

        se->insert_coupling(pPEG, pPEG->output, pProcessor, pProcessor->input);

        se->simulate(10);
    }

    TEST_F(SimAlgorithm, output_handling)
    {
        CSystemExecutor* se = CSystemExecutor::create_system_executor(sim_config);

        CPEG* pPEG = new CPEG("dummy1|");
        se->register_entity(pPEG, 0, Infinity);
        State state1("IDLE3", 1);
        pPEG->set_current_state(&state1);

        CProcessor* pProc1 = new CProcessor("proc");
        se->register_entity(pProc1, 0, Infinity);

        CProcessor* pProc2 = new CProcessor("proc");
        se->register_entity(pProc2, 0, Infinity);

        CProcessor* pProc3 = new CProcessor("proc");
        se->register_entity(pProc3, 0, Infinity);

        se->insert_coupling(pPEG, pPEG->output, pProc1, pProc1->input);
        se->insert_coupling(pPEG, pPEG->output, pProc2, pProc2->input);
        se->insert_coupling(pPEG, pPEG->output, pProc3, pProc3->input);

        se->simulate(10);

        EXPECT_EQ(pProc1->elem_count, 10);
        EXPECT_EQ(pProc2->elem_count, 10);
        EXPECT_EQ(pProc3->elem_count, 10);
    }
}
