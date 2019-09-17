#include "common.h"

bool RulesManager::Initialize()
{
	bool ret = true;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	logger.Trace("RulesManager::Initialize - About to initialize rules manager");

	if (config.IsInitialized())
	{

		if (!IsInitialized())
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
			FileCreateTime.push_back(L"RuleName");
			FileCreateTime.push_back(L"UtcTime");
			FileCreateTime.push_back(L"ProcessGuid");
			FileCreateTime.push_back(L"ProcessId");
			FileCreateTime.push_back(L"Image");
			FileCreateTime.push_back(L"TargetFilename");
			FileCreateTime.push_back(L"CreationUtcTime");
			FileCreateTime.push_back(L"PreviousCreationUtcTime");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_TIME, FileCreateTime));

			EventPropertyNameContainer NetworkConnect;
			NetworkConnect.push_back(L"RuleName");
			NetworkConnect.push_back(L"UtcTime");
			NetworkConnect.push_back(L"ProcessGuid");
			NetworkConnect.push_back(L"ProcessId");
			NetworkConnect.push_back(L"Image");
			NetworkConnect.push_back(L"User");
			NetworkConnect.push_back(L"Protocol");
			NetworkConnect.push_back(L"Initiated");
			NetworkConnect.push_back(L"SourceIsIpv6");
			NetworkConnect.push_back(L"SourceIp");
			NetworkConnect.push_back(L"SourceHostname");
			NetworkConnect.push_back(L"SourcePort");
			NetworkConnect.push_back(L"SourcePortName");
			NetworkConnect.push_back(L"DestinationIsIpv6");
			NetworkConnect.push_back(L"DestinationIp");
			NetworkConnect.push_back(L"DestinationHostname");
			NetworkConnect.push_back(L"DestinationPort");
			NetworkConnect.push_back(L"DestinationPortName");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_NETWORK_CONNECT, NetworkConnect));

			EventPropertyNameContainer ProcessTerminate;
			ProcessTerminate.push_back(L"RuleName");
			ProcessTerminate.push_back(L"UtcTime");
			ProcessTerminate.push_back(L"ProcessGuid");
			ProcessTerminate.push_back(L"ProcessId");
			ProcessTerminate.push_back(L"Image");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_TERMINATE, ProcessTerminate));

			EventPropertyNameContainer DriverLoad;
			DriverLoad.push_back(L"RuleName");
			DriverLoad.push_back(L"UtcTime");
			DriverLoad.push_back(L"ImageLoaded");
			DriverLoad.push_back(L"Hashes");
			DriverLoad.push_back(L"Signed");
			DriverLoad.push_back(L"Signature");
			DriverLoad.push_back(L"SignatureStatus");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_DRIVER_LOADER, DriverLoad));

			EventPropertyNameContainer ImageLoad;
			ImageLoad.push_back(L"RuleName");
			ImageLoad.push_back(L"UtcTime");
			ImageLoad.push_back(L"ProcessGuid");
			ImageLoad.push_back(L"ProcessId");
			ImageLoad.push_back(L"Image");
			ImageLoad.push_back(L"ImageLoaded");
			ImageLoad.push_back(L"FileVersion");
			ImageLoad.push_back(L"Description");
			ImageLoad.push_back(L"Product");
			ImageLoad.push_back(L"Company");
			ImageLoad.push_back(L"OriginalFileName");
			ImageLoad.push_back(L"Hashes");
			ImageLoad.push_back(L"Signed");
			ImageLoad.push_back(L"Signature");
			ImageLoad.push_back(L"SignatureStatus");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_IMAGE_LOAD, ImageLoad));

			EventPropertyNameContainer CreateRemoteThread;
			CreateRemoteThread.push_back(L"RuleName");
			CreateRemoteThread.push_back(L"UtcTime");
			CreateRemoteThread.push_back(L"SourceProcessGuid");
			CreateRemoteThread.push_back(L"SourceProcessId");
			CreateRemoteThread.push_back(L"SourceImage");
			CreateRemoteThread.push_back(L"TargetProcessGuid");
			CreateRemoteThread.push_back(L"TargetProcessId");
			CreateRemoteThread.push_back(L"TargetImage");
			CreateRemoteThread.push_back(L"NewThreadId");
			CreateRemoteThread.push_back(L"StartAddress");
			CreateRemoteThread.push_back(L"StartModule");
			CreateRemoteThread.push_back(L"StartFunction");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_CREATE_REMOTE_THREAD, CreateRemoteThread));

			EventPropertyNameContainer RawAccessRead;
			RawAccessRead.push_back(L"RuleName");
			RawAccessRead.push_back(L"UtcTime");
			RawAccessRead.push_back(L"ProcessGuid");
			RawAccessRead.push_back(L"ProcessId");
			RawAccessRead.push_back(L"Image");
			RawAccessRead.push_back(L"Device");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_RAW_ACCESS_READ, RawAccessRead));

			EventPropertyNameContainer ProcessAccess;
			ProcessAccess.push_back(L"RuleName");
			ProcessAccess.push_back(L"UtcTime");
			ProcessAccess.push_back(L"SourceProcessGUID");
			ProcessAccess.push_back(L"SourceProcessId");
			ProcessAccess.push_back(L"SourceThreadId");
			ProcessAccess.push_back(L"SourceImage");
			ProcessAccess.push_back(L"TargetProcessGUID");
			ProcessAccess.push_back(L"TargetProcessId");
			ProcessAccess.push_back(L"TargetImage");
			ProcessAccess.push_back(L"GrantedAccess");
			ProcessAccess.push_back(L"CallTrace");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PROCESS_ACCESS, ProcessAccess));

			EventPropertyNameContainer FileCreate;
			FileCreate.push_back(L"RuleName");
			FileCreate.push_back(L"UtcTime");
			FileCreate.push_back(L"ProcessGuid");
			FileCreate.push_back(L"ProcessId");
			FileCreate.push_back(L"Image");
			FileCreate.push_back(L"TargetFilename");
			FileCreate.push_back(L"CreationUtcTime");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE, FileCreate));

			EventPropertyNameContainer RegistryEvent;
			RegistryEvent.push_back(L"RuleName");
			RegistryEvent.push_back(L"EventType");
			RegistryEvent.push_back(L"UtcTime");
			RegistryEvent.push_back(L"ProcessGuid");
			RegistryEvent.push_back(L"ProcessId");
			RegistryEvent.push_back(L"Image");
			RegistryEvent.push_back(L"TargetObject");
			RegistryEvent.push_back(L"Details");
			RegistryEvent.push_back(L"NewName");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_REGISTRY_EVENT, RegistryEvent));

			EventPropertyNameContainer FileCreateStreamHash;
			FileCreateStreamHash.push_back(L"RuleName");
			FileCreateStreamHash.push_back(L"UtcTime");
			FileCreateStreamHash.push_back(L"ProcessGuid");
			FileCreateStreamHash.push_back(L"ProcessId");
			FileCreateStreamHash.push_back(L"Image");
			FileCreateStreamHash.push_back(L"TargetFilename");
			FileCreateStreamHash.push_back(L"CreationUtcTime");
			FileCreateStreamHash.push_back(L"Hash");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_STREAM, FileCreateStreamHash));

			EventPropertyNameContainer PipeEvent;
			PipeEvent.push_back(L"RuleName");
			PipeEvent.push_back(L"EventType");
			PipeEvent.push_back(L"UtcTime");
			PipeEvent.push_back(L"ProcessGuid");
			PipeEvent.push_back(L"ProcessId");
			PipeEvent.push_back(L"PipeName");
			PipeEvent.push_back(L"Image");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_PIPE_EVENT, PipeEvent));

			EventPropertyNameContainer WmiEvent;
			WmiEvent.push_back(L"RuleName");
			WmiEvent.push_back(L"EventType");
			WmiEvent.push_back(L"UtcTime");
			WmiEvent.push_back(L"Operation");
			WmiEvent.push_back(L"User");
			WmiEvent.push_back(L"EventNamespace");
			WmiEvent.push_back(L"Name");
			WmiEvent.push_back(L"Query");
			WmiEvent.push_back(L"Type");
			WmiEvent.push_back(L"Destination");
			WmiEvent.push_back(L"Consumer");
			WmiEvent.push_back(L"Filter");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_WMI_EVENT, WmiEvent));

			EventPropertyNameContainer DNSQueryEvent;
			DNSQueryEvent.push_back(L"RuleName");
			DNSQueryEvent.push_back(L"UtcTime");
			DNSQueryEvent.push_back(L"ProcessGuid");
			DNSQueryEvent.push_back(L"ProcessId");
			DNSQueryEvent.push_back(L"QueryName");
			DNSQueryEvent.push_back(L"QueryStatus");
			DNSQueryEvent.push_back(L"QueryResults");
			DNSQueryEvent.push_back(L"Image");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMON_BACKEND_NAME_DNS_QUERY, DNSQueryEvent));

			//Adding SysmonX Supported Events Properties
			EventPropertyNameContainer PowershellEvent;
			PowershellEvent.push_back(L"ScriptBlock");
			m_validEventProperties.insert(std::make_pair(SysmonXDefs::EVENT_SYSMONX_NAME_POWERSHELL, PowershellEvent));

			//Adding SysmonX Aggregation Options;
			m_validEventsAggregation.push_back(SysmonXDefs::EVENT_SCANNER_AGGREATION_MEM_HOLLOW_MODULES);
			m_validEventsAggregation.push_back(SysmonXDefs::EVENT_SCANNER_AGGREATION_PARENT_SPOOFING);
			m_validEventsAggregation.push_back(SysmonXDefs::EVENT_SCANNER_AGGREATION_CMD_SPOOFING);

			m_isInitialized = true;
			ret = true;
		}
		else
		{
			ret = true;
		}
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

	bool shouldBreak = false;
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
						shouldBreak = true;
						break;
					}
				}

				if (shouldBreak)
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


