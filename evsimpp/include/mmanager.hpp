#pragma once
#include "agent.hpp"

#include <vector>

namespace evsim{
class CManeuverManager
{
public:
    CManeuverManager();
    virtual ~CManeuverManager();

public:
    void register_publisher(CAgent* p_agent);
    void unregister_publisher(CAgent* p_agent);
    
    void register_subscriber(CAgent* p_agent);
    void unregister_subscriber(CAgent* p_agent);

    bool is_subscriber(CAgent* p_agent);
    void svc(double time);
private:
    std::vector<CAgent*> m_publish_list;
    std::vector<CAgent*> m_subscriber_list;
};

}