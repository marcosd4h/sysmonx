#include "common.h"

bool RulesManager::Initialize()
{
	bool ret = true;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	logger.Trace("RulesManager::Initialize - About to initialize rules manager");

	if (config.IsInitialized() && !IsInitialized())
	{
		//clearing up containers
		m_validEventsNames.clear();
		m_validConditionFilters.clear();
		m_validEventProperties.clear();
		m_validEventsAggregation.clear();

		//Adding Event Names
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_CREATE);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_TIME);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_NETWORK_CONNECT);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_TERMINATE);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_DRIVER_LOADER);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_IMAGE_LOAD);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_CREATE_REMOTE_THREAD);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_RAW_ACCESS_READ);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_ACCESS);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_REGISTRY_EVENT);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_STREAM);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PIPE_EVENT);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_WMI_EVENT);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_DNS_QUERY);
		m_validEventsNames.push_back(SysmonXDefs::EVENT_SYSMONX_NAME_POWERSHELL);

		//Adding SysmonX Condition Filters
		//TODO: check how to handle default condition to "is"
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_IS);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_IS_NOT);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_CONTAINS);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_EXCLUDES);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_BEGIN_WITH);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_END_WITH);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_LESS_THAN);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_MORE_THAN);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_IMAGE);
		m_validConditionFilters.push_back(SysmonXDefs::EVENT_FILTER_OPERATION_STR_REGEX_MATCH);

		//Adding  Supported Events Properties
		EventPropertyNameContainer ProcessCreate;
		ProcessCreate.push_back(L"RuleName");
		ProcessCreate.push_back(L"UtcTime");
		ProcessCreate.push_back(L"ProcessGuid");
		ProcessCreate.push_back(L"ProcessId");
		ProcessCreate.push_back(L"Image");
		ProcessCreate.push_back(L"FileVersion");
		ProcessCreate.push_back(L"Description");
		ProcessCreate.push_back(L"Product");
		ProcessCreate.push_back(L"Company");
		ProcessCreate.push_back(L"OriginalFileName");
		ProcessCreate.push_back(L"CommandLine");
		ProcessCreate.push_back(L"CurrentDirectory");
		ProcessCreate.push_back(L"User");
		ProcessCreate.push_back(L"LogonGuid");
		ProcessCreate.push_back(L"LogonId");
		ProcessCreate.push_back(L"TerminalSessionId");
		ProcessCreate.push_back(L"IntegrityLevel");
		ProcessCreate.push_back(L"Hashes");
		ProcessCreate.push_back(L"ParentProcessGuid");
		ProcessCreate.push_back(L"ParentProcessId");
		ProcessCreate.push_back(L"ParentImage");
		ProcessCreate.push_back(L"ParentCommandLine");
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_CREATE, ProcessCreate));

		EventPropertyNameContainer FileCreateTime;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_TIME, FileCreateTime));

		EventPropertyNameContainer NetworkConnect;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_NETWORK_CONNECT, FileCreateTime));

		EventPropertyNameContainer ProcessTerminate;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_TERMINATE, ProcessTerminate));

		EventPropertyNameContainer DriverLoad;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_DRIVER_LOADER, DriverLoad));

		EventPropertyNameContainer ImageLoad;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_IMAGE_LOAD, ImageLoad));

		EventPropertyNameContainer CreateRemoteThread;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_CREATE_REMOTE_THREAD, CreateRemoteThread));

		EventPropertyNameContainer RawAccessRead;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_RAW_ACCESS_READ, RawAccessRead));

		EventPropertyNameContainer ProcessAccess;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_ACCESS, ProcessAccess));

		EventPropertyNameContainer FileCreate;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE, FileCreate));

		EventPropertyNameContainer RegistryEvent;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_REGISTRY_EVENT, RegistryEvent));

		EventPropertyNameContainer FileCreateStreamHash;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_STREAM, FileCreateStreamHash));

		EventPropertyNameContainer PipeEvent;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PIPE_EVENT, PipeEvent));

		EventPropertyNameContainer WmiEvent;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_WMI_EVENT, WmiEvent));

		EventPropertyNameContainer DNSQueryEvent;
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_DNS_QUERY, DNSQueryEvent));

		//Adding SysmonX Supported Events Properties
		EventPropertyNameContainer PowershellEvent;
		PowershellEvent.push_back(L"ScriptBlock");
		m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMONX_NAME_POWERSHELL, PowershellEvent));

		//Adding SysmonX Aggregation Options;
		m_validEventsAggregation.push_back(L"cmdline_spoofing");
		m_validEventsAggregation.push_back(L"parent_pid_spoofing");
		m_validEventsAggregation.push_back(L"hollow_modules");

		m_isInitialized = true;
		ret = true;
	}
	else
	{
		ret = true;
	}

	return ret;
}


