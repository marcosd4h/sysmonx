#pragma once

#include "common.h"

class ReportChannelETW : public ReportChannelBase
{
public:
	bool SendReportFinding(const SysmonXTypes::ReportObject &reportData);
	bool Initialize();

	ReportChannelETW() :
		ReportChannelBase(L"ETW Report Channel", SysmonXTypes::ReportChannelID::REPORT_CHANNEL_ETW) {}

private:
	TraceHelpers::Logger& m_logger = TraceHelpers::Logger::Instance();
	ConfigManager& m_config = ConfigManager::Instance();
};
