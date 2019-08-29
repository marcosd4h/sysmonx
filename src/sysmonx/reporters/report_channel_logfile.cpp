#include "common.h"

bool ReportChannelLogfile::Initialize()
{
	bool ret = false;

	m_logger.Trace("ReportChannelLogfile::Initialize - About to initialize Logfile Report Channel processor");

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

bool ReportChannelLogfile::SendReportFinding(const SysmonXTypes::ReportObject &reportData)
{
	bool ret = false;

	if (IsInitialized() && reportData)
	{
		std::wstring dataToReport;
		dataToReport.append(L"\nEventID "); dataToReport.append(reportData->Event->EventID.GetWString()); dataToReport.append(L"\n");
		dataToReport.append(L"ProcessId "); dataToReport.append(reportData->Event->ProcessId.GetWString()); dataToReport.append(L"\n");
		dataToReport.append(L"CommandLine "); dataToReport.append(reportData->Event->CommandLine);  dataToReport.append(L"\n");
		dataToReport.append(L"Image "); dataToReport.append(reportData->Event->Image);  dataToReport.append(L"\n");
		dataToReport.append(L"ParentProcessId "); dataToReport.append(reportData->Event->ParentProcessId.GetWString()); dataToReport.append(L"\n");
		dataToReport.append(L"ParentCommandLine "); dataToReport.append(reportData->Event->ParentCommandLine);  dataToReport.append(L"\n");
		dataToReport.append(L"ParentImage "); dataToReport.append(reportData->Event->ParentImage);  dataToReport.append(L"\n");
		dataToReport.append(L"Scanner Process Hollowing "); dataToReport.append(reportData->Event->ScannerResult);  dataToReport.append(L"\n");

		OutputDebugStringW(dataToReport.c_str());
		//m_logger.TraceConsoleUp(GeneralHelpers::WStrToStr(dataToReport).c_str());
		ret = true;
	}

	return ret;
}