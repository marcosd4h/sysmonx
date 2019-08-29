#pragma once

#include "common.h"

namespace MatchHelpers
{
	SysmonXTypes::EventID GetEventIDBasedOnName(const SysmonXTypes::EventIDName &eventName);
	SysmonXTypes::EventPropertyID GetEventPropertyIDBasedOnName(const SysmonXTypes::EventPropertyName &propertyName);
	SysmonXTypes::EventFilterOperation GetEventFilterOperationBasedOnName(const SysmonXTypes::EventConditionFilterName &conditionName);
	const SysmonXTypes::MATCHING_TYPE_STRING& GetMatchingDataFromEvent(const SysmonXTypes::EventObject &eventData, const SysmonXTypes::EventPropertyID &eventID);
}
