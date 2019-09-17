#pragma once

#include "common.h"
#include "matchers\filter_property_processor.h"
#include "matchers\match_event_base.h"
#include "matchers\match_event_sysmon_create_process.h"

using boost::asio::dispatch;
using boost::asio::spawn;
using boost::asio::strand;
using boost::asio::thread_pool;
using boost::asio::yield_context;


namespace MatchHelpers
{
	SysmonXTypes::EventID GetEventIDBasedOnName(const SysmonXTypes::EventIDName& eventName);
	SysmonXTypes::EventPropertyID GetEventPropertyIDBasedOnName(const SysmonXTypes::EventPropertyName& propertyName);
	SysmonXTypes::EventFilterOperation GetEventFilterOperationBasedOnName(const SysmonXTypes::EventConditionFilterName& conditionName);
	const SysmonXTypes::MATCHING_TYPE_STRING& GetMatchingDataFromEvent(const SysmonXTypes::EventObject& eventData, const SysmonXTypes::EventPropertyID& eventID);
}


class MatchingEngine
{
public:

	static MatchingEngine& Instance()
	{
		static MatchingEngine instance;
		return instance;
	}

	bool IsInitialized() { return m_isInitialized; };
	bool Initialize();


	// Entry points for events arriving to the matching engine.
	// Code return inmediately after enqueuing security events to be handled by the thread pool
	// Design decision: data is not const to allow logic to add attributes while event is processed through the pipeline
	void DispatchEvent(SysmonXTypes::EventObject &data)
	{		
		//Quick and dirty evaluation
		if (m_isInitialized && m_shouldProcessEvents)
		{
			spawn(m_pool, boost::bind(&MatchingEngine::ProcessNewEvents, this, data));
		}
	}

	//Matching engine lifecycle
	bool DisableEventsProcessing()
	{
		boost::lock_guard<boost::mutex> lk(m_mutex);
		bool ret = false;

		if (m_isInitialized)
		{
			m_shouldProcessEvents = false;
			ret = true;
		}

		return ret;
	}

	bool EnableEventsProcessing()
	{
		boost::lock_guard<boost::mutex> lk(m_mutex);
		bool ret = false;

		if (m_isInitialized)
		{
			m_shouldProcessEvents = true;
			ret = true;
		}

		return ret;
	}

	bool AddNewEventFilterCondition(
		const SysmonXTypes::EventID &eventID,
		const EventPropertyName &propertyName,
		const EventFilterEvalGroup &evalGroup,
		const EventFilterOperation &operation,
		const MATCHING_TYPE_STRING &data);

	void ClearEventFilters();

private:

	//Object lifecycle 
	MatchingEngine(const size_t nrThreads = SysmonXDefs::SYSMONX_DEFAULT_WORKER_THREADS):
		m_pool(nrThreads), m_dispatchStrand(m_pool.get_executor()) {}

	//Object lifecycle 
	virtual ~MatchingEngine() 
	{
		m_shouldProcessEvents = false;
		if (IsInitialized())
		{
			m_pool.join();
		}
	}

	//Adding New Matcher
	template <typename T>
	bool AddNewMatcher(const SysmonXTypes::EventID &eventID, std::shared_ptr<T>& matcherPtr)
	{
		bool ret = false;

		if (matcherPtr)
		{
			//Checking if element is not there			
			if (m_matchers.find(eventID) == m_matchers.end())
			{
				m_matchers.insert(std::make_pair(eventID, matcherPtr));
				ret = true;
			}
		}

		return ret;
	}

	//Event Pre processor - Basic filtering happens here
	//Supported filters here will run on a concurrent context
	bool EventPreProcessor(SysmonXTypes::EventObject &data);

	//Event Post processor - Advanced filtering happens here
	//Supported filter here will be called in a serialized fashion
	bool EventPostProcessor(SysmonXTypes::EventObject &data);

	//Event Report Handler
	void SendNewReport(SysmonXTypes::EventObject &data);

	//Serialized completion handler wrapper
	//Report on succesful PostProcessor Filtering
	void SerializedEventsHandler(SysmonXTypes::EventObject &data)
	{
		//Advanced serialized filtering happens here
		if (EventPostProcessor(data))
		{
			//If events pass the pre and post processor filters, it should be schedule to be reported
			SendNewReport(data);
		}
	}

	//Handler of new Security Events
	void ProcessNewEvents(SysmonXTypes::EventObject &data)
	{
		//Basic concurrent filtering happens here
		if (EventPreProcessor(data))
		{
			dispatch(m_dispatchStrand, boost::bind(&MatchingEngine::SerializedEventsHandler, this, data));
		}
	}

	//Forcing singleton here
	MatchingEngine(const MatchingEngine&) = delete;
	MatchingEngine(MatchingEngine&&) = delete;
	MatchingEngine& operator=(const MatchingEngine&) = delete;
	MatchingEngine& operator=(MatchingEngine&&) = delete;

	// Private vars
	bool m_isInitialized = false;
	bool m_shouldProcessEvents = false;
	thread_pool m_pool;
	strand<thread_pool::executor_type> m_dispatchStrand;
	ReportManager& m_reportManager = ReportManager::Instance();
	TraceHelpers::Logger& m_logger = TraceHelpers::Logger::Instance();
	ConfigManager& m_config = ConfigManager::Instance();
	boost::mutex m_mutex;
	MatchEventContainer m_matchers;
};