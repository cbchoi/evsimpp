#pragma once

#include "message.hpp"
#include "sim_config.hpp"
#include "executor_factory.hpp"
#include "system_executor.hpp"

#include "model_dummy_coupled.hpp"
#include "gtest/gtest.h"

namespace evsim {
    // The fixture for testing class Foo.
    class StructureTest : public testing::Test {
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        StructureTest()
        {
            // You can do set-up work for each test here.
            sim_config.engine_name = "evsimpp_first";
            sim_config.execution_mode = BLOCKING;
            sim_config.simulation_mode = VIRTUAL;
            sim_config.ef = new CExecutorFactory();
            sim_config.time_resolution = 1;
        }

        ~StructureTest() override {
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

    TEST_F(StructureTest, test_port)
    {
        CDummyCoupled da = CDummyCoupled("da");
        {
            int before = 0;
            {
                const std::set<port>::iterator iter = da.in_port().find(port("two"));
                ASSERT_NE(iter, da.in_port().end());
                EXPECT_EQ(iter->m_name, "two");
            }
            int after = 0;
        }

        {
            const std::set<port>::iterator iter = da.in_port().find(port("four"));
            EXPECT_EQ(iter, da.in_port().end());
        }
    }

    TEST_F(StructureTest, test_model)
    {
        CDummyCoupled cdc("DummyCoupled");
        ASSERT_NE(cdc.find_model("Model1"), nullptr);
        EXPECT_EQ((*cdc.find_model("Model1")).get_name(), StringInfo("Model1"));
    }

    TEST_F(StructureTest, test_external_input_coupling)
    {
        CDummyCoupled* da = new CDummyCoupled("da");
        se->register_entity(da, 0, Infinity);

        port& input1 = se->create_input_port("one");
        port& input2 = se->create_input_port("two");

        se->insert_coupling(se, input1, da, da->one);
        se->insert_coupling(se, input2, da, da->two);

        Message msg1 = se->create_message(input1);
        se->insert_external_event(msg1);

        Message msg2 = se->create_message(input2, 5);
        se->insert_external_event(msg2);

        se->simulate(10);
        EXPECT_EQ(std::dynamic_pointer_cast<CWaitGEN>(da->find_model("Model1"))->elem_count, 10);
        EXPECT_EQ(std::dynamic_pointer_cast<CWaitGEN>(da->find_model("Model2"))->elem_count, 5);
    }

#if 0
    TEST_F(StructureTest, test_external_output_coupling)
    {
        /**/
        CDummyCoupled* dc = new CDummyCoupled("dc");
        CDummyAtomic* da = new CDummyAtomic("da");
        se->register_entity(dc, 0, Infinity);
        se->register_entity(da, 0, Infinity);

        port& input = se->create_input_port("one");

        se->insert_coupling(se, input, dc, dc->one);
        se->insert_coupling(dc, dc->output, da, da->one);

        Message msg1 = se->create_message(input);
        se->insert_external_event(msg1);

        se->simulate(10);
        EXPECT_EQ(std::dynamic_pointer_cast<CWaitGEN>(dc->find_model("Model1"))->elem_count, 10);
    }

    TEST_F(StructureTest, test_internal_coupling)
    {
        CDummyCoupled* da = new CDummyCoupled("da");
        se->register_entity(da, 0, Infinity);

        port& input1 = se->create_input_port("one");
        port& input2 = se->create_input_port("two");

        se->insert_coupling(se, input1, da, da->one);
        //se->insert_coupling(se, input2, da, da->two);

        Message msg1 = se->create_message(input1);
        se->insert_external_event(msg1);
        Message msg2 = se->create_message(input2, 5);
        //se->insert_external_event(msg2);

        se->simulate(10);
        EXPECT_EQ(std::dynamic_pointer_cast<CWaitGEN>(da->find_model("Model1"))->elem_count, 10);
        //EXPECT_EQ(std::dynamic_pointer_cast<CWaitGEN>(da->find_model("Model2"))->elem_count, 5);
    }
#endif
}

