#pragma once

#include <vector>
#include <string>
#include "agent.hpp"
#include "tinyxml2.h"

namespace evsim{
class InitManager
{
public:
    InitManager();

    InitManager(std::string path);

    std::vector<CAgent*>& get_agent_list();

private:
    tinyxml2::XMLDocument doc;
    std::vector<CAgent*> m_agentList;
};
}