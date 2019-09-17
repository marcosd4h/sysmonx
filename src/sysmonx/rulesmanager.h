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
	bool IsConfigFileSyntaxValid();
	bool LoadNewRulesContent();

private:

	RulesManager() = default;
	~RulesManager() = default;

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
	bool IsConfigXMLEventValid(const pugi::xml_node &event);
	bool ProcessConfigXMLEvent(const pugi::xml_node &event, const UINT32 &workingFilterEvalGroup);

	//private vars
	bool m_isInitialized = false;
	ConfigManager& m_config = ConfigManager::Instance();

	std::vector <EventName> m_validEventsNames;
	std::vector <EventConditionFilterName> m_validConditionFilters;
	std::map <EventName, EventPropertyNameContainer> m_validEventProperties;
	std::vector <EventName> m_validEventsAggregation;
};