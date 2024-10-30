#include "init_manager.h"

InitManager::InitManager()
{
}

InitManager::InitManager(std::string path)
{
	doc.LoadFile(path.c_str());

	double x, y;
	{
		tinyxml2::XMLElement* agListElem = doc.FirstChildElement("scenario")->FirstChildElement("AgentList");
		for (tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
		     agElem != nullptr; agElem = agElem->NextSiblingElement())
		{
			agElem->QueryDoubleAttribute("x", &x);
			agElem->QueryDoubleAttribute("y", &y);

			CAgent* ag = new CAgent(x, y, 0, 1, 0);
			m_agentList.push_back(ag);
		}
	}
}

std::vector<CAgent*>& InitManager::get_agent_list()
{
	return m_agentList;
}