bool RulesManager::SetConfigurationFile(const std::wstring &configFile)
{
	bool ret = true;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	if (config.IsInitialized())
	{
		logger.Trace("RulesManager::SetConfigurationFile - About to process new configuration file");

		if (GeneralHelpers::IsValidFile(configFile))
		{
			logger.Trace("RulesManager::SetConfigurationFile - Give configuration file is valid");

			ret = true;
		}
		else
		{
			logger.Error("RulesManager::SetConfigurationFile - There was a problem with given configuration file");
		}
	}

	return ret;
}


bool RulesManager::IsEventNameValid(const EventName &name)
{
	bool ret = false;

	if (!m_validEventsNames.empty())
	{
		//Checking if given element is present
		for (auto eventName : m_validEventsNames)
		{
			if (boost::algorithm::iequals(eventName, name))
			{
				ret = true;
				break;
			}
		}

	}

	return ret;
}

bool RulesManager::IsConditionFilterValid(const EventConditionFilterName &filterName)
{
	bool ret = false;

	if (!m_validConditionFilters.empty())
	{
		//Checking if given element is present
		for (auto conditionFilterName : m_validConditionFilters)
		{
			if (boost::algorithm::iequals(conditionFilterName, filterName))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool RulesManager::IsEventPropertyValid(const EventName &name, const EventPropertyName &propertyName)
{
	bool ret = false;

	bool shoudlBreak = false;
	if (!m_validEventProperties.empty())
	{
		//Checking if given element is present
		for (auto eventPropertyContainer : m_validEventProperties)
		{
			//First iterating to target event name
			if (boost::algorithm::iequals(name, eventPropertyContainer.first))
			{
				for (auto eventProperty : eventPropertyContainer.second)
				{
					if (boost::algorithm::iequals(eventProperty, propertyName))
					{
						ret = true;
						shoudlBreak = true;
						break;
					}
				}

				if (shoudlBreak)
				{
					ret = true;
					break;
				}
			}
		}
	}

	return ret;
}


bool RulesManager::IsEventAggregationValid(const EventName &name)
{
	bool ret = false;

	if (!m_validEventsAggregation.empty())
	{
		//Checking if given element is present
		for (auto eventAggregationName : m_validEventsAggregation)
		{
			if (boost::algorithm::iequals(eventAggregationName, name))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}


//TODO: Perform this using a xml library that supports dtd/xsd schema validation.
//XML validation for the poor below
bool RulesManager::IsConfigFileSyntaxValid(const std::wstring &configFile)
{
	bool ret = false;

	pugi::xml_document doc;

	if (!configFile.empty() &&
		GeneralHelpers::IsValidFile(configFile) &&
		doc.load_file(configFile.c_str()))
	{
		//Traversing until EventFiltering tag
		pugi::xml_node eventFilteringNode = doc.child("Sysmon").child("EventFiltering");

		//parsing events, one by one
		for (pugi::xml_node event : eventFilteringNode)
		{
			//Checking if we are inside of a RuleGroup tag
			if (boost::algorithm::iequals(event.name(), "RuleGroup"))
			{
				for (pugi::xml_node eventInRuleGroup : event)
				{
					std::string workEventName(eventInRuleGroup.name());

					if (IsEventNameValid(GeneralHelpers::StrToWStr(workEventName)))
					{
						for (pugi::xml_attribute eventAttribute : eventInRuleGroup.attributes())
						{
							std::string workEventAttributeName(eventAttribute.name());
							std::string workEventAttributeValue(eventAttribute.value());
						}

						for (pugi::xml_node eventProperty : eventInRuleGroup.children())
						{
							std::string workEventPropertyName(eventProperty.name());

							if (IsEventPropertyValid(GeneralHelpers::StrToWStr(workEventName),
								GeneralHelpers::StrToWStr(workEventPropertyName)))
							{
								for (pugi::xml_attribute eventPropertyAttribute : eventProperty.attributes())
								{
									std::string workEventPropertyAttributeName(eventPropertyAttribute.name());
									std::string workEventPropertyAttributeValue(eventPropertyAttribute.value());
								}
							}
							else
							{
								return ret;
							}

						}
					}
					else
					{
						return ret;
					}
				}
			}
			else
			{
				std::string workEventName(event.name());

				if (IsEventNameValid(GeneralHelpers::StrToWStr(workEventName)))
				{
					for (pugi::xml_attribute eventAttribute : event.attributes())
					{
						std::string workEventAttributeName(eventAttribute.name());
						std::string workEventAttributeValue(eventAttribute.value());
					}

					for (pugi::xml_node eventProperty : event.children())
					{
						std::string workEventPropertyName(eventProperty.name());

						if (IsEventPropertyValid(GeneralHelpers::StrToWStr(workEventName),
							GeneralHelpers::StrToWStr(workEventPropertyName)))
						{
							for (pugi::xml_attribute eventPropertyAttribute : eventProperty.attributes())
							{
								std::string workEventPropertyAttributeName(eventPropertyAttribute.name());
								std::string workEventPropertyAttributeValue(eventPropertyAttribute.value());
							}
						}
						else
						{
							return ret;
						}
					}
				}
				else
				{
					return ret;
				}
			}
		}

		ret = true;
	}

	return ret;
}

bool RulesManager::PopulateMatchingEngineMatchers(const std::wstring &configFile)
{
	bool ret = false;

	pugi::xml_document doc;
	MatchingEngine &matchingEngine = MatchingEngine::Instance();

	if (!configFile.empty() &&
		GeneralHelpers::IsValidFile(configFile) &&
		matchingEngine.IsInitialized() &&
		doc.load_file(configFile.c_str()))
	{
		//Traversing until EventFiltering tag
		pugi::xml_node eventFilteringNode = doc.child("Sysmon").child("EventFiltering");

		//parsing events, one by one
		for (pugi::xml_node event : eventFilteringNode)
		{
			EventID workingEventID = MatchHelpers::GetEventIDBasedOnName(GeneralHelpers::StrToWStr(event.name()));
			UINT32 workingFilterEvalGroup = EventFilterEvalGroup::EVENT_FILTER_EVAL_NA;

			//Checking if we are inside of a RuleGroup tag
			if (boost::algorithm::iequals(event.name(), "RuleGroup"))
			{
				for (pugi::xml_attribute groupAttribute : event.attributes())
				{
					std::string workGroupAttributeName(groupAttribute.name());
					std::string workGroupAttributeValue(groupAttribute.value());

					if (boost::algorithm::iequals(workGroupAttributeName, "groupRelation") &&
						boost::algorithm::iequals(workGroupAttributeValue, "or"))
					{
						workingFilterEvalGroup = EventFilterEvalGroup::EVENT_FILTER_EVAL_INCLUDE_GROUP_OR;
					}
					else
					{
						workingFilterEvalGroup = EventFilterEvalGroup::EVENT_FILTER_EVAL_INCLUDE_GROUP_AND;
					}
				}

				for (pugi::xml_node eventInRuleGroup : event)
				{
					std::string workEventName(eventInRuleGroup.name());

					//Trace backend events, remove non Sysmon filter conditions
					if (IsEventNameValid(GeneralHelpers::StrToWStr(workEventName)))
					{
						for (pugi::xml_attribute eventAttribute : eventInRuleGroup.attributes())
						{
							std::string workEventAttributeName(eventAttribute.name());
							std::string workEventAttributeValue(eventAttribute.value());
							if (boost::algorithm::iequals(workEventAttributeName, "scanners"))
							{

							}
						}

						for (pugi::xml_node eventProperty : eventInRuleGroup.children())
						{
							std::string workingPropertyName(eventProperty.name());
							std::string workingPropertyValue(eventProperty.first_child().value());

							if (!workingPropertyName.empty() &&
								!workingPropertyName.empty() &&								
								IsEventPropertyValid(GeneralHelpers::StrToWStr(workEventName),
								GeneralHelpers::StrToWStr(workingPropertyName)))
							{
								for (pugi::xml_attribute eventPropertyAttribute : eventProperty.attributes())
								{
									std::string workEventPropertyAttributeName(eventPropertyAttribute.name());
									std::string workEventPropertyAttributeValue(eventPropertyAttribute.value());

									if (boost::algorithm::iequals(workEventPropertyAttributeName, "condition"))
									{
										if (!matchingEngine.AddNewEventFilterCondition(workingEventID, 
											GeneralHelpers::StrToWStr(workingPropertyName),
											(EventFilterEvalGroup)workingFilterEvalGroup,
											MatchHelpers::GetEventFilterOperationBasedOnName(GeneralHelpers::StrToWStr(workEventPropertyAttributeValue)),
											GeneralHelpers::StrToWStr(workingPropertyValue)))
										{
											return ret;
										}
									}
								}
							}
						}
					}
				}
			}
			else
			{
				std::string workEventName(event.name());
				workingFilterEvalGroup = EventFilterEvalGroup::EVENT_FILTER_EVAL_INCLUDE_GROUP_OR;

				if (IsEventNameValid(GeneralHelpers::StrToWStr(workEventName)))
				{
					for (pugi::xml_attribute eventAttribute : event.attributes())
					{
						std::string workEventAttributeName(eventAttribute.name());
						std::string workEventAttributeValue(eventAttribute.value());
						if (boost::algorithm::iequals(workEventAttributeName, "scanners"))
						{

						}
					}

					for (pugi::xml_node eventProperty : event.children())
					{
						std::string workingPropertyName(eventProperty.name());
						std::string workingPropertyValue(eventProperty.first_child().value());

						if (!workingPropertyName.empty() &&
							!workingPropertyValue.empty() &&
							IsEventPropertyValid(GeneralHelpers::StrToWStr(workEventName),
							GeneralHelpers::StrToWStr(workingPropertyName)))
						{
							for (pugi::xml_attribute eventPropertyAttribute : eventProperty.attributes())
							{
								std::string workEventPropertyAttributeName(eventPropertyAttribute.name());
								std::string workEventPropertyAttributeValue(eventPropertyAttribute.value());

								if (boost::algorithm::iequals(workEventPropertyAttributeName, "condition"))
								{
									if (!matchingEngine.AddNewEventFilterCondition(workingEventID,
										GeneralHelpers::StrToWStr(workingPropertyName),
										(EventFilterEvalGroup)workingFilterEvalGroup,
										MatchHelpers::GetEventFilterOperationBasedOnName(GeneralHelpers::StrToWStr(workEventPropertyAttributeValue)),
										GeneralHelpers::StrToWStr(workingPropertyValue)))
									{
										return ret;
									}
								}
							}
						}
					}
				}
			}
		}

		ret = true;
	}

	return ret;
}