bool RulesManager::IsConfigXMLEventValid(const pugi::xml_node &event)
{
	bool ret = false;

	std::string workEventName(event.name());

	if (!workEventName.empty() &&
		IsEventNameValid(GeneralHelpers::StrToWStr(workEventName)))
	{
		for (pugi::xml_attribute eventAttribute : event.attributes())
		{
			std::string workEventAttributeName(eventAttribute.name());
			std::string workEventAttributeValue(eventAttribute.value());

			//TODO: Add validation here
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

					//TODO: Add validation here
				}
			}
			else
			{
				return ret;
			}
		}

		ret = true;
	}

	return ret;
}

bool RulesManager::ProcessConfigXMLEvent(const pugi::xml_node &event, const UINT32 &workingFilterEvalGroup)
{
	bool ret = false;
	MatchingEngine &matchingEngine = MatchingEngine::Instance();
	EventID workingEventID = MatchHelpers::GetEventIDBasedOnName(GeneralHelpers::StrToWStr(event.name()));
	std::string workEventName(event.name());

	if (!workEventName.empty() && IsEventNameValid(GeneralHelpers::StrToWStr(workEventName)))
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

		ret = true;
	}

	return ret;
}


//TODO: Perform this using a xml library that supports dtd/xsd schema validation.
//XML validation for the poor below
bool RulesManager::IsConfigFileSyntaxValid()
{
	bool ret = false;
	pugi::xml_document doc;

	if (m_config.IsInitialized())
	{
		std::string workingContent(GeneralHelpers::WStrToStr(m_config.GetConfigurationFileContent()));
		if (!workingContent.empty() &&
			doc.load_buffer(workingContent.c_str(), workingContent.size()))
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
						if (!IsConfigXMLEventValid(eventInRuleGroup))
						{
							return ret;
						}
					}
				}
				else
				{
					if (!IsConfigXMLEventValid(event))
					{
						return ret;
					}
				}
			}

			ret = true;
		}
	}

	return ret;
}

