#pragma once

#include "common.h"


class MatchEventBase
{
public:

	//Adding new policies from registry
	bool AddNewFilterCondition(const SysmonXTypes::EventPropertyName &propertyName, const  SysmonXTypes::EventFilterEvalGroup &evalGroup, const  SysmonXTypes::EventFilterOperation &operation, const  SysmonXTypes::MATCHING_TYPE_STRING &data);

	//Check if property is supported by current event
	bool IsPropertySupported(const SysmonXTypes::EventPropertyName &propertyName);

	//Evaluation if event should be filtered
	//Upon true  events will be sent to next phase (reporting) 
	bool ShouldFilterEvent(const  SysmonXTypes::EventObject &eventData);

	//Helpers
	SysmonXTypes::EventID GetEventID() { return m_eventID; }
	SysmonXTypes::EventIDName GetEventName() { return m_eventName; }
	bool IsInitialized() { return m_isInitialized; }

	//Initialize
	virtual bool Initialize() = 0;

protected:

	//Lifecycle Management
	MatchEventBase(const SysmonXTypes::EventID &eventID, const SysmonXTypes::EventIDName &eventName) : m_isInitialized(false), m_eventID(eventID), m_eventName(eventName) {}
	virtual ~MatchEventBase() = default;

	//Evaluation if event should be filtered
	bool ShouldEventBeExcluded(const  SysmonXTypes::EventObject &eventData);
	bool ShouldEventBeIncludedGroupOR(const  SysmonXTypes::EventObject &eventData);
	bool ShouldEventBeIncludedGroupAND(const  SysmonXTypes::EventObject &eventData);
	bool AddNewFilterConditionToTargetProperty(
		const SysmonXTypes::EventPropertyName &propertyName,
		const SysmonXTypes::EventFilterEvalGroup &evalGroup,
		const SysmonXTypes::EventFilterOperation &operation,
		const SysmonXTypes::MATCHING_TYPE_STRING &data,
		FilterEventProperties &targetProperty);

	//Protected vars
	bool m_isInitialized;
	SysmonXTypes::EventIDName m_eventName;
	std::vector < SysmonXTypes::EventPropertyID> m_properties;

	//Filter containers
	FilterEventProperties m_IncludeANDFilterGroup;
	FilterEventProperties m_IncludeORFilterGroup;
	FilterEventProperties m_ExcludeFilterGroup;

private:
	SysmonXTypes::EventID m_eventID;
};
typedef std::shared_ptr<MatchEventBase> MatchEventBaseObject;
typedef std::map<EventID, MatchEventBaseObject> MatchEventContainer;