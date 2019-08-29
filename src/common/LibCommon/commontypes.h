#pragma once

#include <external.h>

namespace CommonTypes
{
	enum LoggerMode
	{
		LOGGER_FILE = 0x00,
		LOGGER_CONSOLE,
		LOGGER_NA
	};

	enum LoggerVerbose
	{
		LOGGER_TRACE = 0x00,
		LOGGER_DEBUG,
		LOGGER_INFO,
		LOGGER_WARN,
		LOGGER_ERR,
		LOGGER_CRITICAL,
		LOGGER_OFF
	};

	enum HashAlgorithm
	{
		HASH_ALGORITHM_SHA1 = 0x00,
		HASH_ALGORITHM_SHA256,
		HASH_ALGORITHM_MD5,
		HASH_ALGORITHM_IMPHASH,
		HASH_ALGORITHM_ALL,
		HASH_ALGORITHM_NA
	};

	enum BackendInstallVector
	{
		BACKEND_INSTALLER_WEB = 0x00,
		BACKEND_INSTALLER_FILES,
		BACKEND_INSTALLER_NA
	};


	typedef std::vector<std::string> StringsContainer;
	typedef std::vector<std::wstring> WStringsContainer;
	typedef std::vector<char> CharContainer;
	typedef std::vector<wchar_t> WCharContainer;
	typedef std::vector<unsigned int> UIntContainer;
	typedef std::vector<BYTE> ByteContainer;
	typedef std::vector<LoggerMode> LoggerContainer;
	typedef std::vector<unsigned int> HashAlgorithmsContainer;
	typedef StringsContainer ProcessesList;
	typedef cista::offset::vector<char> SerializedCharVector;
	typedef cista::offset::vector<wchar_t> SerializedWCharVector;
	typedef cista::offset::vector<unsigned int> SerializedUIntVector;
	typedef cista::offset::vector<cista::offset::string> SerializedCistaStringVector;
	typedef std::vector<bool>	ArrayBoolT;
	typedef std::vector<UINT16> Array16T;
	typedef std::vector<UINT32> Array32T;
	typedef std::vector<UINT64> Array64T;

	struct ProxyConfData
	{
		std::wstring proxy;
		std::wstring port;
		std::wstring username;
		std::wstring password;
	};

	struct ServiceProperties
	{
		ServiceProperties() : canBeStopped(true), canBeShuttedDown(true), canBePausedOrResumed(true) {}

		bool canBeStopped;
		bool canBeShuttedDown;
		bool canBePausedOrResumed;
	};

	enum ProcessType
	{
		PROCESS_32_BITS = 0x00,
		PROCESS_WOW_32_BITS,
		PROCESS_64_BITS,
		PROCESS_UNKNOWN
	};

	enum TraceBackendType
	{
		TRACE_BACKEND_CUSTOM = 0,
		TRACE_BACKEND_SYSMON,
		TRACE_BACKEND_NA
	};
}

namespace CustomWinTypes
{
	typedef struct _CLIENT_ID {
		HANDLE UniqueProcess;
		HANDLE UniqueThread;
	} CLIENT_ID, *PCLIENT_ID;

	typedef LONG NTSTATUS;
	typedef LONG KPRIORITY;
#define STATUS_SUCCESS    ((NTSTATUS)0x00000000L)

	typedef struct _THREAD_BASIC_INFORMATION {
		NTSTATUS  ExitStatus;
		PVOID     TebBaseAddress;
		CLIENT_ID ClientId;
		KAFFINITY AffinityMask;
		KPRIORITY Priority;
		KPRIORITY BasePriority;
	} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

	const static THREADINFOCLASS ThreadBasicInformation = (THREADINFOCLASS)0;
	const static THREADINFOCLASS ThreadQuerySetWin32StartAddress = (THREADINFOCLASS)9;

	typedef NTSTATUS(WINAPI *PNtQueryInformationThread) (
		HANDLE thread,
		THREADINFOCLASS infoclass,
		PVOID buffer,
		ULONG buffersize,
		PULONG used);

	typedef NTSTATUS(NTAPI *pNtQueryInformationProcess)(
		HANDLE ProcessHandle,
		DWORD ProcessInformationClass,
		PVOID ProcessInformation,
		DWORD ProcessInformationLength,
		PDWORD ReturnLength
		);

	typedef BOOL(WINAPI *PFN_ISWOW64PROCESS)(HANDLE hProcess, PBOOL Wow64Process);
}


namespace ReportAttributes
{
	//Common
	static const std::wstring REPORT_SYSMONX_EVENT_NAME = L"SysmonX Module";
	static const std::wstring REPORT_PROCESS_ID = L"Process ID";
	static const std::wstring REPORT_PROCESS_NAME = L"Process Name";
	static const std::wstring REPORT_PROCESS_EXECUTABLE_PATH = L"Process Path";
	static const std::wstring REPORT_PROCESS_CMDLINE = L"Process Commmand Line";
	static const std::wstring REPORT_PROCESS_BASE_PRIORITY = L"Process Base Priority";
	static const std::wstring REPORT_PROCESS_NR_THREADS = L"Process Nr Threads";
	static const std::wstring REPORT_TOKEN_INFO = L"Suspicious Thread Token Information";

	static const std::wstring REPORT_EXTENDED_THREAD_INFO = L"Suspicious Extended Thread Information";
	static const std::wstring REPORT_EXTENDED_MODULES_INFO = L"Suspicious Extended Module Information";
}

namespace SysmonXCommon
{
	typedef std::vector<THREADENTRY32> ThreadsCollection;
	typedef std::vector<MODULEENTRY32> ModulesCollection;

