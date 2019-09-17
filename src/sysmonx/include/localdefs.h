#pragma once

#include "common.h"

namespace SysmonXDefs
{
	constexpr int SYSMONX_SERIALIZED_CONFIG_VERSION = 1;
	constexpr unsigned int SYSMONX_MIN_NUMBER_OF_ARGS = 2;
	constexpr unsigned int SYSMONX_DEFAULT_WORKER_THREADS = 8;
	constexpr unsigned int SYSMONX_DEFAULT_SERVICE_OP_TIMEOUT_IN_MS = 500;
	constexpr bool DEFAULT_DISSOLVABLE_MODE = false;
	constexpr bool DEFAULT_FP_MITIGATIONS_MODE = true;
	static const std::string SYSMONX_VERSION = "v0.3.1";
	static const std::wstring SYSMONX_APPNAME = L"SysmonX";
	static const std::wstring SYSMONX_KERNEL_TRACE_NAME = L"SysmonXKernelTraceSession";
	static const std::wstring SYSMONX_USERSPACE_TRACE_NAME = L"SysmonXUserTraceSession";
	static const std::wstring DEFAULT_REPORT_MODE = L"regular";
	static const std::wstring DEFAULT_REPORT_OUTPUT = L"console";
	static const std::wstring SERVICE_NAME = L"sysmonxcollect";
	static const std::wstring SERVICE_DISPLAY = L"sysmonxcollect";
	static const std::wstring DEFAULT_LOG_FILE = L"default_sysmonx_logfile.log";
	static const std::wstring DEFAULT_CONFIG_FILE = L"default_sysmonx_config.xml";
	static const std::wstring DEFAULT_BACKEND_CONFIG_FILE_NAME = L"trace_backend_config.xml";
	static const std::wstring BACKEND_NAME = L"Sysmon";
	static const std::wstring DEFAULT_DB_NAME = L"huntingdb.db";
	static const std::wstring SERVICE_MODE_ARGS = L"-service";
	static const std::wstring SYSMONX_REGISTRY_KEY_LOCATION = L"SOFTWARE\\SysmonX";
	static const std::wstring SYSMONX_REGISTRY_VALUE_LOCATION = L"ConfigData";
	static const std::wstring SYSMON_REGISTRY_EULA_KEY_LOCATION = L"Software\\Sysinternals\\System Monitor";
	static const std::wstring SYSMON_REGISTRY_EULA_VALUE_LOCATION = L"EulaAccepted";
	static const std::string  DEFAULT_SYSMONX_ENTROPY = "SysmonX";
	static const std::wstring SYSMON_TRACE_BACKEND_32BITS = L"Sysmon.exe";
	static const std::wstring SYSMON_TRACE_BACKEND_64BITS = L"Sysmon64.exe";
	static const std::wstring SYSMON_DEFAULT_HOST = L"download.sysinternals.com";
	static const std::wstring SYSMON_DEFAULT_URI = L"/files/Sysmon.zip";
	static const std::wstring SYSMON_DEFAULT_HASH_ALGORITHM = L"sha256";

	//Sysmon Exit Codes
	constexpr DWORD SYSMON_EXIT_CODE_ALREADY_INSTALLED = 1242;
	constexpr DWORD SYSMON_EXIT_CODE_ALREADY_RUNNING = 259;
	constexpr DWORD SYSMON_EXIT_CODE_ALREADY_UNINSTALLED = 87;
	constexpr DWORD SYSMON_EXIT_CODE_OK = EXIT_SUCCESS;

	//Default Config File Header
	static const std::wstring DEFAULT_SYSMON_CONFIG_FILE_HEADER =
		L"<Sysmon schemaversion=\"4.21\">\n";

	//Default Config File Tail
	static const std::wstring DEFAULT_SYSMON_CONFIG_FILE_TAIL =
		L"</Sysmon>\n";

	//Default Trace Backend Config Filtering
	static const std::wstring DEFAULT_SYSMON_TRACE_BACKEND_CONFIG_FILE_FILTERING =
		L"	<EventFiltering>\n"
		L"		<!-- Event ID 1 == Process Creation. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<ProcessCreate onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 2 == File Creation Time. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<FileCreateTime onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 3 == Network Connection. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<NetworkConnect onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 5 == Process Terminated. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<ProcessTerminate onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 6 == Driver Loaded. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<DriverLoad onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 7 == Image Loaded. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<ImageLoad onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 8 == CreateRemoteThread. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<CreateRemoteThread onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 9 == RawAccessRead. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<RawAccessRead onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 10 == ProcessAccess. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<ProcessAccess onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 11 == FileCreate. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<FileCreate onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 12,13,14 == RegObject added/deleted, RegValue Set, RegObject Renamed. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<RegistryEvent onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 15 == FileStream Created. -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<FileCreateStreamHash onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 17,18 == PipeEvent. Log Named pipe created & Named pipe connected -->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<PipeEvent onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"		<!-- Event ID 19,20,21, == WmiEvent. Log all WmiEventFilter, WmiEventConsumer, WmiEventConsumerToFilter activity-->\n"
		L"		<RuleGroup name=\"\" groupRelation=\"or\">\n"
		L"			<WmiEvent onmatch=\"exclude\"/>\n"
		L"		</RuleGroup>\n"
		L"	</EventFiltering>\n";

	//Default SysmonX Config Filtering
	static const std::wstring DEFAULT_SYSMONX_CONFIG_FILE_FILTERING =
		L"  <EventFiltering>\n"
		L"  </EventFiltering>\n";
}

