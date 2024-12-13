#include "gtest/gtest.h"
#include "libevsimpp.hpp"
#include "test_atomic.hpp"
#include "test_coupled.hpp"
#include "test_sim_algorithm.hpp"


using namespace evsim;

int main(int argc, char** argv)
{// Clinet Code  

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    return 0;
}
