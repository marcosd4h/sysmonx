#pragma once

#include "common.h"

class MatchEventPowershell : public MatchEventBase
{
public:
	bool Initialize()
	{
		bool ret = false;

		m_properties.push_back(EventPropertyID::EVENT_PROPERTY_ID_ScriptBlockText);

		if (!m_properties.empty())
		{
			ret = true;
		}

		return ret;
	}


	MatchEventPowershell() :
		MatchEventBase(SysmonXTypes::EventID::SECURITY_EVENT_ID_SYSMONX_POWERSHELL_START_COMMAND, L"SECURITY_EVENT_ID_SYSMONX_POWERSHELL_START_COMMAND") {}

};