bool RulesManager::LoadNewRulesContent()
{
	bool ret = false;

	pugi::xml_document doc;
	MatchingEngine &matchingEngine = MatchingEngine::Instance();

	if (m_config.IsInitialized() && 
		matchingEngine.IsInitialized())
	{
		//Get ConfigurationFileContent
		std::string workingContent(GeneralHelpers::WStrToStr(m_config.GetConfigurationFileContent()));

		//Cleaning previous content
		matchingEngine.ClearEventFilters();

		//Loading new configuration
		if (!workingContent.empty() &&
			doc.load_buffer(workingContent.c_str(), workingContent.size()))
		{
			//Traversing until EventFiltering tag
			pugi::xml_node eventFilteringNode = doc.child("Sysmon").child("EventFiltering");

			//parsing events, one by one
			for (pugi::xml_node event : eventFilteringNode)
			{
				EventID workingEventID = MatchHelpers::GetEventIDBasedOnName(GeneralHelpers::StrToWStr(event.name()));
				UINT32 workingFilterEvalGroup = EventFilterEvalGroup::EVENT_FILTER_EVAL_INCLUDE_GROUP_OR;

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
						if (!ProcessConfigXMLEvent(eventInRuleGroup, workingFilterEvalGroup))
						{
							return ret;
						}
					}
				}
				else
				{
					if (!ProcessConfigXMLEvent(event, workingFilterEvalGroup))
					{
						return ret;
					}
				}
			}

			ret = true;
		}
	}


	return ret;
}