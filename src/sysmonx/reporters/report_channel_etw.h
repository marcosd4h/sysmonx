#pragma once

#include "common.h"

class ReportChannelETW : public ReportChannelBase
{
public:
	bool SendReportFinding(const SysmonXTypes::ReportObject &reportData);
	bool Initialize();

	ReportChannelETW() :
		ReportChannelBase(L"ETW Report Channel", SysmonXTypes::ReportChannelID::REPORT_CHANNEL_ETW),
		m_logger(TraceHelpers::Logger::Instance()),
		m_config(ConfigManager::Instance()) {}

private:
	TraceHelpers::Logger& m_logger;
	ConfigManager& m_config;
};
