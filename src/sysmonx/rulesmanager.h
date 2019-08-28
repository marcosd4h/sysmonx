#pragma once

#include "common.h"

class RulesManager
{
public:

	static RulesManager& Instance()
	{
		static RulesManager instance;
		return instance;
	}

	bool IsInitialized() { return m_isInitialized; };
	bool Initialize();
	bool SetConfigurationFile(const std::wstring &configFile);
	bool IsConfigFileSyntaxValid(const std::wstring &configFile);
	bool PopulateMatchingEngineMatchers(const std::wstring &configFile);

private:

	RulesManager() : m_isInitialized(false) {};
	~RulesManager() {}

	//Forcing singleton here
	RulesManager(const RulesManager&) = delete;
	RulesManager(RulesManager&&) = delete;
	RulesManager& operator=(const RulesManager&) = delete;
	RulesManager& operator=(RulesManager&&) = delete;

	// Private vars
	bool IsEventNameValid(const EventName &name);
	bool IsConditionFilterValid(const EventConditionFilterName &filterName);
	bool IsEventPropertyValid(const EventName &name, const EventPropertyName &property);
	bool IsEventAggregationValid(const EventName &name);

	//private vars
	std::vector <EventName> m_validEventsNames;
	std::vector <EventConditionFilterName> m_validConditionFilters;
	std::map <EventName, EventPropertyNameContainer> m_validEventProperties;
	std::vector <EventName> m_validEventsAggregation;
	bool m_isInitialized;
};