#include <iostream>
#include <string>
#include <vector>

#include "agent.hpp"
#include "mmanager.hpp"

#include "system_executor.hpp"
//#include "special_agent.hpp"

#include "init_manager.h"
int main(int argc, char** argv)
{// Clinet Code  

    // XMLElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
    // XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement( "attributeApproach" );
    // attributeApproachElement->QueryIntAttribute( "v", &v0 );
    evsim::CSystemExecutor* sys = evsim::CSystemExecutor::create_system_executor();
    evsim::InitManager init_manager("test.xml");

    /* Simulation Engine */
    evsim::CManeuverManager mmanager;

    for (std::vector<evsim::CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter)
        mmanager.register_publisher(*iter);

    /*
    CAgent* p_agent1 = new CAgent(10,  0, 0, 1, 10);
    p_agent1 = new StochasticDecorator(p_agent1);
    p_agent1 = new SimpleDecorator(p_agent1);

    mmanager.register_publisher(p_agent1);

    CAgent* p_agent2 = new CAgent(10,  30, 180, 1, 10);
    mmanager.register_publisher(p_agent2);
    */

    /* Simulation Engine Initialization */
    double sim_time;
    double time_step = 1;

    for (sim_time = 0; sim_time < 30; sim_time += time_step)
    {
        //p_agent1->maneuver(time_step);
        //p_agent2->maneuver(time_step);

        //p_agent1->detect(p_agent2);
        //p_agent2->detect(p_agent1);

        mmanager.svc(time_step);

        std::cout << "----" << std::endl;
        for (std::vector<evsim::CAgent*>::iterator iter = init_manager.get_agent_list().begin();
            iter != init_manager.get_agent_list().end(); ++iter)
            std::cout << "Time: " << sim_time << ", " << **iter << std::endl;
        // (*iter) => CAgent*
        // *(*iter) => CAgent
    }
    evsim::UNIQ val = 0x1000000000000000;
    std::cout << std::hex <<val << std::endl;

    return 0;
}
