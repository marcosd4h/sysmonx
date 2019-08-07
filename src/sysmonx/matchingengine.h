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
		if (IsInitialized())
		{
			spawn(m_pool, boost::bind(&MatchingEngine::ProcessNewEvents, this, data));
		}
	}

	bool AddNewEventFilterCondition(
		const SysmonXTypes::EventID &eventID,
		const EventPropertyName &propertyName,
		const EventFilterEvalGroup &evalGroup,
		const EventFilterOperation &operation,
		const MATCHING_TYPE_STRING &data);

private:

	//Object lifecycle 
	MatchingEngine(const size_t nrThreads = SysmonXDefs::SYSMONX_DEFAULT_WORKER_THREADS):
		m_isInitialized(false), m_pool(nrThreads), m_dispatchStrand(m_pool.get_executor()),
		m_reportManager(ReportManager::Instance()), m_logger(TraceHelpers::Logger::Instance()),
		m_config(ConfigManager::Instance()) {}

	//Object lifecycle 
	virtual ~MatchingEngine() 
	{
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
	bool m_isInitialized;
	thread_pool m_pool;
	strand<thread_pool::executor_type> m_dispatchStrand;
	ReportManager& m_reportManager;
	TraceHelpers::Logger& m_logger;
	ConfigManager& m_config;
	boost::mutex m_mutex;
	MatchEventContainer m_matchers;
};