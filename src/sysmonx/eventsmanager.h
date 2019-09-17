#pragma once

#include "common.h"

class EventsManager
{
public:

	static EventsManager& Instance()
	{
		static EventsManager instance;
		return instance;
	}

	bool Initialize();	
	bool StartEventsCollection();
	bool StopEventsCollection();

	bool IsInitialized() { return m_isInitialized; };
	bool AreCollectorsReady() { return !m_collectors.empty(); }

private:

	//Adding new Event Collector
	template <typename T>
	bool AddNewCollector(const SysmonXTypes::EventCollectorTechID &collectorID, std::shared_ptr<T>& collectorPtr)
	{
		bool ret = false;
		try
		{
			if (collectorPtr)
			{
				//Checking if element is not there
				if (m_collectors.find(collectorID) == m_collectors.end())
				{
					m_collectors.insert(std::make_pair(collectorID, collectorPtr));
					ret = true;
				}
			}
		}
		catch (...)
		{
			ret = false;
		}

		return ret;
	}

	//Lifecycle management
	EventsManager() = default;

	virtual ~EventsManager() 
	{
		if (IsInitialized())
		{
			for (auto it = m_collectors.begin(); it != m_collectors.end(); it++)
			{
				std::shared_ptr<EventCollectorBase> &collectorBase = it->second;
				if (collectorBase)
				{
					if (collectorBase->IsCollectionRunning())
					{
						collectorBase->StopEventsCollection();
					}					
				}
			}
		}
	}

	//Forcing singleton here
	EventsManager(const EventsManager&) = delete;
	EventsManager(EventsManager&&) = delete;
	EventsManager& operator=(const EventsManager&) = delete;
	EventsManager& operator=(EventsManager&&) = delete;

	// Private vars
	bool m_isInitialized = false;
	TraceHelpers::Logger& m_logger = TraceHelpers::Logger::Instance();
	ConfigManager& m_config = ConfigManager::Instance();
	boost::mutex m_mutex;
	SysmonXTypes::EventCollectorsContainer m_collectors;
};