#pragma once

#include "common.h"


namespace ETWProvidersName
{
	static const wchar_t* SYSMON_PROVIDER_NAME = L"Microsoft-Windows-Sysmon";
	static const wchar_t* POWERSHELL_PROVIDER_NAME = L"Microsoft-Windows-PowerShell";
}

namespace SysmonXDefs
{
	static const UINT32 CURRENT_EVENT_VERSION = 1;
	static const UINT32 INVALID_PROCESS_ID = 0;
	static const UINT32 DEFAULT_OPCODE = 0;
	static const LARGE_INTEGER INVALID_TIMESTAMP = { 0 };
	static const UINT32 INVALID_EVENT_ID = 65535;

	//Events Operations
	static const std::wstring EVENT_FILTER_OPERATION_STR_IS = L"is";
	static const std::wstring EVENT_FILTER_OPERATION_STR_IS_NOT = L"is not";
	static const std::wstring EVENT_FILTER_OPERATION_STR_CONTAINS = L"contains";
	static const std::wstring EVENT_FILTER_OPERATION_STR_EXCLUDES = L"excludes";
	static const std::wstring EVENT_FILTER_OPERATION_STR_BEGIN_WITH = L"begin with";
	static const std::wstring EVENT_FILTER_OPERATION_STR_END_WITH = L"end with";
	static const std::wstring EVENT_FILTER_OPERATION_STR_LESS_THAN = L"less than";
	static const std::wstring EVENT_FILTER_OPERATION_STR_MORE_THAN = L"more than";
	static const std::wstring EVENT_FILTER_OPERATION_STR_IMAGE = L"image";
	static const std::wstring EVENT_FILTER_OPERATION_STR_REGEX_MATCH = L"regex";

	//Events Names - Trace Backend
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_PROCESS_CREATE = L"ProcessCreate";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_TIME = L"FileCreateTime";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_NETWORK_CONNECT = L"NetworkConnect";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_PROCESS_TERMINATE = L"ProcessTerminate";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_DRIVER_LOADER = L"DriverLoad";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_IMAGE_LOAD = L"ImageLoad";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_CREATE_REMOTE_THREAD = L"CreateRemoteThread";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_RAW_ACCESS_READ = L"RawAccessRead";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_PROCESS_ACCESS = L"ProcessAccess";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_FILE_CREATE = L"FileCreate";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_REGISTRY_EVENT = L"RegistryEvent";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_FILE_CREATE_STREAM = L"FileCreateStreamHash";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_PIPE_EVENT = L"PipeEvent";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_WMI_EVENT = L"WmiEvent";
	static const std::wstring EVENT_SYSMON_BACKEND_NAME_DNS_QUERY = L"DnsQuery";

	//Events Names - SysmonX 
	static const std::wstring EVENT_SYSMONX_NAME_POWERSHELL = L"PowershellEvent";
}

namespace SysmonXTypes
{
	//Security Events Types
	typedef std::wstring		MATCHING_TYPE_STRING;
	typedef UINT64				MATCHING_TYPE_UINT64;
	typedef UINT32				MATCHING_TYPE_UINT32;
	typedef UINT16				MATCHING_TYPE_UINT16;
	typedef UINT8				MATCHING_TYPE_UINT8;
	typedef std::wstring		EventPropertyName;
	typedef std::wstring		EventIDName;
	typedef std::wstring		EventWString;
	typedef std::string			EventString;
	typedef bool				EventBool;
	typedef UINT16				EventUINT16;
	typedef UINT32				EventUINT32;
	typedef UINT64				EventUINT64;
	typedef LARGE_INTEGER		EventLARGEINT;
	typedef GUID				EventGUID;
	typedef std::vector<bool>	EventArrayBool;
	typedef std::vector<UINT16> EventArray16;
	typedef std::vector<UINT32> EventArray32;
	typedef std::vector<UINT64> EventArray64;

	typedef std::wstring		EventPropertyName;
	typedef std::vector<EventPropertyName> EventPropertyNameContainer;
	typedef std::wstring		EventConditionFilterName;
	typedef std::wstring		EventName;
	typedef std::wstring		EventAggregationName;


	//Tracking Sysmon Backend Options Flags
	enum SysmonBackendOptionFlags
	{
		SYSMON_OPTIONS_ALL_DISABLED = 0x00,
		SYSMON_OPTIONS_NETWORK_TRACKING_ENABLED = 0x01,
		SYSMON_OPTIONS_IMAGE_LOADING_ENABLED = 0x02,
		SYSMON_OPTIONS_IMAGE_NA,
	};

