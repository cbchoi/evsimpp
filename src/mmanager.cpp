#include "mmanager.hpp"

CManeuverManager::CManeuverManager()
{

}

CManeuverManager::~CManeuverManager()
{
    for(std::vector<CAgent*>::iterator iter = m_publish_list.begin();
        iter != m_publish_list.end(); ++iter)
        {
            delete *iter;
        }
}

void CManeuverManager::register_publisher(CAgent* p_agent)
{
    m_publish_list.push_back(p_agent);
}

void CManeuverManager::unregister_publisher(CAgent* p_agent)
{

}

void CManeuverManager::register_subscriber(CAgent* p_agent)
{
    m_subscriber_list.push_back(p_agent);
}

void CManeuverManager::unregister_subscriber(CAgent* p_agent)
{
    std::vector<CAgent*>::iterator iter = m_subscriber_list.begin();
    for( ;iter != m_subscriber_list.end(); ++iter)
    {
        if(*iter == p_agent)
            break;
    }

    if(iter != m_subscriber_list.end())
        m_subscriber_list.erase(iter);
}

bool CManeuverManager::is_subscriber(CAgent* p_agent)
{
    for(std::vector<CAgent*>::iterator iter = m_subscriber_list.begin();
        iter != m_subscriber_list.end(); ++iter)
        {
            if(*iter == p_agent)
                return true;
        }
    return false;
}

void CManeuverManager::svc(double time)
{
    Pos temp_pos;
    for(std::vector<CAgent*>::iterator iter = m_publish_list.begin();
        iter != m_publish_list.end(); ++iter)
        {
            (*iter)->maneuver(time);
            temp_pos = (*iter)->getPosition();
            if((temp_pos.x >= 10 && temp_pos.x <= 20) && 
                (temp_pos.y >= 10 && temp_pos.y <= 20) )
            {
                // Subscriber management
                if(!is_subscriber(*iter))
                    register_subscriber(*iter);

                for(std::vector<CAgent*>::iterator jter = m_subscriber_list.begin();
                    jter != m_subscriber_list.end(); ++jter)
                {
                    if((*iter) != (*jter))
                    {
                        (*jter)->detect((*iter));
                    }
                }
            }
            else
            {
                // Subscriber management
                if(is_subscriber(*iter))
                    unregister_subscriber(*iter);
            }
        }
}