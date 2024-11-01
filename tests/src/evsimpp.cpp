#include <iostream>
#include <string>
#include <vector>

#include "agent.hpp"
#include "mmanager.hpp"

#include "system_executor.hpp"
//#include "special_agent.hpp"

#include "init_manager.h"

#include "gtest/gtest.h"
#include "test_atomic.hpp"

using namespace evsim;

int main(int argc, char** argv)
{// Clinet Code  

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

#if 0
    evsim::CSystemExecutor* sys = evsim::CSystemExecutor::create_system_executor();
    evsim::InitManager init_manager("test.xml");

    /* Simulation Engine */
    evsim::CManeuverManager mmanager;

    for (std::vector<evsim::CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter)
        mmanager.register_publisher(*iter);


    /* Simulation Engine Initialization */
    double sim_time;
    double time_step = 1;

    for (sim_time = 0; sim_time < 30; sim_time += time_step)
    {

        mmanager.svc(time_step);

        std::cout << "----" << std::endl;
        for (std::vector<evsim::CAgent*>::iterator iter = init_manager.get_agent_list().begin();
            iter != init_manager.get_agent_list().end(); ++iter)
            std::cout << "Time: " << sim_time << ", " << **iter << std::endl;
    }

    evsim::Time t = 10;

    std::cout << (12.1 < evsim::Infinity) << std::endl;
#endif
    return 0;
}
