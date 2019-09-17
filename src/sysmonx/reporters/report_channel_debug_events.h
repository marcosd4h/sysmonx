#pragma once

#include "common.h"

class ReportChannelDebugEvents : public ReportChannelBase
{
public:
	bool SendReportFinding(const SysmonXTypes::ReportObject &reportData);
	bool Initialize();

	ReportChannelDebugEvents() :
		ReportChannelBase(L"Debug Events Report Channel", SysmonXTypes::ReportChannelID::REPORT_CHANNEL_DEBUG_EVENTS) {}

private:
	TraceHelpers::Logger& m_logger = TraceHelpers::Logger::Instance();
	ConfigManager& m_config = ConfigManager::Instance();
};
