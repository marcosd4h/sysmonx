#include "common.h"

bool EventsManager::Initialize()
{
	bool ret = true;

	m_logger.Trace("EventsManager::Initialize - About to initialize event collectors");

	if (m_config.IsInitialized() && !IsInitialized())
	{
		//Adding reporters known at compile time
		auto eventCollectionETW = std::make_shared<EventCollectorETW>();

		//auto eventCollectionSample = std::make_shared<EventCollectorSample>();

		//ETW event collection
		if (eventCollectionETW->Initialize() && AddNewCollector(SysmonXTypes::EventCollectorTechID::EVENT_COLLECTOR_TECH_ETW, eventCollectionETW))
		{
			m_logger.Trace("EventsManager::Initialize - ETW technology event collection was succesfully added");
		}
		else
		{
			m_logger.Error("EventsManager::Initialize - There was a problem adding ETW event collection technology");
		}

		if (AreCollectorsReady())
		{
			m_isInitialized = true;
			ret = true;
		}
	}
	else
	{
		ret = true;
	}

	return ret;
}


bool EventsManager::StartEventsCollection()
{
	bool ret = false;

	m_logger.Trace("About to run EventsManager::StartEventsCollection()");

	if (m_config.IsInitialized() && IsInitialized())
	{
		for (auto it = m_collectors.begin(); it != m_collectors.end(); it++)
		{
			std::shared_ptr<EventCollectorBase> &collectorBase = it->second;
			if (collectorBase)
			{
				m_logger.Trace(L"About to start collection on collector {}", collectorBase->GetCollectorDescription());

				if (collectorBase->StartEventsCollection())
				{
					m_logger.Trace(L"Collection was succesfully started at collector {}", collectorBase->GetCollectorDescription());
				}
				else
				{
					m_logger.Trace(L"There was a problem starting events collection at {}", collectorBase->GetCollectorDescription());
				}
			}
		}

		ret = true;
	}

	return ret;
}

bool EventsManager::StopEventsCollection()
{
	bool ret = false;

	m_logger.Trace("About to run EventsManager::StopEventsCollection()");

	if (m_config.IsInitialized() && IsInitialized())
	{
		for (auto it = m_collectors.begin(); it != m_collectors.end(); it++)
		{
			std::shared_ptr<EventCollectorBase> &collectorBase = it->second;
			if (collectorBase)
			{
				m_logger.Trace(L"About to stop collection on collector {}",collectorBase->GetCollectorDescription());

				if (collectorBase->StopEventsCollection())
				{
					m_logger.Trace(L"Collection was succesfully stopped at collector {}", collectorBase->GetCollectorDescription());
				}
				else
				{
					m_logger.Trace(L"There was a problem stopping events collection at {}", collectorBase->GetCollectorDescription());
				}
			}
		}

		ret = true;
	}

	return ret;
}