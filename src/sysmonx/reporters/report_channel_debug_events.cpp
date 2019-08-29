#include "common.h"

bool ReportChannelDebugEvents::Initialize()
{
	bool ret = false;

	m_logger.Trace("ReportChannelDebugEvents::Initialize - About to initialize Debug Events Report Channel processor");

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

bool ReportChannelDebugEvents::SendReportFinding(const SysmonXTypes::ReportObject &reportData)
{
	bool ret = false;

	if (IsInitialized() && reportData)
	{
		std::wstring dataToReport;

		dataToReport.append(L"TEST_SYSMONX_EVENT");
		dataToReport.append(L"ProcessId "); dataToReport.append(reportData->Event->ProcessId.GetWString()); dataToReport.append(L"\n");
		dataToReport.append(L"CommandLine "); dataToReport.append(reportData->Event->CommandLine);  dataToReport.append(L"\n");
		dataToReport.append(L"Image "); dataToReport.append(reportData->Event->Image);  dataToReport.append(L"\n");
		dataToReport.append(L"ParentProcessId "); dataToReport.append(reportData->Event->ParentProcessId.GetWString()); dataToReport.append(L"\n");
		dataToReport.append(L"ParentCommandLine "); dataToReport.append(reportData->Event->ParentCommandLine);  dataToReport.append(L"\n");
		dataToReport.append(L"ParentImage "); dataToReport.append(reportData->Event->ParentImage);  dataToReport.append(L"\n");

		OutputDebugStringW(dataToReport.c_str());

		m_logger.Trace("ReportChannelDebugEvents::SendReportFinding - About to process a new event for Debug Events channel");

		ret = true;
	}

	return ret;
}