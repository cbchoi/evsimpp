#pragma once

#include "message.hpp"
#include "sim_config.hpp"
#include "executor_factory.hpp"
#include "system_executor.hpp"

#include "model_dummy_coupled.hpp"
#include "model_skeleton_coupled.hpp"
#include "model_buffer.hpp"

#include "gtest/gtest.h"

namespace evsim {
    // The fixture for testing class Foo.
    class LoadTest : public testing::Test {
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        LoadTest()
        {
            // You can do set-up work for each test here.
            sim_config.engine_name = "evsimpp_first";
            sim_config.execution_mode = BLOCKING;
            sim_config.simulation_mode = VIRTUAL;
            sim_config.ef = new CExecutorFactory();
            sim_config.time_resolution = 1;
        }

        ~LoadTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            se = CSystemExecutor::create_system_executor(sim_config);
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
            delete se;
        }

        // Class members declared here can be used by all tests in the test suite
        SimConfig sim_config;
        CSystemExecutor* se;
    };

    CSkeletonCoupledModel* recursive_hierarchical_construction(int depth)
    {
	    if (depth == 1)
	    {
            std::stringstream sstream;
            sstream << "model_" << depth;

            CBuffer* pModel = new CBuffer("buffer");
            CSkeletonCoupledModel* pCoupled = new CSkeletonCoupledModel(sstream.str());
            Port& sc_input = pCoupled->create_input_port("sc_input");
            Port& sc_output = pCoupled->create_output_port("sc_output");

            pCoupled->insert_model(pModel);
            pCoupled->insert_coupling(pCoupled, sc_input, pModel, pModel->input);
            pCoupled->insert_coupling(pModel, pModel->output, pCoupled, sc_output);

            return pCoupled;
	    }
        else
        {
            std::stringstream sstream;
            sstream << "model_" << depth;

            CSkeletonCoupledModel* pModel = recursive_hierarchical_construction(depth - 1);

            CSkeletonCoupledModel* pCoupled = new CSkeletonCoupledModel(sstream.str());
            Port& sc_input = pCoupled->create_input_port("sc_input");
            Port& sc_output = pCoupled->create_output_port("sc_output");

            pCoupled->insert_model(pModel);
            pCoupled->insert_coupling(pCoupled, sc_input, pModel, *pModel->find_dyn_in_port("sc_input"));
            pCoupled->insert_coupling(pModel, *pModel->find_dyn_out_port("sc_output"), pCoupled, sc_output);
            return pCoupled;
        }
    }

    TEST_F(LoadTest, test_vertical_load_test)
    {
        Port& input = se->create_input_port("one");
        Port& output = se->create_output_port("output");

        CWaitGEN* pModel1 = new CWaitGEN("Model1");
        se->register_entity(pModel1, 0, Infinity);
        se->insert_coupling(se, input, pModel1, pModel1->input);
        

        CCoupledModel* sc = recursive_hierarchical_construction(500);
        se->register_entity(sc, 0, Infinity);
        se->insert_coupling(pModel1, pModel1->output, sc, *sc->find_dyn_in_port("sc_input"));
        se->insert_coupling(sc, *sc->find_dyn_out_port("sc_output"), se, output);

    	Message msg1 = se->create_message(input);
        se->insert_external_event(msg1);

        se->simulate(10);
        EXPECT_EQ(se->get_external_output_deliverer().get_contents().size(), 10);
    }

    TEST_F(LoadTest, test_horizontal_load_test)
    {
        Port& input = se->create_input_port("one");
        Port& output = se->create_output_port("output");

        CWaitGEN* pModel1 = new CWaitGEN("Model1");
        se->register_entity(pModel1, 0, Infinity);
        se->insert_coupling(se, input, pModel1, pModel1->input);


        for(int i = 0; i < 100; i++)
        {
            std::stringstream sstream;
            sstream << i;
            CBuffer* sc = new CBuffer(sstream.str());
            se->register_entity(sc, 0, Infinity);
            se->insert_coupling(pModel1, pModel1->output, sc, sc->input);
            se->insert_coupling(sc, sc->output, se, output);
        }

        Message msg1 = se->create_message(input);
        se->insert_external_event(msg1);

        se->simulate(100);
        EXPECT_EQ(se->get_external_output_deliverer().get_contents().size(), 10000);
    }

}