	enum EventFilterEvalGroup
	{
		EVENT_FILTER_EVAL_INCLUDE_GROUP_AND = 0,
		EVENT_FILTER_EVAL_INCLUDE_GROUP_OR,
		EVENT_FILTER_EVAL_EXCLUDE_GROUP,
		EVENT_FILTER_EVAL_NA,
	};

	enum EventFilterOperation
	{
		EVENT_FILTER_OPERATION_IS = 0,
		EVENT_FILTER_OPERATION_IS_NOT,
		EVENT_FILTER_OPERATION_CONTAINS,
		EVENT_FILTER_OPERATION_EXCLUDES,
		EVENT_FILTER_OPERATION_BEGIN_WITH,
		EVENT_FILTER_OPERATION_END_WITH,
		EVENT_FILTER_OPERATION_LESS_THAN,
		EVENT_FILTER_OPERATION_MORE_THAN,
		EVENT_FILTER_OPERATION_IMAGE,
		EVENT_FILTER_OPERATION_REGEX_MATCH,
		EVENT_FILTER_OPERATION_NA
	};

	typedef std::vector<MATCHING_TYPE_STRING> EventFilterContainer;
	typedef std::vector<boost::wregex> EventFilterContainerRegex;
	typedef std::vector<MATCHING_TYPE_UINT64> EventFilterContainerInt;

	//Available Events Collector IDs
	enum EventCollectorTechID
	{
		EVENT_COLLECTOR_TECH_ETW = 0x00,
		EVENT_COLLECTOR_TECH_DUMMY,
		EVENT_COLLECTOR_TECH_NA
	};


	enum EventCollectorVectorID
	{
		EVENT_SUBTYPE_ETW_SYSMON = 0x00,
		EVENT_SUBTYPE_ETW_USER_CUSTOM,
		EVENT_SUBTYPE_ETW_KERNEL,
		EVENT_SUBTYPE_NA
	};


	//Security Events Enumerations
	enum EventID
	{
		SECURITY_EVENT_ID_NOT_ASSIGNED = 0,

		//Sysmon related IDs
		SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS = 1,
		SECURITY_EVENT_ID_SYSMON_FILE_CREATE_TIME = 2,
		SECURITY_EVENT_ID_SYSMON_NETWORK_CONNECT = 3,
		SECURITY_EVENT_ID_SYSMON_SERVICE_STATE_CHANGE = 4,
		SECURITY_EVENT_ID_SYSMON_PROCESS_TERMINATE = 5,
		SECURITY_EVENT_ID_SYSMON_DRIVER_LOAD = 6,
		SECURITY_EVENT_ID_SYSMON_IMAGE_LOAD = 7,
		SECURITY_EVENT_ID_SYSMON_CREATE_REMOTE_THREAD = 8,
		SECURITY_EVENT_ID_SYSMON_RAW_ACCESS_READ = 9,
		SECURITY_EVENT_ID_SYSMON_PROCESS_ACCESS = 10,
		SECURITY_EVENT_ID_SYSMON_FILE_CREATE = 11,
		SECURITY_EVENT_ID_SYSMON_REGISTRY_EVENT = 12,
		SECURITY_EVENT_ID_SYSMON_REGISTRY_EVENT_SET_VALUE = 13,
		SECURITY_EVENT_ID_SYSMON_REGISTRY_EVENT_NAME = 14,
		SECURITY_EVENT_ID_SYSMON_FILE_CREATE_STREAM_HASH = 15,
		SECURITY_EVENT_ID_SYSMON_SERVICE_CONFIGURATION_CHANGE = 16,
		SECURITY_EVENT_ID_SYSMON_NAMEDPIPE_EVENT = 17,
		SECURITY_EVENT_ID_SYSMON_NAMEDPIPE_CONNECT = 18,
		SECURITY_EVENT_ID_SYSMON_WMI_EVENT = 19,
		SECURITY_EVENT_ID_SYSMON_DNS_EVENT = 22,
		SECURITY_EVENT_ID_SYSMON_ERROR = 255,

		//SysmonX related IDs
		SECURITY_EVENT_ID_SYSMONX_CREATE_PROCESS = 1000,
		SECURITY_EVENT_ID_SYSMONX_POWERSHELL_START_COMMAND = 1001,
		SECURITY_EVENT_ID_SYSMONX_AUTHENTICATION = 1002,

		//Last possible ID
		SECURITY_EVENT_NOT_VALID = 65535 //(USHRT_MAX)
	};


