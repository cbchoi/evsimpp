#pragma once

#include "model_dummy_coupled.hpp"
#include "gtest/gtest.h"

namespace evsim {
    // The fixture for testing class Foo.
    class StructureTest : public testing::Test {
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        StructureTest()
            :p1("one"), p2("two"), p3("three")
        {
            // You can do set-up work for each test here.
        }

        ~StructureTest() override {
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
        port p1;
        port p2;
        port p3;
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

    TEST_F(StructureTest, test_coupling)
    {
        
    }
}

