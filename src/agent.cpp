#include <iostream>
#include <cmath>
#include <sstream>

#include "agent.hpp"

/* INITIALIZE STATIC FIELDS*/
unsigned long CAgent::AGENT_ID = 0;

CAgent::CAgent(double x, double y, double heading, double speed, double drange)
{
    m_agent_id = AGENT_ID++;

    m_position.x = x;
    m_position.y = y;

    m_heading = heading;
    m_speed = speed;
    m_detect_range = drange;
}

CAgent::~CAgent()
{

}

void CAgent::maneuver(double time)
{
    m_position.x += sin(DEG2RAD(m_heading)) * m_speed * time;
    m_position.y += cos(DEG2RAD(m_heading)) * m_speed * time;

}

bool CAgent::detect(CAgent* pTarget)
{
    bool detectFlag = false;
    std::cout << "\tAgentID: " << m_agent_id << " Detection Start" << std::endl;
    if(CALC_DIST(m_position, pTarget->getPosition()) <= m_detect_range)
    {
        std::cout << "\t>> Target Detected: " << pTarget->getAgentID() << std::endl; 
        detectFlag = true;
    }
    std::cout << "\tAgentID: " << m_agent_id << " Detection Ended" << std::endl;
    return detectFlag;
}

const Pos& CAgent::getPosition() const
{
    return m_position;
}

unsigned long CAgent::getAgentID() const
{
    return m_agent_id;
}

double CAgent::getSpeed() const
{
    return m_speed;
}

double CAgent::getHeading() const
{
    return m_heading;
}

double CAgent::getRange() const
{
    return m_detect_range;
}

std::string CAgent::to_string() const
{
    std::stringstream os;
    const Pos pos = getPosition();
    os << "Agent ID:" << getAgentID() << '(' << (long) pos.x << ", " << (long) pos.y << ")";
    os << " [S: " << getSpeed() << ", H: " << getHeading() << ", R: " << getRange()<< "]";
    return os.str();
}

std::ostream& operator<<( std::ostream& os, const CAgent& ag)
{
    os << ag.to_string();
    return os;
}

