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
	bool GetFullPathConfigFile(std::wstring &configFile);
	bool GetFullPathBackendConfigFile(std::wstring &configFile);
	bool GetFullPathCollectionServiceFile(std::wstring &collectionServiceFile);
	bool IsNewCollectionServiceInstance();
	bool IsNewBackend32Instance();
	bool IsNewBackend64Instance();
	bool IsNewWorkingDirectory();

	//Boolean Configuration Helpers
	bool IsInitialized() { return m_isInitialized; }
	bool IsServiceMode() { return m_isServiceMode; }
	bool IsServiceDataAvailable() { return m_isServiceDataAvailable; }
	bool WasInstallRequested() { return m_wasInstallRequested; }
	bool WasInstallAcceptEulaRequested() { return m_wasInstallAcceptEulaRequested; }
	bool WasUninstallRequested() { return m_wasUninstallRequested; }
	bool WasForceUninstallRequested() { return m_wasUninstallForceRequested; }
	bool WasSignatureRevocationRequested() { return m_wasSignatureRevocationCheckRequested; }
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
	bool IsTraceBackendDriverNameAvailable() { return !m_traceBackendName.empty(); }
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
	const HashAlgorithm& GetHashAlgorithmToUse() { return m_hashAlgorithmToUse; }
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
	const std::wstring& GetConfigurationFile() { return m_configurationFile; }
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
	bool ValidateConfigurationFile(std::wstring &configFilename);
	bool IsValidHashAlgorithm(const unsigned int &hashAlgorithm);
	bool GenerateTraceBackendNames(const std::wstring &traceBackendName, std::wstring &traceBackend32BitsName, std::wstring &traceBackend64BitsName);
	bool GetParsedProxyConfData(const std::wstring &proxyStr, ProxyConfData &proxyData);
	bool GenerateDefaultConfigFile(std::wstring &newConfigFile);
	const BackendInstallVector GetBackendLocation(const std::wstring &backend);
	const LoggerContainer GetLoggerMode(const std::wstring &loggerMode);
	const LoggerVerbose GetVerbosityMode(const std::wstring &verbosityMode);
	const HashAlgorithm GetHashAlgorithm(const std::wstring &selectedHash);
	const ReportOutputList GetReportList(const std::wstring &reportList);
	const ProcessesList GetProcessList(const std::wstring &processesList);
	const ReportChannelID GetReportChannelID(const std::wstring &reportChannel);

	//Lifecycle management
	ConfigManager() : m_isInitialized(false), m_isServiceMode(false), m_isServiceDataAvailable(false), 
		m_wasInstallRequested(false), m_wasInstallAcceptEulaRequested(false), m_wasUninstallRequested(false), 
		m_wasUninstallForceRequested(false), m_wasSignatureRevocationCheckRequested(false), m_wasDumpOfSchemaDefinitionsRequested(false),
		m_wasInstallEventsManifestRequested(false), m_wasPrintCurrentConfigurationRequested(false), m_wasHelpRequested(false), 
		m_configFileSyntaxOK(false), m_wasNumberOfWorkersRequested(false), m_wasCollectionLoggingRequested(false), 
		m_wasVerbosityRequested(false), m_wasReportOptionsRequested(false), m_wasNewConfigFileRequested (false),
		m_hashAlgorithmToUse(HashAlgorithm::HASH_ALGORITHM_SHA1), m_backendInstallerVector(BackendInstallVector::BACKEND_INSTALLER_WEB),
		m_loggingMgmtApp(LoggerMode::LOGGER_CONSOLE), m_loggingCollectionService(LoggerMode::LOGGER_NA),
		m_loggingVerbosity(LoggerVerbose::LOGGER_TRACE), m_nrWorkerOrchThreads(SysmonXDefs::SYSMONX_DEFAULT_WORKER_THREADS)
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
	bool m_isInitialized;
	bool m_isServiceMode;
	bool m_isServiceDataAvailable;
	bool m_wasInstallRequested;
	bool m_wasInstallAcceptEulaRequested;
	bool m_wasUninstallRequested;
	bool m_wasUninstallForceRequested;
	bool m_wasSignatureRevocationCheckRequested;
	bool m_wasDumpOfSchemaDefinitionsRequested;
	bool m_wasInstallEventsManifestRequested;
	bool m_wasPrintCurrentConfigurationRequested;
	bool m_wasHelpRequested;
	bool m_configFileSyntaxOK;
	bool m_wasNumberOfWorkersRequested;
	bool m_wasCollectionLoggingRequested;
	bool m_wasVerbosityRequested;
	bool m_wasReportOptionsRequested;
	bool m_wasNewConfigFileRequested;
	HashAlgorithm m_hashAlgorithmToUse;
	ProcessesList m_processesToTrackOnLoadingModules;
	ProcessesList m_processesToTrackOnNetworkConnections;
	BackendInstallVector m_backendInstallerVector;
	LoggerContainer m_loggingMgmtApp;
	LoggerContainer m_loggingCollectionService;
	ReportOutputList m_reportOutputList;
	LoggerVerbose m_loggingVerbosity;
	UINT32 m_nrWorkerOrchThreads;
	ProxyConfData m_proxyConfData;
	std::wstring m_configurationFile;
	std::wstring m_traceBackendName;
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