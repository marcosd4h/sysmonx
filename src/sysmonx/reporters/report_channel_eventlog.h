#pragma once

#include "common.h"

class ReportChannelEventlog : public ReportChannelBase
{
public:
	bool SendReportFinding(const SysmonXTypes::ReportObject &reportData);
	bool Initialize();

	ReportChannelEventlog() :
		ReportChannelBase(L"EventLog Report Channel", SysmonXTypes::ReportChannelID::REPORT_CHANNEL_EVENTLOG),
		m_logger(TraceHelpers::Logger::Instance()),
		m_config(ConfigManager::Instance()) {}

private:
	TraceHelpers::Logger& m_logger;
	ConfigManager& m_config;
};
