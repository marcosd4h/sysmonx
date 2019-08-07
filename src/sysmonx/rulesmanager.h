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
	bool GenerateTraceBackendConfiguration(const std::wstring &configFile, const std::wstring &targetTraceBackendConfigFile);
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
	bool IsSysmonXOnlyEventNameValid(const EventName &name);

	bool IsConditionFilterValid(const EventConditionFilterName &filterName);
	bool IsSysmonXOnlyConditionFilterValid(const EventConditionFilterName &filterName);

	bool IsEventPropertyValid(const EventName &name, const EventPropertyName &property);
	bool IsSysmonXOnlyEventPropertyValid(const EventName &name, const EventPropertyName &property);

	bool IsSysmonXOnlyEventAggregationValid(const EventName &name);

	//private vars
	std::vector <EventName> m_validEventsNames;
	std::vector <EventName> m_validSysmonXOnlyEventsNames;

	std::vector <EventConditionFilterName> m_validConditionFilters;
	std::vector <EventConditionFilterName> m_validSysmonXOnlyConditionFilters;

	std::map <EventName, EventPropertyNameContainer> m_validEventProperties;
	std::map <EventName, EventPropertyNameContainer> m_validSysmonXOnlyEventProperties;

	std::vector <EventName> m_validSysmonXOnlyEventsAggregation;
	bool m_isInitialized;
};