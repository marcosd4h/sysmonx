#pragma once

#include "common.h"

class FilterPropertyBase
{
public:

	//General functions
	bool AddEvalCondition(const EventFilterOperation &operation, const MATCHING_TYPE_STRING &stringData);
	bool IsReadyToProcess();
	bool IsThereAMatch(const MATCHING_TYPE_STRING &matchData);

	//Eval Conditions
	bool EvalFilterCondition_IS(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_IS_NOT(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_CONTAINS(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_EXCLUDES(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_BEGIN_WITH(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_END_WITH(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_LESS_THAN(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_MORE_THAN(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_IMAGE(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_REGEX_MATCH(const MATCHING_TYPE_STRING &value);
	bool EvalFilterCondition_REGEX_SEARCH(const MATCHING_TYPE_STRING &value);

	//Helpers
	bool IsReadyPolicyFilterCondition_IS() { return !m_evalPolicyCondition_IS.empty(); }
	bool IsReadyPolicyFilterCondition_IS_NOT() { return !m_evalPolicyCondition_IS_NOT.empty(); }
	bool IsReadyPolicyFilterCondition_CONTAINS() { return !m_evalPolicyCondition_CONTAINS.empty(); }
	bool IsReadyPolicyFilterCondition_EXCLUDES() { return !m_evalPolicyCondition_EXCLUDES.empty(); }
	bool IsReadyPolicyFilterCondition_BEGIN_WITH() { return !m_evalPolicyCondition_BEGIN_WITH.empty(); }
	bool IsReadyPolicyFilterCondition_END_WITH() { return !m_evalPolicyCondition_END_WITH.empty(); }
	bool IsReadyPolicyFilterCondition_LESS_THAN() { return !m_evalPolicyCondition_LESS_THAN.empty(); }
	bool IsReadyPolicyFilterCondition_MORE_THAN() { return !m_evalPolicyCondition_MORE_THAN.empty(); }
	bool IsReadyPolicyFilterCondition_IMAGE() { return !m_evalPolicyCondition_IMAGE.empty(); }
	bool IsReadyPolicyFilterCondition_REGEX_MATCH() { return !m_evalPolicyCondition_REGEX_MATCH.empty(); }
	bool IsReadyPolicyFilterCondition_REGEX_SEARCH() { return !m_evalPolicyCondition_REGEX_SEARCH.empty(); }
	std::wstring &GetFilteredProperty() { return m_propertyName; }

	//Lifecycle Management
	FilterPropertyBase() = default;
	FilterPropertyBase(const SysmonXTypes::EventPropertyName &propertyName) : m_propertyName(propertyName) {}
	~FilterPropertyBase() = default;

private:

	//Private Vars
	SysmonXTypes::EventPropertyName m_propertyName;
	EventFilterContainer m_evalPolicyCondition_IS;
	EventFilterContainer m_evalPolicyCondition_IS_NOT;
	EventFilterContainer m_evalPolicyCondition_CONTAINS;
	EventFilterContainer m_evalPolicyCondition_EXCLUDES;
	EventFilterContainer m_evalPolicyCondition_BEGIN_WITH;
	EventFilterContainer m_evalPolicyCondition_END_WITH;
	EventFilterContainer m_evalPolicyCondition_LESS_THAN;
	EventFilterContainer m_evalPolicyCondition_MORE_THAN;
	EventFilterContainer m_evalPolicyCondition_IMAGE;
	EventFilterContainerRegex m_evalPolicyCondition_REGEX_MATCH;
	EventFilterContainerRegex m_evalPolicyCondition_REGEX_SEARCH;

};

typedef std::shared_ptr<FilterPropertyBase> FilterPropertyProcessor;
typedef std::unordered_map<EventPropertyID, FilterPropertyProcessor> FilterEventProperties;