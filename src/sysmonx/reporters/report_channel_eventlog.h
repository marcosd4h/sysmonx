#pragma once

#include "common.h"

class ReportChannelEventlog : public ReportChannelBase
{
public:
	bool SendReportFinding(const SysmonXTypes::ReportObject &reportData);
	bool Initialize();

	ReportChannelEventlog() :
		ReportChannelBase(L"EventLog Report Channel", SysmonXTypes::ReportChannelID::REPORT_CHANNEL_EVENTLOG) {}

private:
	TraceHelpers::Logger& m_logger = TraceHelpers::Logger::Instance();
	ConfigManager& m_config = ConfigManager::Instance();

};
