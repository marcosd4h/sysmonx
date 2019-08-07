#pragma once

#include "common.h"

class ReportChannelLogfile : public ReportChannelBase
{
public:
	bool SendReportFinding(const SysmonXTypes::ReportObject &reportData);
	bool Initialize();

	ReportChannelLogfile() :
		ReportChannelBase(L"Logfile Report Channel", SysmonXTypes::ReportChannelID::REPORT_CHANNEL_LOGFILE),
		m_logger(TraceHelpers::Logger::Instance()),
		m_config(ConfigManager::Instance()) {}

private:
	TraceHelpers::Logger& m_logger;
	ConfigManager& m_config;
};
