#include "common.h"
#include "eventhelpers/events_common.h"

namespace MatchHelpers
{
	//Parsing helper - New events IDs should be added here
	//Performance OK - Not called from matching loop
	SysmonXTypes::EventID GetEventIDBasedOnName(const SysmonXTypes::EventIDName &eventName)
	{
		EventID ret = EventID::SECURITY_EVENT_ID_NOT_ASSIGNED;

		if (!eventName.empty())
		{
			//Trace Backend Events - Sysmon
			if (boost::algorithm::equals(eventName, L"ProcessCreate"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS;
			}
			else if (boost::algorithm::equals(eventName, L"FileCreateTime"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_FILE_CREATE_TIME;
			}
			else if (boost::algorithm::equals(eventName, L"NetworkConnect"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_NETWORK_CONNECT;
			}
			else if (boost::algorithm::equals(eventName, L"SYSMON_SERVICE_STATE_CHANGE"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_SERVICE_STATE_CHANGE;
			}
			else if (boost::algorithm::equals(eventName, L"ProcessTerminate"))
			{
				ret = SECURITY_EVENT_ID_SYSMON_PROCESS_TERMINATE;
			}
			else if (boost::algorithm::equals(eventName, L"DriverLoad"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_DRIVER_LOAD;
			}
			else if (boost::algorithm::equals(eventName, L"ImageLoad"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_IMAGE_LOAD;
			}
			else if (boost::algorithm::equals(eventName, L"CreateRemoteThread"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_CREATE_REMOTE_THREAD;
			}
			else if (boost::algorithm::equals(eventName, L"RawAccessRead"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_RAW_ACCESS_READ;
			}
			else if (boost::algorithm::equals(eventName, L"ProcessAccess"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_PROCESS_ACCESS;
			}
			else if (boost::algorithm::equals(eventName, L"FileCreate"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_FILE_CREATE;
			}
			else if (boost::algorithm::equals(eventName, L"RegistryEvent"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_REGISTRY_EVENT;
			}
			else if (boost::algorithm::equals(eventName, L"FileCreateStreamHash"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_FILE_CREATE_STREAM_HASH;
			}
			else if (boost::algorithm::equals(eventName, L"PipeEvent"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_NAMEDPIPE_EVENT;
			}
			else if (boost::algorithm::equals(eventName, L"WmiEvent"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_WMI_EVENT;
			}
			else if (boost::algorithm::equals(eventName, L"DnsQuery"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMON_DNS_EVENT;
			}

			//SysmonX Events
			else if (boost::algorithm::equals(eventName, L"PowershellEvent"))
			{
				ret = EventID::SECURITY_EVENT_ID_SYSMONX_POWERSHELL_START_COMMAND;
			}
		}

		return ret;
	}

	//Parsinh helper - New Filter Operations should be added here
	//Performance OK - Not called from matching loop
	SysmonXTypes::EventFilterOperation GetEventFilterOperationBasedOnName(const SysmonXTypes::EventConditionFilterName &conditionName)
	{
		SysmonXTypes::EventFilterOperation ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_NA;

		if (!conditionName.empty())
		{
			if (boost::algorithm::iequals(conditionName, L"is"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_IS;
			}
			else if (boost::algorithm::iequals(conditionName, L"is not"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_IS_NOT;
			}
			else if (boost::algorithm::iequals(conditionName, L"contains"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_CONTAINS;
			}
			else if (boost::algorithm::iequals(conditionName, L"excludes"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_EXCLUDES;
			}
			else if (boost::algorithm::iequals(conditionName, L"begin with"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_BEGIN_WITH;
			}
			else if (boost::algorithm::iequals(conditionName, L"end with"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_END_WITH;
			}
			else if (boost::algorithm::iequals(conditionName, L"less than"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_LESS_THAN;
			}
			else if (boost::algorithm::iequals(conditionName, L"more than"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_MORE_THAN;
			}
			else if (boost::algorithm::iequals(conditionName, L"image"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_IMAGE;
			}
			else if (boost::algorithm::iequals(conditionName, L"regex"))
			{
				ret = SysmonXTypes::EventFilterOperation::EVENT_FILTER_OPERATION_REGEX_MATCH;
			}
		}

		return ret;
	}


	//Parsing Helper - New properties should be added here
	//Performance OK - Not called from matching loop
	SysmonXTypes::EventPropertyID GetEventPropertyIDBasedOnName(const SysmonXTypes::EventPropertyName &propertyName)
	{
		EventPropertyID ret = EventPropertyID::EVENT_PROPERTY_NA;

		if (!propertyName.empty())
		{
			//Common Section
			if (boost::algorithm::equals(propertyName, L"EventVersion"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_COMMON_Version;
			}
			else if (boost::algorithm::equals(propertyName, L"EventID"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_COMMON_EventID;
			}
			else if (boost::algorithm::equals(propertyName, L"EventCollectorTechID"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_COMMON_EventCollectorTechID;
			}
			else if (boost::algorithm::equals(propertyName, L"EventCollectorVectorID"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_COMMON_EventCollectorVectorID;
			}
			else if (boost::algorithm::equals(propertyName, L"EventCreationTimestamp"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_COMMON_EventCreationTimestamp;
			}
			else if (boost::algorithm::equals(propertyName, L"EventETWProviderID"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderID;
			}
			else if (boost::algorithm::equals(propertyName, L"EventETWProviderName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderName;
			}

			//Backend specifics - Sysmon
			else if (boost::algorithm::equals(propertyName, L"CallTrace"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_CallTrace;
			}
			else if (boost::algorithm::equals(propertyName, L"CommandLine"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_CommandLine;
			}
			else if (boost::algorithm::equals(propertyName, L"Company"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Company;
			}
			else if (boost::algorithm::equals(propertyName, L"Configuration"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Configuration;
			}
			else if (boost::algorithm::equals(propertyName, L"ConfigurationFileHash"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ConfigurationFileHash;
			}
			else if (boost::algorithm::equals(propertyName, L"Consumer"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Consumer;
			}
			else if (boost::algorithm::equals(propertyName, L"CreationUtcTime"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_CreationUtcTime;
			}
			else if (boost::algorithm::equals(propertyName, L"CurrentDirectory"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_CurrentDirectory;
			}
			else if (boost::algorithm::equals(propertyName, L"Description"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Description;
			}
			else if (boost::algorithm::equals(propertyName, L"Destination"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Destination;
			}
			else if (boost::algorithm::equals(propertyName, L"DestinationHostname"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_DestinationHostname;
			}
			else if (boost::algorithm::equals(propertyName, L"DestinationIp"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_DestinationIp;
			}
			else if (boost::algorithm::equals(propertyName, L"DestinationIsIpv6"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_DestinationIsIpv6;
			}
			else if (boost::algorithm::equals(propertyName, L"DestinationPort"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_DestinationPort;
			}
			else if (boost::algorithm::equals(propertyName, L"DestinationPortName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_DestinationPortName;
			}
			else if (boost::algorithm::equals(propertyName, L"Details"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Details;
			}
			else if (boost::algorithm::equals(propertyName, L"Device"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Device;
			}
			else if (boost::algorithm::equals(propertyName, L"EventNamespace"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_EventNamespace;
			}
			else if (boost::algorithm::equals(propertyName, L"EventType"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_EventType;
			}
			else if (boost::algorithm::equals(propertyName, L"FileVersion"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_FileVersion;
			}
			else if (boost::algorithm::equals(propertyName, L"Filter"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Filter;
			}
			else if (boost::algorithm::equals(propertyName, L"GrantedAccess"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_GrantedAccess;
			}
			else if (boost::algorithm::equals(propertyName, L"Hash"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Hash;
			}
			else if (boost::algorithm::equals(propertyName, L"Hashes"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Hashes;
			}
			else if (boost::algorithm::equals(propertyName, L"ID"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_EID;
			}
			else if (boost::algorithm::equals(propertyName, L"Image"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Image;
			}
			else if (boost::algorithm::equals(propertyName, L"ImageLoaded"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ImageLoaded;
			}
			else if (boost::algorithm::equals(propertyName, L"Initiated"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Initiated;
			}
			else if (boost::algorithm::equals(propertyName, L"IntegrityLevel"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_IntegrityLevel;
			}
			else if (boost::algorithm::equals(propertyName, L"LogonGuid"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_LogonGuid;
			}
			else if (boost::algorithm::equals(propertyName, L"LogonId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_LogonId;
			}
			else if (boost::algorithm::equals(propertyName, L"Name"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Name;
			}
			else if (boost::algorithm::equals(propertyName, L"NewName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_NewName;
			}
			else if (boost::algorithm::equals(propertyName, L"NewThreadId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_NewThreadId;
			}
			else if (boost::algorithm::equals(propertyName, L"Operation"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Operation;
			}
			else if (boost::algorithm::equals(propertyName, L"OriginalFileName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_OriginalFileName;
			}
			else if (boost::algorithm::equals(propertyName, L"ParentCommandLine"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ParentCommandLine;
			}
			else if (boost::algorithm::equals(propertyName, L"ParentImage"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ParentImage;
			}
			else if (boost::algorithm::equals(propertyName, L"ParentProcessGuid"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ParentProcessGuid;
			}
			else if (boost::algorithm::equals(propertyName, L"ParentProcessId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ParentProcessId;
			}
			else if (boost::algorithm::equals(propertyName, L"PipeName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_PipeName;
			}
			else if (boost::algorithm::equals(propertyName, L"PreviousCreationUtcTime"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_PreviousCreationUtcTime;
			}
			else if (boost::algorithm::equals(propertyName, L"ProcessGuid"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ProcessGuid;
			}
			else if (boost::algorithm::equals(propertyName, L"ProcessId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_ProcessId;
			}
			else if (boost::algorithm::equals(propertyName, L"Product"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Product;
			}
			else if (boost::algorithm::equals(propertyName, L"Protocol"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Protocol;
			}
			else if (boost::algorithm::equals(propertyName, L"Query"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Query;
			}
			else if (boost::algorithm::equals(propertyName, L"QueryName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_QueryName;
			}
			else if (boost::algorithm::equals(propertyName, L"QueryResults"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_QueryResults;
			}
			else if (boost::algorithm::equals(propertyName, L"QueryStatus"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_QueryStatus;
			}
			else if (boost::algorithm::equals(propertyName, L"RuleName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_RuleName;
			}
			else if (boost::algorithm::equals(propertyName, L"SchemaVersion"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SchemaVersion;
			}
			else if (boost::algorithm::equals(propertyName, L"Signature"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Signature;
			}
			else if (boost::algorithm::equals(propertyName, L"SignatureStatus"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SignatureStatus;
			}
			else if (boost::algorithm::equals(propertyName, L"Signed"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Signed;
			}
			else if (boost::algorithm::equals(propertyName, L"SourceHostname"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourceHostname;
			}
			else if (boost::algorithm::equals(propertyName, L"SourceImage"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourceImage;
			}
			else if (boost::algorithm::equals(propertyName, L"SourceIp"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourceIp;
			}
			else if (boost::algorithm::equals(propertyName, L"SourceIsIpv6"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourceIsIpv6;
			}
			else if (boost::algorithm::equals(propertyName, L"SourcePort"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourcePort;
			}
			else if (boost::algorithm::equals(propertyName, L"SourcePortName"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourcePortName;
			}
			else if (boost::algorithm::equals(propertyName, L"SourceProcessGuid"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourceProcessGuid;
			}
			else if (boost::algorithm::equals(propertyName, L"SourceProcessId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourceProcessId;
			}
			else if (boost::algorithm::equals(propertyName, L"SourceThreadId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_SourceThreadId;
			}
			else if (boost::algorithm::equals(propertyName, L"StartAddress"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_StartAddress;
			}
			else if (boost::algorithm::equals(propertyName, L"StartFunction"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_StartFunction;
			}
			else if (boost::algorithm::equals(propertyName, L"StartModule"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_StartModule;
			}
			else if (boost::algorithm::equals(propertyName, L"State"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_State;
			}
			else if (boost::algorithm::equals(propertyName, L"TargetFilename"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_TargetFilename;
			}
			else if (boost::algorithm::equals(propertyName, L"TargetImage"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_TargetImage;
			}
			else if (boost::algorithm::equals(propertyName, L"TargetObject"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_TargetObject;
			}
			else if (boost::algorithm::equals(propertyName, L"TargetProcessGuid"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_TargetProcessGuid;
			}
			else if (boost::algorithm::equals(propertyName, L"TargetProcessId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_TargetProcessId;
			}
			else if (boost::algorithm::equals(propertyName, L"TerminalSessionId"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_TerminalSessionId;
			}
			else if (boost::algorithm::equals(propertyName, L"Type"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Type;
			}
			else if (boost::algorithm::equals(propertyName, L"User"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_User;
			}
			else if (boost::algorithm::equals(propertyName, L"UtcTime"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_UtcTime;
			}
			else if (boost::algorithm::equals(propertyName, L"Version"))
			{
				ret = EventPropertyID::EVENT_PROPERTY_Version;
			}

			//SysmonX specifics
			else if (propertyName == L"ScriptBlock")
			{
				ret = EventPropertyID::EVENT_PROPERTY_ScriptBlockText;
			}
		}

		return ret;
	}

	//Performance SENSITIVE - This gets executed inside of the matching loop
	//TODO: mjo - Rethink this entire thing - Consider moving performance overhead to event sensor allocator instead of paying performance cost here 
	//TODO: mjo - fix incomplete conversions
	//As a way to return data from already event objec and avoid unnecesary constructors, a reference is returned here
	//We are using composed types and overloaded () operators to do this trick
	const SysmonXTypes::MATCHING_TYPE_STRING& GetMatchingDataFromEvent(const SysmonXTypes::EventObject &eventData, const SysmonXTypes::EventPropertyID &eventID)
	{
		static const MATCHING_TYPE_STRING ret;

		if (eventData)
		{
			//Common Section
			if (eventID == EventPropertyID::EVENT_PROPERTY_COMMON_Version)
			{
				return eventData->EventVersion.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_COMMON_EventID)
			{
				return eventData->EventID.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_COMMON_EventCollectorTechID)
			{
				return eventData->EventCollectorTechID.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_COMMON_EventCollectorVectorID)
			{
				return eventData->EventCollectorVectorID.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_COMMON_EventCreationTimestamp)
			{
				return eventData->EventCreationTimestamp.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderID)
			{
				return eventData->EventCommonProcessID.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_COMMON_EventETWProviderName)
			{
				return eventData->EventETWProviderName;
			}

			//Backend specifics - Sysmon
			else if (eventID == EventPropertyID::EVENT_PROPERTY_CallTrace)
			{
				return eventData->CallTrace;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_CommandLine)
			{
				return eventData->CommandLine;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Company)
			{
				return eventData->Company;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Configuration)
			{
				return eventData->Configuration;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ConfigurationFileHash)
			{
				return eventData->ConfigurationFileHash;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Consumer)
			{
				return eventData->Consumer;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_CreationUtcTime)
			{
				return eventData->CreationUtcTime;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_CurrentDirectory)
			{
				return eventData->CurrentDirectory;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Description)
			{
				return eventData->Description;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Destination)
			{
				return eventData->Destination;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_DestinationHostname)
			{
				return eventData->DestinationHostname;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_DestinationIp)
			{
				return eventData->DestinationIp;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_DestinationIsIpv6)
			{
				return eventData->DestinationIsIpv6.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_DestinationPort)
			{
				return eventData->DestinationPort.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_DestinationPortName)
			{
				return eventData->DestinationPortName;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Details)
			{
				return eventData->Details;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Device)
			{
				return eventData->Device;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_EventNamespace)
			{
				return eventData->EventNamespace;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_EventType)
			{
				return eventData->EventType;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_FileVersion)
			{
				return eventData->FileVersion;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Filter)
			{
				return eventData->Filter;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_GrantedAccess)
			{
				return eventData->GrantedAccess.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Hash)
			{
				return eventData->Hash;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Hashes)
			{
				return eventData->Hashes;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_EID)
			{
				return eventData->ErrorID;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Image)
			{
				return eventData->Image;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ImageLoaded)
			{
				return eventData->ImageLoaded;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Initiated)
			{
				return eventData->Initiated.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_IntegrityLevel)
			{
				return eventData->IntegrityLevel;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_LogonGuid)
			{
				return eventData->LogonGuid.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_LogonId)
			{
				return eventData->LogonId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Name)
			{
				return eventData->Name;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_NewName)
			{
				return eventData->NewName;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_NewThreadId)
			{
				return eventData->NewThreadId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Operation)
			{
				return eventData->Operation;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_OriginalFileName)
			{
				return eventData->OriginalFileName;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ParentCommandLine)
			{
				return eventData->ParentCommandLine;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ParentImage)
			{
				return eventData->ParentImage;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ParentProcessGuid)
			{
				return eventData->ParentProcessGuid.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ParentProcessId)
			{
				return eventData->ParentProcessId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_PipeName)
			{
				return eventData->PipeName;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_PreviousCreationUtcTime)
			{
				return eventData->PreviousCreationUtcTime;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ProcessGuid)
			{
				return eventData->ProcessGuid.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ProcessId)
			{
				return eventData->ProcessId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Product)
			{
				return eventData->Product;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Protocol)
			{
				return eventData->Protocol;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Query)
			{
				return eventData->Query;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_QueryName)
			{
				return eventData->QueryName;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_QueryResults)
			{
				return eventData->QueryResults;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_QueryStatus)
			{
				return eventData->QueryStatus;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_RuleName)
			{
				return eventData->RuleName;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SchemaVersion)
			{
				return eventData->SchemaVersion;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Signature)
			{
				return eventData->Signature;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SignatureStatus)
			{
				return eventData->SignatureStatus;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Signed)
			{
				return eventData->Signed;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourceHostname)
			{
				return eventData->SourceHostname;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourceImage)
			{
				return eventData->SourceImage;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourceIp)
			{
				return eventData->SourceIp;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourceIsIpv6)
			{
				return eventData->SourceIsIpv6.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourcePort)
			{
				return eventData->SourcePort.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourcePortName)
			{
				return eventData->SourcePortName;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourceProcessGuid)
			{
				return eventData->SourceProcessGuid.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourceProcessId)
			{
				return eventData->SourceProcessId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_SourceThreadId)
			{
				return eventData->SourceThreadId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_StartAddress)
			{
				return eventData->StartAddress;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_StartFunction)
			{
				return eventData->StartFunction;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_StartModule)
			{
				return eventData->StartModule;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_State)
			{
				return eventData->State;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_TargetFilename)
			{
				return eventData->TargetFilename;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_TargetImage)
			{
				return eventData->TargetImage;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_TargetObject)
			{
				return eventData->TargetObject;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_TargetProcessGuid)
			{
				return eventData->TargetProcessGuid.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_TargetProcessId)
			{
				return eventData->TargetProcessId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_TerminalSessionId)
			{
				return eventData->TerminalSessionId.GetWString();
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Type)
			{
				return eventData->Type;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_User)
			{
				return eventData->User;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_UtcTime)
			{
				return eventData->UtcTime;
			}
			else if (eventID == EventPropertyID::EVENT_PROPERTY_Version)
			{
				return eventData->Version;
			}

			//SysmonX specifics
			else if (eventID == EventPropertyID::EVENT_PROPERTY_ScriptBlockText)
			{
				return eventData->FreeText;
			}
		}

		return ret;
	}



}