	//This object will hold the security event data
	//I've decided to go with big and single data object to hold security data for simplification
	//In the future I'll explore using passing polymorphic shared_ptr references around instead of a single object type
	struct EventData
	{
		//Common
		EventUINT32		EventVersion;
		EventUINT32		EventID;
		EventUINT32		EventCollectorTechID;
		EventUINT32		EventCollectorVectorID;
		EventLARGEINT	EventCreationTimestamp;
		EventUINT32		EventETWProviderPID;
		EventWString	EventETWProviderName;

		//Sysmon Generic
		EventWString	UtcTime;
		EventWString	ErrorID;
		EventWString	RuleName;
		EventUINT32		ProcessId;
		EventGUID		ProcessGuid;
		EventWString	Image;
		EventWString	FileVersion;
		EventWString	Description;
		EventWString	Product;
		EventWString	Company;
		EventWString	OriginalFileName;
		EventWString	CommandLine;
		EventWString	CurrentDirectory;
		EventWString	User;
		EventGUID		LogonGuid;
		EventArray64	LogonId;
		EventUINT32		TerminalSessionId;
		EventWString	IntegrityLevel;
		EventWString	Hashes;
		EventGUID		ParentProcessGuid;
		EventUINT32		ParentProcessId;
		EventWString	ParentImage;
		EventWString	ParentCommandLine;
		EventWString	TargetFilename;
		EventWString	CreationUtcTime;
		EventWString	PreviousCreationUtcTime;
		EventWString	Protocol;
		EventBool		Initiated;
		EventBool		SourceIsIpv6;
		EventWString	SourceIp;
		EventWString	SourceHostname;
		EventUINT16		SourcePort;
		EventWString	SourcePortName;
		EventBool		DestinationIsIpv6;
		EventWString	DestinationIp;
		EventWString	DestinationHostname;
		EventUINT16		DestinationPort;
		EventWString	DestinationPortName;
		EventWString	State;
		EventWString	Version;
		EventWString	SchemaVersion;
		EventWString	ImageLoaded;
		EventWString	Signed;
		EventWString	Signature;
		EventWString	SignatureStatus;
		EventGUID		SourceProcessGuid;
		EventUINT32		SourceProcessId;
		EventUINT32		SourceThreadId;
		EventWString	SourceImage;
		EventGUID		TargetProcessGuid;
		EventUINT32		TargetProcessId;
		EventWString	TargetImage;
		EventUINT32		NewThreadId;
		EventWString	StartAddress;
		EventWString	StartModule;
		EventWString	StartFunction;
		EventWString	Device;
		EventArray32	GrantedAccess;
		EventWString	CallTrace;
		EventWString	TargetObject;
		EventWString	Details;
		EventWString	NewName;
		EventWString	Hash;
		EventWString	Configuration;
		EventWString	ConfigurationFileHash;
		EventWString	EventType;
		EventWString	PipeName;
		EventWString	Operation;
		EventWString	EventNamespace;
		EventWString	Name;
		EventWString	Query;
		EventWString	Type;
		EventWString	Destination;
		EventWString	Consumer;
		EventWString	Filter;
		EventWString	QueryName;
		EventWString	QueryStatus;
		EventWString	QueryResults;

		//User Specific	
		EventWString	FreeText;
		EventWString	ScannerResult;

		//Default Data
		EventData():
			EventID(EventID::SECURITY_EVENT_ID_NOT_ASSIGNED),
			EventCollectorTechID(EventCollectorTechID::EVENT_COLLECTOR_TECH_NA),
			EventCollectorVectorID(EventCollectorVectorID::EVENT_SUBTYPE_NA),
			EventVersion(SysmonXDefs::CURRENT_EVENT_VERSION),
			EventCreationTimestamp(SysmonXDefs::INVALID_TIMESTAMP) 
		{
			memset(&ProcessGuid, 0, sizeof(TargetProcessGuid));
			memset(&LogonGuid, 0, sizeof(LogonGuid));
			memset(&ParentProcessGuid, 0, sizeof(ParentProcessGuid));
			memset(&SourceProcessGuid, 0, sizeof(SourceProcessGuid));
			memset(&TargetProcessGuid, 0, sizeof(TargetProcessGuid));
		}


		//EventID
		EventData(const EventUINT32 &eventID) :
			EventID(eventID),
			EventCollectorTechID(EventCollectorTechID::EVENT_COLLECTOR_TECH_NA),
			EventCollectorVectorID(EventCollectorVectorID::EVENT_SUBTYPE_NA),
			EventVersion(SysmonXDefs::CURRENT_EVENT_VERSION),
			EventCreationTimestamp(SysmonXDefs::INVALID_TIMESTAMP) 		
		{
			memset(&ProcessGuid, 0, sizeof(TargetProcessGuid));
			memset(&LogonGuid, 0, sizeof(LogonGuid));
			memset(&ParentProcessGuid, 0, sizeof(ParentProcessGuid));
			memset(&SourceProcessGuid, 0, sizeof(SourceProcessGuid));
			memset(&TargetProcessGuid, 0, sizeof(TargetProcessGuid));		
		}


