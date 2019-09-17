#pragma once

#include "common.h"

//This interface was designed to be stateless, so no synchronization was added
class ReportChannelBase
{

public:
	virtual bool SendReportFinding(const SysmonXTypes::ReportObject &reportData) = 0;
	virtual bool Initialize() = 0;

	const std::wstring &GetChannelDescription()
	{
		return m_description;
	}

	const SysmonXTypes::ReportChannelID GetChannelID()
	{
		return m_channelID;
	}

	void SetEnableStatus(bool newStatus)
	{
		m_enabledStatus = newStatus;
	}

	void SetInitializeStatus(bool newStatus)
	{
		m_isInitialized = newStatus;
	}

	bool IsEnabled()
	{
		return m_enabledStatus;
	}

	bool IsInitialized()
	{
		return m_isInitialized;
	}

	ReportChannelBase(const std::wstring &channelDescription, const SysmonXTypes::ReportChannelID &channelID) :
		m_description(channelDescription), m_channelID(channelID) {}

	ReportChannelBase() = default;

	virtual ~ReportChannelBase() = default;

private:
	std::wstring m_description;
	SysmonXTypes::ReportChannelID m_channelID = SysmonXTypes::ReportChannelID::REPORT_CHANNEL_NA;
	bool m_enabledStatus = false;
	bool m_isInitialized = false;
};

typedef std::shared_ptr<ReportChannelBase> ReporterChannelObject;



class EventCollectorBase
{

public:
	virtual bool UpdatePolicy(const SysmonXTypes::RulesContainer &reportData) = 0;
	virtual bool Initialize() = 0;
	virtual bool StartEventsCollection() = 0;
	virtual bool StopEventsCollection() = 0;
	virtual bool IsCollectionRunning() = 0;
	virtual bool IsInitialized() = 0;

	const std::wstring &GetCollectorDescription()
	{
		return m_description;
	}

	const SysmonXTypes::EventCollectorTechID GetCollectorID()
	{
		return m_collectorID;
	}

	EventCollectorBase(const std::wstring &channelDescription, const SysmonXTypes::EventCollectorTechID &collectorID) :
		m_description(channelDescription), m_collectorID(collectorID) {}

	EventCollectorBase() = default;

	virtual ~EventCollectorBase() = default;

private:

	// Private vars
	std::wstring m_description;
	SysmonXTypes::EventCollectorTechID m_collectorID = SysmonXTypes::EventCollectorTechID::EVENT_COLLECTOR_TECH_NA;
};
typedef std::shared_ptr<EventCollectorBase> EventCollectorObject;



namespace SysmonXTypes
{
	//Containers
	typedef std::map<ReportChannelID, ReporterChannelObject> ReportersContainer;
	typedef std::map<EventCollectorTechID, EventCollectorObject> EventCollectorsContainer;
}
