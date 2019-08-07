#pragma once

#include "common.h"

namespace SysmonXDefs
{
	static const int SYSMONX_SERIALIZED_CONFIG_VERSION = 1;
	static const unsigned int SYSMONX_MIN_NUMBER_OF_ARGS = 2;
	static const unsigned int SYSMONX_DEFAULT_WORKER_THREADS = 8;
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
}

