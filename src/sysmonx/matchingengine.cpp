#include "common.h"


void PerformProcessHollowingScan(SysmonXTypes::EventObject &data) 
{
	//Performing Process Hollowing checks here
	blackbone::Process process;
	blackbone::pe::PEImage mainModuleMemory;
	blackbone::pe::PEImage mainModuleDisk;
	WindowsTypes::BytePTR buffer = nullptr;
	DWORD processIDtoInspect = data->ProcessId;

	//Grabbing main module from memory and disk
	if (WindowsHelpers::GetBBProcessFromPID(processIDtoInspect, process) &&
		WindowsHelpers::GetMainModuleFromMemory(process, buffer, mainModuleMemory) &&
		WindowsHelpers::GetMainModuleFromDisk(process, mainModuleDisk))
	{
		const IMAGE_DOS_HEADER *pMainModuleMemoryDosHdr = nullptr;
		const IMAGE_DOS_HEADER *pMainModuleDiskDosHdr = nullptr;
		const IMAGE_SECTION_HEADER *pMainModuleMemorySection = nullptr;
		const IMAGE_SECTION_HEADER *pMainModuleDiskSection = nullptr;

		pMainModuleMemoryDosHdr = reinterpret_cast<const IMAGE_DOS_HEADER*>(mainModuleDisk.base());
		pMainModuleDiskDosHdr = reinterpret_cast<const IMAGE_DOS_HEADER*>((void *)buffer.get());

		//Cross checking PE fields of modules on memory vs counterpart on disk
		if (pMainModuleMemoryDosHdr &&	pMainModuleDiskDosHdr)
		{
			if ((pMainModuleMemoryDosHdr->e_magic == IMAGE_DOS_SIGNATURE) &&
				(pMainModuleDiskDosHdr->e_magic == IMAGE_DOS_SIGNATURE) &&
				(pMainModuleDiskDosHdr->e_oemid == pMainModuleMemoryDosHdr->e_oemid) &&
				(mainModuleMemory.imageSize() == mainModuleDisk.imageSize()) &&
				(mainModuleMemory.DirectorySize(0) == mainModuleDisk.DirectorySize(0)) &&
				(mainModuleMemory.mType() == mainModuleDisk.mType()) &&
				(mainModuleMemory.headersSize() == mainModuleDisk.headersSize()) &&
				(mainModuleMemory.subsystem() == mainModuleDisk.subsystem()) &&
				(mainModuleMemory.DllCharacteristics() == mainModuleDisk.DllCharacteristics()) &&
				(mainModuleMemory.GetImports().size() == mainModuleDisk.GetImports().size()) &&
				(mainModuleMemory.sections().size() == mainModuleDisk.sections().size()))
			{
				data->ScannerResult.append(L" module_not_hollowed ");
			}
			else
			{
				data->ScannerResult.append(L" module_hollowed ");
			}
		}
	}
	else
	{
		data->ScannerResult.append(L" module_not_hollowed ");
	}

}


bool MatchingEngine::Initialize()
{
	boost::lock_guard<boost::mutex> lk(m_mutex);
	bool ret = true;

	m_logger.Trace("MatchingEngine::Initialize - About to initialize correlation engine channels");

	if (m_config.IsInitialized() && !IsInitialized())
	{	
		MatchEventBaseObject matchEventProcessCreate = std::make_shared<MatchEventProcessCreate>();
		MatchEventBaseObject matchEventPowershell = std::make_shared<MatchEventPowershell>();

		//Adding new event
		if (matchEventProcessCreate->Initialize() && 
			AddNewMatcher(SysmonXTypes::EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS, matchEventProcessCreate))
		{
			m_logger.Trace("MatchingEngine::Initialize - Process create matcher was succesfully added");
		}
		else
		{
			m_logger.Error("MatchingEngine::Initialize - There was a problem adding Process Create matcher");
		}

		if (matchEventPowershell->Initialize() &&
			AddNewMatcher(SysmonXTypes::EventID::SECURITY_EVENT_ID_SYSMONX_POWERSHELL_START_COMMAND, matchEventPowershell))
		{
			m_logger.Trace("MatchingEngine::Initialize - Powershell event matching was succesfully added");
		}
		else
		{
			m_logger.Error("MatchingEngine::Initialize - There was a problem adding Powershell event matcher");
		}

		m_isInitialized = true;
		ret = true;
	}
	else
	{
		ret = true;
	}

	return ret;
}

//Events Pre filters calls - Filters are called concurrently
bool MatchingEngine::EventPreProcessor(SysmonXTypes::EventObject &data)
{
	bool ret = false;

	if (m_config.IsInitialized())
	{
		MatchEventContainer::iterator matcherIT = m_matchers.find((SysmonXTypes::EventID)data->EventID);
		if (matcherIT != m_matchers.end())
		{
			MatchEventBaseObject &matchEventHandler = matcherIT->second;
			if (matchEventHandler &&
				matchEventHandler->ShouldFilterEvent(data))
			{
				ret = true;
			}
		}
	}

	return ret;
}

//Events Post filters calls - Filters are called in a serialized fashion
bool MatchingEngine::EventPostProcessor(SysmonXTypes::EventObject &data)
{
	bool ret = true;

	if (m_config.IsInitialized())
	{
		if (data->EventID == EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS)
		{
			PerformProcessHollowingScan(data);
		}
		ret = true;
	}

	return ret;
}

//Handling new report dispatching
void MatchingEngine::SendNewReport(SysmonXTypes::EventObject &data)
{
	if (m_reportManager.IsInitialized())
	{
		//Create new report
		SysmonXTypes::ReportObject newReportEvent = SysmonXEvents::GetNewReportEvent(data);

		//And dispatch it
		m_reportManager.DispatchReport(newReportEvent);
	}
}

//Adding a new Filtering Condition to an specific event
bool MatchingEngine::AddNewEventFilterCondition(
	const EventID &eventID,
	const EventPropertyName &propertyName,
	const EventFilterEvalGroup &evalGroup,
	const EventFilterOperation &operation,
	const MATCHING_TYPE_STRING &data)
{
	boost::lock_guard<boost::mutex> lk(m_mutex);
	bool ret = false;

	if (!m_matchers.empty() && 
		!propertyName.empty() && 
		!data.empty() && 
		(evalGroup != EventFilterEvalGroup::EVENT_FILTER_EVAL_NA) &&
		(operation != EventFilterOperation::EVENT_FILTER_OPERATION_NA))
	{
		MatchEventContainer::iterator matcherIT = m_matchers.find(eventID);
		if (matcherIT != m_matchers.end())
		{
			MatchEventBaseObject &matchEventHandler = matcherIT->second;
			if (matchEventHandler &&
				matchEventHandler->AddNewFilterCondition(propertyName, evalGroup, operation, data))
			{
				ret = true;
			}
		}
	}

	return ret;
}
