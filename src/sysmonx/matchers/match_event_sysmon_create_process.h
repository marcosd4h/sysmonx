#pragma once

#include "common.h"

class MatchEventProcessCreate : public MatchEventBase
{
public:
	bool Initialize()
	{
		bool ret = false;

		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventCollectorTechID);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderName);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderPID);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_CommandLine);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_Image);

		if (!m_properties.empty())
		{
			ret = true;
		}

		return ret;
	}


	MatchEventProcessCreate() :
		MatchEventBase(SysmonXTypes::EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS, L"SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS") {}

};