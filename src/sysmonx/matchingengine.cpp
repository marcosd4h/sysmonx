#include "common.h"




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
		MatchEventContainer::iterator matcherIT = m_matchers.find((SysmonXTypes::EventID)data->EventID.GetValue());
		if (matcherIT != m_matchers.end())
		{
			MatchEventBaseObject &matchEventHandler = matcherIT->second;
			if (matchEventHandler &&
				matchEventHandler->AreMatchingFiltersForThisEvent(data))
			{
				ret = true;
			}
		}
	}

	return ret;
}

//Events Post filters calls - Filters are called in a serialized fashion
//TODO: Update this with better security scan logic
bool MatchingEngine::EventPostProcessor(SysmonXTypes::EventObject &data)
{
	bool ret = true;

	if (m_config.IsInitialized())
	{
		if (data->EventID == EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS)
		{

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

//Clearing previous filters before forcing new policy
void MatchingEngine::ClearEventFilters()
{
	boost::lock_guard<boost::mutex> lk(m_mutex);

	for (auto& matchEventEntry : m_matchers) 
	{
		matchEventEntry.second->ClearFilters();
	}
}