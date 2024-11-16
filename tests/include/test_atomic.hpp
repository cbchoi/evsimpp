#pragma once

#include "model_dummy_atomic.hpp"
#include "gtest/gtest.h"

namespace evsim {
    // The fixture for testing class Foo.
    class AtomicTest : public testing::Test {
    protected:
        // You can remove any or all of the following functions if their bodies would
        // be empty.

        AtomicTest()
            :p1("one"), p2("two"), p3("three")
        {
            // You can do set-up work for each test here.
        }

        ~AtomicTest() override {
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
        Port p1; 
        Port p2; 
        Port p3; 
    };

    TEST_F(AtomicTest, test_port)
    {  
        CDummyAtomic da = CDummyAtomic("da");
        {
            const std::set<Port>::iterator iter = da.in_port().find(Port("two"));
            ASSERT_NE(iter, da.in_port().end());
            EXPECT_EQ(iter->m_name, "two");
        }

        {
            const std::set<Port>::iterator iter = da.in_port().find(Port("four"));
            EXPECT_EQ(iter, da.in_port().end());
        }
    }

    TEST_F(AtomicTest, test_ext_transitions)
    {
        CDummyAtomic da = CDummyAtomic("da");
        MessageDeliverer sm = MessageDeliverer();
        Port p1 = Port("two");
        da.external_transition(p1, sm);
        EXPECT_EQ(da.IDLE2.m_name, da.get_current_state().m_name);
    }

    TEST_F(AtomicTest, test_int_transitions)
    {
        CDummyAtomic da = CDummyAtomic("da");
        EXPECT_EQ(da.IDLE1.m_name, da.get_current_state().m_name);
        da.internal_transition();
        EXPECT_EQ(da.IDLE2.m_name, da.get_current_state().m_name);
    }
}

