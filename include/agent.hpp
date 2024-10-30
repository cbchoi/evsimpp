#pragma once

#include <iostream>
#include "definition.hpp"

namespace evsim{
class CAgent
{
protected:
    CAgent(){}
public:
    CAgent(double x, double y, double heading, double speed, double drange);
    virtual ~CAgent();

public:
    virtual void maneuver(double time);
    virtual bool detect(CAgent* pTarget);

public:
    virtual const Pos& getPosition() const;
    virtual unsigned long getAgentID() const;
    virtual double getSpeed() const;
    virtual double getHeading() const;
    virtual double getRange() const;

private:
    unsigned long m_agent_id;
    Pos    m_position;  
    double m_heading;
    double m_speed;
    double m_detect_range;

private:
    static unsigned long AGENT_ID;

public:
    virtual std::string to_string() const;
public:
    friend std::ostream& operator<<(std::ostream& os, const CAgent& ag);
};

}