		//EventID and TechID
		EventData(const EventUINT32 &eventID, const EventUINT32 &techID) :
			EventID(eventID),
			EventCollectorTechID(techID),
			EventCollectorVectorID(EventCollectorVectorID::EVENT_SUBTYPE_NA),
			EventVersion(SysmonXDefs::CURRENT_EVENT_VERSION),
			EventCreationTimestamp(SysmonXDefs::INVALID_TIMESTAMP)
		{
			memset(&ProcessGuid, 0, sizeof(TargetProcessGuid));
			memset(&LogonGuid, 0, sizeof(LogonGuid));
			memset(&ParentProcessGuid, 0, sizeof(ParentProcessGuid));
			memset(&SourceProcessGuid, 0, sizeof(SourceProcessGuid));
			memset(&TargetProcessGuid, 0, sizeof(TargetProcessGuid));
		}


		//EventID, TechID and VectorID
		EventData(const EventUINT32 &eventID, const EventUINT32 &techID, const EventUINT32 &vectorID) :
			EventID(eventID),
			EventCollectorTechID(techID),
			EventCollectorVectorID(vectorID),
			EventVersion(SysmonXDefs::CURRENT_EVENT_VERSION),
			EventCreationTimestamp(SysmonXDefs::INVALID_TIMESTAMP)
		{
			memset(&ProcessGuid, 0, sizeof(TargetProcessGuid));
			memset(&LogonGuid, 0, sizeof(LogonGuid));
			memset(&ParentProcessGuid, 0, sizeof(ParentProcessGuid));
			memset(&SourceProcessGuid, 0, sizeof(SourceProcessGuid));
			memset(&TargetProcessGuid, 0, sizeof(TargetProcessGuid));
		}
	};
	typedef std::shared_ptr<EventData> EventObject;


	//EventData Property ID
	enum EventPropertyID
	{
		//Common Payload
		EVENT_PROPERTY_COMMON_Version = 0,
		EVENT_PROPERTY_COMMON_EventID,
		EVENT_PROPERTY_COMMON_EventCollectorTechID,
		EVENT_PROPERTY_COMMON_EventCollectorVectorID,
		EVENT_PROPERTY_COMMON_EventCreationTimestamp,
		EVENT_PROPERTY_COMMON_EventETWProviderPID,
		EVENT_PROPERTY_COMMON_EventETWProviderName,

