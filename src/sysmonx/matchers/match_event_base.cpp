#pragma once

#include "common.h"

bool MatchEventBase::IsPropertySupported(const SysmonXTypes::EventPropertyName &propertyName)
{
	bool ret = false;

	if (!propertyName.empty())
	{
		EventPropertyID workingProperty = MatchHelpers::GetEventPropertyIDBasedOnName(propertyName);

		for (EventPropertyID &it : m_properties)
		{
			if (workingProperty == it)
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}


bool MatchEventBase::AreMatchingFiltersForThisEvent(const EventObject &eventData)
{
	bool ret = false;

	if (eventData && (eventData->EventID == m_eventID))
	{
		//Exclude takes precedence
		if (ShouldEventBeExcluded(eventData))
		{
			//True means here that  event have to be exluded
			//so quitting now
			return ret;
		}


		//Then we look into OR group and decide if event should be included for next phase
		if (ShouldEventBeIncludedGroupOR(eventData))
		{
			//if events have to be included, return immediately
			ret = true;
		}
		else
		{
			//Event was not included for OR evaluation, now trying performance expensive AND group conditions
			if (ShouldEventBeIncludedGroupAND(eventData))
			{
				//Event have to be included!
				ret = true;
			}
		}
	}

	return ret;
}

bool MatchEventBase::AddNewFilterConditionToTargetProperty(
	const SysmonXTypes::EventPropertyName &propertyName,
	const EventFilterEvalGroup &evalGroup,
	const EventFilterOperation &operation,
	const MATCHING_TYPE_STRING &data,
	FilterEventProperties &workContainer)
{
	bool ret = false;

	//Getting property ID for new property name
	EventPropertyID propertyID = MatchHelpers::GetEventPropertyIDBasedOnName(propertyName);
	if (propertyID != EventPropertyID::EVENT_PROPERTY_NA)
	{
		//Checking if new filter condition needs to be added to existing element or not
		auto mapITFilterCondition = workContainer.find(propertyID);
		if (mapITFilterCondition != workContainer.end())
		{
			//previous element found
			mapITFilterCondition->second->AddEvalCondition(operation, data);
		}
		else
		{
			//creating new element with new eval condition, and then passing it down to working container
			FilterPropertyProcessor newFilterCondition = std::make_shared<FilterPropertyBase>(propertyName);
			newFilterCondition->AddEvalCondition(operation, data);
			workContainer.insert(std::make_pair(propertyID, newFilterCondition));
		}

		ret = true;
	}

	return ret;
}

bool MatchEventBase::AddNewFilterCondition(const SysmonXTypes::EventPropertyName &propertyName, const EventFilterEvalGroup &evalGroup, const EventFilterOperation &operation, const MATCHING_TYPE_STRING &data)
{
	bool ret = false;

	//Checking input first
	if ((!propertyName.empty()) &&
		(!data.empty()) &&
		IsPropertySupported(propertyName) &&
		(evalGroup >= EventFilterEvalGroup::EVENT_FILTER_EVAL_INCLUDE_GROUP_AND) &&
		(evalGroup < EventFilterEvalGroup::EVENT_FILTER_EVAL_NA) &&
		(operation >= EventFilterOperation::EVENT_FILTER_OPERATION_IS) &&
		(operation < EventFilterOperation::EVENT_FILTER_OPERATION_NA))
	{
		if (evalGroup == EventFilterEvalGroup::EVENT_FILTER_EVAL_INCLUDE_GROUP_AND)
		{
			ret = AddNewFilterConditionToTargetProperty(propertyName, evalGroup, operation, data, m_IncludeANDFilterGroup);
		}
		else if (evalGroup == EventFilterEvalGroup::EVENT_FILTER_EVAL_INCLUDE_GROUP_OR)
		{
			ret = AddNewFilterConditionToTargetProperty(propertyName, evalGroup, operation, data, m_IncludeORFilterGroup);
		}
		else if (evalGroup == EventFilterEvalGroup::EVENT_FILTER_EVAL_EXCLUDE_GROUP)
		{
			ret = AddNewFilterConditionToTargetProperty(propertyName, evalGroup, operation, data, m_ExcludeFilterGroup);
		}
	}

	return ret;
}


bool MatchEventBase::ShouldEventBeExcluded(const EventObject &eventData)
{
	bool ret = false;

	if (!m_ExcludeFilterGroup.empty() && eventData && (eventData->EventID == m_eventID))
	{
		//Iterating over supported filter properties and performing an eval operation
		for (EventPropertyID &eventPropertyID : m_properties)
		{
			auto propertyProcessorIT = m_ExcludeFilterGroup.find(eventPropertyID);
			if (propertyProcessorIT != m_ExcludeFilterGroup.end())
			{
				if (propertyProcessorIT->second->IsThereAMatch(MatchHelpers::GetMatchingDataFromEvent(eventData, eventPropertyID)))
				{
					ret = true;
				}
			}

		}
	}

	return ret;
}

bool MatchEventBase::ShouldEventBeIncludedGroupOR(const EventObject &eventData)
{
	bool ret = false;

	if (!m_IncludeORFilterGroup.empty() && eventData && (eventData->EventID == m_eventID))
	{
		//Iterating over supported filter properties and performing an eval operation
		for (EventPropertyID &eventPropertyID : m_properties)
		{
			auto propertyProcessorIT = m_IncludeORFilterGroup.find(eventPropertyID);
			if (propertyProcessorIT != m_IncludeORFilterGroup.end())
			{
				if (propertyProcessorIT->second->IsThereAMatch(MatchHelpers::GetMatchingDataFromEvent(eventData, eventPropertyID)))
				{
					ret = true;
				}
			}

		}
	}

	return ret;
}

bool MatchEventBase::ShouldEventBeIncludedGroupAND(const EventObject &eventData)
{
	bool ret = false;

	if (!m_IncludeANDFilterGroup.empty() && eventData && (eventData->EventID == m_eventID))
	{
		//Iterating over supported filter properties and performing an eval operation
		for (EventPropertyID &eventPropertyID : m_properties)
		{
			auto propertyProcessorIT = m_IncludeANDFilterGroup.find(eventPropertyID);
			if (propertyProcessorIT != m_IncludeANDFilterGroup.end())
			{
				if (propertyProcessorIT->second->IsThereAMatch(MatchHelpers::GetMatchingDataFromEvent(eventData, eventPropertyID)))
				{
					ret = true;
				}
			}

		}
	}

	return ret;
}