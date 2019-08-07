#include "common.h"

bool ReportChannelETW::Initialize()
{
	bool ret = false;

	m_logger.Trace("ReportChannelETW::Initialize() - About to initialize ETW report channel processor");

	if (m_config.IsInitialized() && !IsInitialized())
	{
		ReportChannelBase::SetInitializeStatus(true);
		ret = true;
	}
	else
	{
		ret = true;
	}

	return ret;
}

bool ReportChannelETW::SendReportFinding(const SysmonXTypes::ReportObject &reportData)
{
	bool ret = false;

	if (IsInitialized() && reportData)
	{
		m_logger.Trace("ReportChannelETW::SendReportFinding() - About to process a new event for ETW channel");

		ret = true;
	}

	return ret;
}