#include "common.h"

bool ReportChannelEventlog::Initialize()
{
	bool ret = false;

	m_logger.Trace("ReportChannelEventlog::Initialize() - About to initialize Eventlog Report Channel processor");

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

bool ReportChannelEventlog::SendReportFinding(const SysmonXTypes::ReportObject &reportData)
{
	bool ret = false;

	if (IsInitialized() && reportData)
	{
		m_logger.Trace("ReportChannelEventlog::SendReportFinding() - About to process a new event for Eventlog channel");
		EventObject& workingEvent = reportData->Event;

		if (workingEvent->EventID == EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS)
		{
			EventWriteProcessCreateSysmonX(workingEvent->RuleName.c_str(),
				workingEvent->UtcTime.c_str(),
				&workingEvent->ProcessGuid,
				workingEvent->ProcessID,
				workingEvent->Image.c_str(),
				workingEvent->FileVersion.c_str(),
				workingEvent->Description.c_str(),
				workingEvent->Product.c_str(),
				workingEvent->Company.c_str(),
				workingEvent->OriginalFileName.c_str(),
				workingEvent->CommandLine.c_str(),
				workingEvent->CurrentDirectory.c_str(),
				workingEvent->User.c_str(),
				&workingEvent->LogonGuid,
				//workingEvent->LogonId,
				0,
				workingEvent->TerminalSessionId,
				workingEvent->IntegrityLevel.c_str(),
				workingEvent->Hashes.c_str(),
				&workingEvent->ParentProcessGuid,
				workingEvent->ParentProcessId,
				workingEvent->ParentImage.c_str(),
				workingEvent->ParentCommandLine.c_str(),
				workingEvent->ScannerResult.c_str());
		}
		else if (workingEvent->EventID == EventID::SECURITY_EVENT_ID_SYSMONX_POWERSHELL_START_COMMAND)
		{
			EventWritePowershellEvents(workingEvent->FreeText.c_str(), workingEvent->ProcessID);
		}

		ret = true;
	}

	return ret;
}