#include "common.h"

bool EventCollectorSample::Initialize()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	logger.Trace("EventCollectorSample::Initialize - About to initialize Event Collector Sample Technology");

	if (config.IsInitialized() && !IsInitialized())
	{
		m_collectorPolicies.clear();
		m_isEnabled = false;
		m_executionThread = nullptr;
		m_isInitialized = true;
		ret = true;
	}
	else
	{
		ret = true;
	}

	return ret;
}


bool EventCollectorSample::UpdatePolicy(const SysmonXTypes::RulesContainer &reportData)
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	if (IsInitialized())
	{
		m_isEnabled = false;
		logger.Trace("EventCollectorSample::UpdatePolicy - About to update Sample Event collection policy");

		ret = true;
	}

	return ret;
}

bool EventCollectorSample::StartEventsCollection()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	if (IsInitialized())
	{
		logger.Trace("EventCollectorSample::StartEventsCollection - About to start Sample event collection");

		//Waiting running thread to finish
		if (m_isEnabled)
		{
			m_isEnabled = false;
			m_executionThread->join();
		}

		//Restarting the thread
		m_isEnabled = true;
		m_executionThread.reset(new boost::thread(boost::bind(&EventCollectorSample::ThreadWorker, this)));
		if (m_executionThread)
		{
			ret = true;
		}
	}

	return ret;
}

bool EventCollectorSample::StopEventsCollection()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	logger.Trace("EventCollectorSample::StopEventsCollection - About to stop Sample event collection");

	if (IsInitialized())
	{
		//Waiting running thread to finish
		if (m_isEnabled)
		{
			//Waiting for current thread to finish
			m_isEnabled = false;
			m_executionThread->join();
		}
		else
		{
			//Thread is already finished
			ret = true;
		}
	}

	return ret;
}

void EventCollectorSample::ThreadWorker()
{
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	//Waiting for collection to be enable
	while (m_isEnabled)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
