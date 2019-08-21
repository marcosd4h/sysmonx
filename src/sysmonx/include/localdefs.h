#pragma once

#include "common.h"

namespace SysmonXDefs
{
	static const int SYSMONX_SERIALIZED_CONFIG_VERSION = 1;
	static const unsigned int SYSMONX_MIN_NUMBER_OF_ARGS = 2;
	static const unsigned int SYSMONX_DEFAULT_WORKER_THREADS = 8;
	static const unsigned int SYSMONX_DEFAULT_SERVICE_OP_TIMEOUT_IN_MS = 500;
	static const std::wstring SYSMONX_APPNAME = L"SysmonX";
	static const std::wstring SYSMONX_KERNEL_TRACE_NAME = L"SysmonXKernelTraceSession";
	static const std::wstring SYSMONX_USERSPACE_TRACE_NAME = L"SysmonXUserTraceSession";
	static const std::string SYSMONX_VERSION = "v0.1";
	static const bool DEFAULT_DISSOLVABLE_MODE = false;
	static const bool DEFAULT_FP_MITIGATIONS_MODE = true;
	static const std::wstring DEFAULT_REPORT_MODE = L"regular";
	static const std::wstring DEFAULT_REPORT_OUTPUT = L"console";
	static const std::wstring SERVICE_NAME = L"sysmonxcollect";
	static const std::wstring SERVICE_DISPLAY = L"sysmonxcollect";
	static const std::wstring DEFAULT_LOG_FILE = L"default_sysmonx_logfile.log";
	static const std::wstring DEFAULT_CONFIG_FILE = L"default_sysmonx_config.xml";
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

	//Sysmon Exit Codes
	static const DWORD SYSMON_EXIT_CODE_ALREADY_INSTALLED = 1242;
	static const DWORD SYSMON_EXIT_CODE_ALREADY_RUNNING = 259;
	static const DWORD SYSMON_EXIT_CODE_ALREADY_UNINSTALLED = 87;
	static const DWORD SYSMON_EXIT_CODE_OK = EXIT_SUCCESS;


	//Default Content
	static const std::wstring DEFAULT_SYSMON_CONFIG_FILE_CONTENT =
		L"<Sysmon schemaversion=\"4.1\">\n"
		L"   <!-- Capture all hashes -->\n"
		L"   <HashAlgorithms>*</HashAlgorithms>\n"
		L"   <CheckRevocation/>\n"
		L"   <EventFiltering>\n"
		L"      <!-- Event ID 1 == Process Creation. -->\n"
		L"      <ProcessCreate onmatch=\"include\"/>\n"
		L"      <!-- Event ID 2 == File Creation Time. -->\n"
		L"      <FileCreateTime onmatch=\"include\"/>\n"
		L"      <!-- Event ID 3 == Network Connection. -->\n"
		L"      <NetworkConnect onmatch=\"include\"/>\n"
		L"      <!-- Event ID 5 == Process Terminated. -->\n"
		L"      <ProcessTerminate onmatch=\"include\"/>\n"
		L"      <!-- Event ID 6 == Driver Loaded. -->\n"
		L"      <DriverLoad onmatch=\"include\"/>\n"
		L"      <!-- Event ID 7 == Image Loaded. -->\n"
		L"      <ImageLoad onmatch=\"include\"/>\n"
		L"      <!-- Event ID 8 == CreateRemoteThread. -->\n"
		L"      <CreateRemoteThread onmatch=\"include\"/>\n"
		L"      <!-- Event ID 9 == RawAccessRead. -->\n"
		L"      <RawAccessRead onmatch=\"include\"/>\n"
		L"      <!-- Event ID 10 == ProcessAccess. -->\n"
		L"      <ProcessAccess onmatch=\"include\"/>\n"
		L"      <!-- Event ID 11 == FileCreate. -->\n"
		L"      <FileCreate onmatch=\"include\"/>\n"
		L"      <!-- Event ID 12,13,14 == RegObject added/deleted, RegValue Set, RegObject Renamed. -->\n"
		L"      <RegistryEvent onmatch=\"include\"/>\n"
		L"      <!-- Event ID 15 == FileStream Created. -->\n"
		L"      <FileCreateStreamHash onmatch=\"include\"/>\n"
		L"      <!-- Event ID 17,18 == PipeEvent. Log Named pipe created & Named pipe connected -->\n"
		L"      <PipeEvent onmatch=\"include\"/>\n"
		L"      <!-- Event ID 19,20,21, == WmiEvent. Log all WmiEventFilter, WmiEventConsumer, WmiEventConsumerToFilter activity-->\n"
		L"      <WmiEvent onmatch=\"include\"/>\n"
		L"  </EventFiltering>\n"
		L"</Sysmon>";
}

