#pragma once

#include "common.h"

using namespace CommonTypes;
using namespace SysmonXTypes;

class ConfigManager
{
public:
	static ConfigManager& Instance()
	{
		static ConfigManager instance;
		return instance;
	}

	//Control functions
	bool Initialize(int argc, wchar_t *argv[]);
	bool GetRegistryConfigData(CommonTypes::ByteContainer& rawBytes, SysmonXTypes::ConfigSerializedData* data);
	bool GetBackendFiles(std::wstring &backend32BitsFile, std::wstring &backend64BitsFile);	
	bool GetFullPathBackendConfigFile(std::wstring &configFile);
	bool GetFullPathCollectionServiceFile(std::wstring &collectionServiceFile);
	bool IsNewCollectionServiceInstance();
	bool IsNewBackend32Instance();
	bool IsNewBackend64Instance();
	bool IsNewWorkingDirectory();
	bool GenerateBackendConfigFile(std::wstring &configFile);

	//Boolean Configuration Helpers
	bool IsInitialized() { return m_isInitialized; }
	bool IsServiceMode() { return m_isServiceMode; }
	bool IsDebugMode() { return m_isDebugMode; }	
	bool IsServiceDataAvailable() { return m_isServiceDataAvailable; }
	bool WasInstallRequested() { return m_wasInstallRequested; }
	bool WasInstallAcceptEulaRequested() { return m_wasInstallAcceptEulaRequested; }
	bool WasUninstallRequested() { return m_wasUninstallRequested; }
	bool WasForceUninstallRequested() { return m_wasUninstallForceRequested; }
	bool WasSignatureRevocationRequested() { return m_wasSignatureRevocationCheckRequested; }
	bool WasLogOfLoadingModulesRequested() { return m_wasLogOfLoadingModulesRequested; }
	bool WasLogOfNetworkConnectionsRequested() { return m_wasLogOfNetworkConnectionsRequested; }
	bool WasDumpOfSchemaRequested() { return m_wasDumpOfSchemaDefinitionsRequested; }
	bool WasInstallManifestRequested() { return m_wasInstallEventsManifestRequested; }
	bool WasConfigurationDumpRequested() { return m_wasPrintCurrentConfigurationRequested; }
	bool WasHelpRequested() { return m_wasHelpRequested; }
	bool WasNumberOfWorkerThreadsRequested() { return m_wasNumberOfWorkersRequested; }
	bool WasCollectionLoggingRequested() { return m_wasCollectionLoggingRequested; }
	bool WasVerbosityRequested() { return m_wasVerbosityRequested; }
	bool WasReportOptionsRequested() { return m_wasReportOptionsRequested; }
	bool WasNewConfigFileProvided() { return m_wasNewConfigFileRequested; }
	bool WereStandaloneActionsRequested() { return (m_wasNewConfigFileRequested || m_wasPrintCurrentConfigurationRequested || m_wasDumpOfSchemaDefinitionsRequested || m_wasInstallEventsManifestRequested); }
	bool IsConfigurationFileAvailable() { return !m_configurationFile.empty(); }
	bool IsConfigurationFileContentAvailable() { return !m_configurationFileContent.empty(); }
	bool IsTraceBackendDriverNameAvailable() { return !m_traceBackendDriverName.empty(); }
	bool IsBackend32ServiceNameAvailable() { return !m_backend32ServiceName.empty(); }
	bool IsBackend64ServiceNameAvailable() { return !m_backend64ServiceName.empty(); }
	bool IsCollectionServiceNameAvailable() { return !m_collectionServiceName.empty(); }
	bool IsInstallLocationAvailable() { return !m_installLocation.empty(); }
	bool IsProxyDataAvailable() { return !m_proxyRawData.empty(); }
	bool IsConsoleLogfileAvailable() { return !m_consoleLogfile.empty(); }
	bool IsCollectionServiceLogfileAvailable() { return !m_collectionServiceLogfile.empty(); }
	bool IsReportLogfileAvailable() { return !m_reportLogfile.empty(); }
	bool IsSystemDirectoryAvaialble() { return !m_systemDirectory.empty(); }
	bool IsSystemRootAvaialble() { return !m_systemRootDirectory.empty(); }
	bool IsWorkingDirectoryAvailable() { return !m_workingDirectory.empty(); }

	//Data Configuration Helpers
	const ProcessesList& GetProcessesToTrackLoadingModulesList() { return m_processesToTrackOnLoadingModules; }
	const ProcessesList& GetProcessesToTrackNetworkConnections() { return m_processesToTrackOnNetworkConnections; }
	const BackendInstallVector& GetBackendInstallerVector() { return m_backendInstallerVector; }
	const LoggerContainer& GetLoggerOutputModuleMGMTApp() { return m_loggingMgmtApp; }
	const LoggerContainer& GetLoggerOutputCollectionService() { return m_loggingCollectionService; }
	const ReportOutputList& GetReportOutputList() { return m_reportOutputList; }
	const LoggerVerbose& GetLoggingVerbosity() { return m_loggingVerbosity; }
	const UINT32& GetWorkerThreads() { return m_nrWorkerOrchThreads; }
	const ProxyConfData& GetProxyConfData() { return m_proxyConfData; }

