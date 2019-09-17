#pragma once

#include "common.h"

class EventCollectorSample : public EventCollectorBase
{
public:
	bool Initialize();
	bool UpdatePolicy(const SysmonXTypes::RulesContainer &reportData);
	bool StartEventsCollection();
	bool StopEventsCollection();
	void ThreadWorker();
	bool IsInitialized() { return m_isInitialized; };
	bool IsCollectionRunning() { return m_isEnabled; };

	EventCollectorSample() :
		EventCollectorBase(L"Event Collector Sample Technology", SysmonXTypes::EventCollectorTechID::EVENT_COLLECTOR_TECH_ETW) {}

	virtual ~EventCollectorSample()
	{
		m_executionThread->join();
	}

private:
	bool m_isInitialized = false;
	bool m_isEnabled = false;
	boost::shared_ptr<boost::thread> m_executionThread = nullptr;
	boost::mutex m_mutex;
	SysmonXTypes::RulesContainer m_collectorPolicies;
};

