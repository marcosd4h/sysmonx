#pragma once

#include "common.h"

class MatchEventProcessCreate : public MatchEventBase
{
public:
	bool Initialize()
	{
		bool ret = false;

		//Common fields (TODO: mjo define if we are matching against these fields or not)
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventID);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventCollectorTechID);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventCollectorVectorID);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventCreationTimestamp);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderID);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderName);

		//Event specific attributes
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_RuleName);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_UtcTime);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_ProcessGuid);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_ProcessId);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_Image);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_FileVersion);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_Description);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_Product);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_Company);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_OriginalFileName);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_CommandLine);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_CurrentDirectory);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_User);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_LogonGuid);
		//m_properties.push_back(EventPropertyID::EVENT_PROPERTY_LogonId);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_TerminalSessionId);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_IntegrityLevel);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_Hashes);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_ParentProcessGuid);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_ParentProcessId);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_ParentImage);
		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_ParentCommandLine);

		if (!m_properties.empty())
		{
			ret = true;
		}

		return ret;
	}


	MatchEventProcessCreate() :
		MatchEventBase(SysmonXTypes::EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS, L"SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS") {}

};