	//String Configuration Helpers
	const std::wstring& GetHashAlgorithmToUse() { return m_hashAlgorithmToUse; }
	const std::wstring& GetConfigurationFileContent() { return m_configurationFileContent; }
	const std::wstring& GetBackend32ServiceName() { return m_backend32ServiceName; }
	const std::wstring& GetBackend64ServiceName() { return m_backend64ServiceName; }
	const std::wstring& GetCollectionServiceName() { return m_collectionServiceName; }
	const std::wstring& GetPreviousBackend32ServiceName() { return m_previousBackend32ServiceName; }
	const std::wstring& GetPreviousBackend64ServiceName() { return m_previousBackend64ServiceName; }
	const std::wstring& GetPreviousCollectionServiceName() { return m_previousCollectionServiceName; }
	const std::wstring& GetInstallLocation() { return m_installLocation; }
	const std::wstring& GetProxyData() { return m_proxyRawData; }
	const std::wstring& GetConsoleLogFile() { return m_consoleLogfile; }
	const std::wstring& GetCollectionServiceLogFile() { return m_collectionServiceLogfile; }
	const std::wstring& GetReportLogFile() { return m_reportLogfile; }
	const std::wstring& GetSysmonXSystemDirectory() { return m_systemDirectory; }
	const std::wstring& GetSysmonXSystemRootDirectory() { return m_systemRootDirectory; }
	const std::wstring& GetSysmonXWorkingDirectory() { return m_workingDirectory; }

private:
	//Private Helpers
	bool SyncRuntimeConfigData();
	bool IsSystemDataAvailable();
	bool ValidateConfigurationFile(const std::wstring &configFilename);
	bool IsValidHashAlgorithm(const std::wstring &selectedHashAlgorithms);
	bool IsValidHashAlgorithmRange(const unsigned int &hashAlgorithm);
	bool GenerateTraceBackendNames(const std::wstring &traceBackendName, std::wstring &traceBackend32BitsName, std::wstring &traceBackend64BitsName);
	bool GetParsedProxyConfData(const std::wstring &proxyStr, ProxyConfData &proxyData);
	bool ParseConfigurationFile(const std::wstring &configFile);
	bool GenerateCollectionServiceConfigFileContent(std::wstring& configFile);
	const BackendInstallVector GetBackendLocation(const std::wstring &backend);
	const LoggerContainer GetLoggerMode(const std::wstring &loggerMode);
	const LoggerVerbose GetVerbosityMode(const std::wstring &verbosityMode);
	const HashAlgorithmsContainer GetHashAlgorithmsVector(const std::wstring &selectedHashAlgorithms);	
	const ReportOutputList GetReportList(const std::wstring &reportList);
	const ProcessesList GetProcessList(const std::wstring &processesList);
	const ReportChannelID GetReportChannelID(const std::wstring &reportChannel);
	std::wstring GetHashAlgorithmsConfiguration();
	std::wstring GetRevocationConfiguration();

	//Lifecycle management
	ConfigManager() : m_loggingMgmtApp(LoggerMode::LOGGER_CONSOLE), m_loggingCollectionService(LoggerMode::LOGGER_NA)
	{
		m_processesToTrackOnLoadingModules.clear();
		m_processesToTrackOnNetworkConnections.clear();
		m_reportOutputList.clear();
	};

	~ConfigManager() {}

	//Forcing singleton here
	ConfigManager(const ConfigManager&) = delete;
	ConfigManager(ConfigManager&&) = delete;
	ConfigManager& operator=(const ConfigManager&) = delete;
	ConfigManager& operator=(ConfigManager&&) = delete;

	//Private variables
	bool m_isInitialized = false;
	bool m_isServiceMode = false;
	bool m_isDebugMode = false;
	bool m_isServiceDataAvailable = false;
	bool m_wasInstallRequested = false;
	bool m_wasInstallAcceptEulaRequested = false;
	bool m_wasUninstallRequested = false;
	bool m_wasUninstallForceRequested = false;
	bool m_wasSignatureRevocationCheckRequested = false;
	bool m_wasLogOfLoadingModulesRequested = false;
	bool m_wasLogOfNetworkConnectionsRequested = false;
	bool m_wasDumpOfSchemaDefinitionsRequested = false;
	bool m_wasInstallEventsManifestRequested = false;
	bool m_wasPrintCurrentConfigurationRequested = false;
	bool m_wasHelpRequested = false;
	bool m_configFileSyntaxOK = false;
	bool m_wasNumberOfWorkersRequested = false;
	bool m_wasCollectionLoggingRequested = false;
	bool m_wasVerbosityRequested = false;
	bool m_wasReportOptionsRequested = false;
	bool m_wasNewConfigFileRequested = false;
	std::wstring m_hashAlgorithmToUse = SysmonXDefs::SYSMON_DEFAULT_HASH_ALGORITHM;
	ProcessesList m_processesToTrackOnLoadingModules;
	ProcessesList m_processesToTrackOnNetworkConnections;
	BackendInstallVector m_backendInstallerVector = BackendInstallVector::BACKEND_INSTALLER_WEB;
	LoggerContainer m_loggingMgmtApp ;
	LoggerContainer m_loggingCollectionService;
	ReportOutputList m_reportOutputList;
	LoggerVerbose m_loggingVerbosity = LoggerVerbose::LOGGER_TRACE;
	UINT32 m_nrWorkerOrchThreads = SysmonXDefs::SYSMONX_DEFAULT_WORKER_THREADS;
	ProxyConfData m_proxyConfData;
	std::wstring m_configurationFile;
	std::wstring m_configurationFileContent;
	std::wstring m_traceBackendDriverName;
	std::wstring m_backend32ServiceName;
	std::wstring m_backend64ServiceName;
	std::wstring m_previousBackend32ServiceName;
	std::wstring m_previousBackend64ServiceName;
	std::wstring m_collectionServiceName;
	std::wstring m_previousCollectionServiceName;
	std::wstring m_installLocation;
	std::wstring m_proxyRawData;
	std::wstring m_consoleLogfile;
	std::wstring m_collectionServiceLogfile;
	std::wstring m_reportLogfile;
	std::wstring m_systemDirectory;
	std::wstring m_systemRootDirectory;
	std::wstring m_workingDirectory;
	std::wstring m_previousWorkingDirectory;
};