		//Generic Sysmon Events Payload
		EVENT_PROPERTY_CallTrace,
		EVENT_PROPERTY_CommandLine,
		EVENT_PROPERTY_Company,
		EVENT_PROPERTY_Configuration,
		EVENT_PROPERTY_ConfigurationFileHash,
		EVENT_PROPERTY_Consumer,
		EVENT_PROPERTY_CreationUtcTime,
		EVENT_PROPERTY_CurrentDirectory,
		EVENT_PROPERTY_Description,
		EVENT_PROPERTY_Destination,
		EVENT_PROPERTY_DestinationHostname,
		EVENT_PROPERTY_DestinationIp,
		EVENT_PROPERTY_DestinationIsIpv6,
		EVENT_PROPERTY_DestinationPort,
		EVENT_PROPERTY_DestinationPortName,
		EVENT_PROPERTY_Details,
		EVENT_PROPERTY_Device,
		EVENT_PROPERTY_EventNamespace,
		EVENT_PROPERTY_EventType,
		EVENT_PROPERTY_FileVersion,
		EVENT_PROPERTY_Filter,
		EVENT_PROPERTY_GrantedAccess,
		EVENT_PROPERTY_Hash,
		EVENT_PROPERTY_Hashes,
		EVENT_PROPERTY_EID,
		EVENT_PROPERTY_Image,
		EVENT_PROPERTY_ImageLoaded,
		EVENT_PROPERTY_Initiated,
		EVENT_PROPERTY_IntegrityLevel,
		EVENT_PROPERTY_LogonGuid,
		EVENT_PROPERTY_LogonId,
		EVENT_PROPERTY_Name,
		EVENT_PROPERTY_NewName,
		EVENT_PROPERTY_NewThreadId,
		EVENT_PROPERTY_Operation,
		EVENT_PROPERTY_OriginalFileName,
		EVENT_PROPERTY_ParentCommandLine,
		EVENT_PROPERTY_ParentImage,
		EVENT_PROPERTY_ParentProcessGuid,
		EVENT_PROPERTY_ParentProcessId,
		EVENT_PROPERTY_PipeName,
		EVENT_PROPERTY_PreviousCreationUtcTime,
		EVENT_PROPERTY_ProcessGuid,
		EVENT_PROPERTY_ProcessId,
		EVENT_PROPERTY_Product,
		EVENT_PROPERTY_Protocol,
		EVENT_PROPERTY_Query,
		EVENT_PROPERTY_QueryName,
		EVENT_PROPERTY_QueryResults,
		EVENT_PROPERTY_QueryStatus,
		EVENT_PROPERTY_RuleName,
		EVENT_PROPERTY_SchemaVersion,
		EVENT_PROPERTY_Signature,
		EVENT_PROPERTY_SignatureStatus,
		EVENT_PROPERTY_Signed,
		EVENT_PROPERTY_SourceHostname,
		EVENT_PROPERTY_SourceImage,
		EVENT_PROPERTY_SourceIp,
		EVENT_PROPERTY_SourceIsIpv6,
		EVENT_PROPERTY_SourcePort,
		EVENT_PROPERTY_SourcePortName,
		EVENT_PROPERTY_SourceProcessGuid,
		EVENT_PROPERTY_SourceProcessId,
		EVENT_PROPERTY_SourceThreadId,
		EVENT_PROPERTY_StartAddress,
		EVENT_PROPERTY_StartFunction,
		EVENT_PROPERTY_StartModule,
		EVENT_PROPERTY_State,
		EVENT_PROPERTY_TargetFilename,
		EVENT_PROPERTY_TargetImage,
		EVENT_PROPERTY_TargetObject,
		EVENT_PROPERTY_TargetProcessGuid,
		EVENT_PROPERTY_TargetProcessId,
		EVENT_PROPERTY_TerminalSessionId,
		EVENT_PROPERTY_Type,
		EVENT_PROPERTY_User,
		EVENT_PROPERTY_UtcTime,
		EVENT_PROPERTY_Version,

		//SysmonX Payload
		EVENT_PROPERTY_ScriptBlockText, //Powershell

		//Guard
		EVENT_PROPERTY_NA
	};


	//Report Channels
	enum ReportChannelID
	{
		REPORT_CHANNEL_DRIVEN_BY_CONFIG = 0,
		REPORT_CHANNEL_ETW,
		REPORT_CHANNEL_EVENTLOG,
		REPORT_CHANNEL_LOGFILE,
		REPORT_CHANNEL_DEBUG_EVENTS,
		REPORT_CHANNEL_META_ALL,
		REPORT_CHANNEL_NA
	};

	//Report Data Object
	//https://stackoverflow.com/questions/51554694/should-shared-ptr-constructor-arguments-be-passed-by-value
	struct ReportData
	{
		ReportChannelID ChannelID;
		std::wstring Name;
		EventObject Event;

		ReportData(const ReportChannelID &channelID, EventObject &eventData) :
			ChannelID(channelID), Event(std::move(eventData)) {}

		ReportData(EventObject &eventData) :
			ChannelID(ReportChannelID::REPORT_CHANNEL_DRIVEN_BY_CONFIG), Event(std::move(eventData)) {}
	};
	typedef std::shared_ptr<ReportData> ReportObject;


	//Rule Data Object
	struct RuleData
	{
		std::wstring Description;
	};
	typedef std::shared_ptr<RuleData> RuleObject;


	//Containers
	typedef std::vector<RuleObject> RulesContainer;
	typedef std::vector<ReportChannelID> ReportOutputList;

	//Callbacks
	typedef boost::function<bool(EventObject &data)> CallbackFN;
}


namespace SysmonXEvents
{
	//Event creation factories
	static inline SysmonXTypes::EventObject GetNewSecurityEvent()
	{
		return std::make_shared<SysmonXTypes::EventData>();
	}

	static inline SysmonXTypes::EventObject GetNewSecurityEvent(const SysmonXTypes::EventID &eventID)
	{
		return std::make_shared<SysmonXTypes::EventData>(eventID);
	}

	//Report creation factories
	static inline SysmonXTypes::ReportObject GetNewReportEvent(SysmonXTypes::EventObject &eventData)
	{
		return std::make_shared<SysmonXTypes::ReportData>(eventData);
	}

	static inline SysmonXTypes::ReportObject GetNewReportEvent(const SysmonXTypes::ReportChannelID &channelID, SysmonXTypes::EventObject &eventData)
	{
		return std::make_shared<SysmonXTypes::ReportData>(channelID, eventData);
	}

}