	struct ProcessData
	{
		ProcessData() : handle(INVALID_HANDLE_VALUE), pid(0) {}

		ProcessData(HANDLE _handle, DWORD _pid, CommonTypes::ProcessType _type = CommonTypes::ProcessType::PROCESS_UNKNOWN)
			: handle(_handle), pid(_pid), processType(_type), isManaged(false), bbProcessReady(false), bbPEReady(false), bbMainModule(nullptr) {}
		ProcessData(HANDLE _handle, DWORD _pid, std::wstring &_name, CommonTypes::ProcessType _type = CommonTypes::ProcessType::PROCESS_UNKNOWN)
			: handle(_handle), pid(_pid), processType(_type), isManaged(false), bbProcessReady(false), bbPEReady(false), bbMainModule(nullptr), name(_name) {}

		HANDLE handle;
		DWORD pid;
		CommonTypes::ProcessType processType;
		bool isManaged;
		bool bbProcessReady;
		bool bbPEReady;
		blackbone::ModuleDataPtr bbMainModule;
		std::wstring name;
		blackbone::Process bbProcess;
		blackbone::pe::PEImage bbPE;
		std::vector<blackbone::HandleInfo> bbHandles;
		blackbone::ProcessModules::mapModules bbModulesManuallyMapped; //Get List of manually mapped modules
		blackbone::ProcessModules::mapModules bbModulesLdrList;   //modules found thru walking list at peb.ldr.InLoadOrderModuleList
		blackbone::ProcessModules::mapModules bbModulesPEHeaders; //modules found thru walking memory looking for valid PE headers
		blackbone::ProcessModules::mapModules bbModulesSections;  //modules found thru walking memory sections and looking for PE headers
																  //(wiped headers will be detected)

		ThreadsCollection threads;
		ModulesCollection modules;
	};

	typedef std::shared_ptr<ProcessData> ProcessDataPtr;
	typedef std::map<DWORD, ProcessDataPtr> ProcessCollection;
	typedef std::map<DWORD, std::vector<std::wstring>> ThreadInfoCollection;
	typedef std::map<std::wstring, std::wstring> ProcessInfoContainer;
	typedef std::vector<std::wstring> ModulesInfoContainer;
	//	typedef std::unordered_map<std::wstring, std::wstring> ReportPropertiesType;
	typedef std::vector<std::pair<std::wstring, std::wstring>> ReportPropertiesType;
	
	struct RangeData
	{
		RangeData() : minValue(0), maxValue(0) {}

		RangeData(size_t _minValue, size_t _maxValue)
			: minValue(_minValue), maxValue(_maxValue) {}

		size_t minValue;
		size_t maxValue;
	};

	struct CallStackData
	{
		CallStackData() : stackFrameAddress(0), baseOfImageAddress(0), symbolAddress(0), imageSize(0), symbolAvailable(0),
			timeDateStamp(0), checksum(0), numSyms(0), symType(SymNone) {}

		DWORD64 stackFrameAddress;
		DWORD64 baseOfImageAddress;
		DWORD64 symbolAddress;
		DWORD64 symbolAvailable;
		DWORD imageSize;
		DWORD timeDateStamp;
		DWORD checksum;
		DWORD numSyms;
		SYM_TYPE symType;
		std::string moduleName;
		std::string imageName;
		std::string loadedImageName;
		std::string symbolName;
	};

	typedef std::shared_ptr<CallStackData> CallStackDataPtr;
	typedef std::vector<CallStackDataPtr> CallStackDataList;

	struct PatternData
	{
		blackbone::PatternSearch pattern;
		uint8_t wildcard;
		size_t patternSize;
	};
	typedef std::shared_ptr<PatternData> PatternDataPtr;

	struct ETWRemoteProcessData
	{
		ETWRemoteProcessData() :
			UniqueProcessKey(0),
			ProcessId(0),
			ParentId(0),
			SessionId(0),
			ExitStatus(0),
			ImageFileName(""),
			CommandLine(""),
			ProcessEnded(0),
			StartTime(0),
			EndTime(0) {}

		size_t UniqueProcessKey;
		uint32_t ProcessId;
		uint32_t ParentId;
		uint32_t SessionId;
		uint32_t ExitStatus;
		std::string ImageFileName;
		std::string CommandLine;
		uint32_t ProcessEnded;
		LONGLONG StartTime;
		LONGLONG EndTime;
	};

	struct ETWRemoteThreadData
	{
		ETWRemoteThreadData() :
			ProcessId(0),
			TThreadId(0),
			CallerProcessId(0),
			CallerTThreadId(0),
			BasePriority(0),
			PagePriority(0),
			IoPriority(0),
			ThreadFlags(0),
			Win32StartAddr(0),
			TebBase(0),
			ThreadEnded(0),
			StartTime(0),
			EndTime(0) {}

		uint32_t ProcessId;
		uint32_t TThreadId;
		uint32_t CallerProcessId;
		uint32_t CallerTThreadId;
		uint32_t BasePriority;
		uint32_t PagePriority;
		uint32_t IoPriority;
		uint32_t ThreadFlags;
		size_t Win32StartAddr;
		size_t TebBase;
		uint32_t ThreadEnded;
		LONGLONG StartTime;
		LONGLONG EndTime;
	};

	struct ETWRemoteAllocData
	{
		ETWRemoteAllocData() :
			ProcessId(0),
			CallerProcessId(0),
			CallerTThreadId(0),
			BaseAddress(0),
			Flags(0),
			AllocationTimestamp(0) {}

		uint32_t ProcessId;
		uint32_t CallerProcessId;
		uint32_t CallerTThreadId;
		size_t BaseAddress;
		uint32_t Flags;
		LONGLONG AllocationTimestamp;
	};
}

