#pragma once

#include "common.h"

namespace SysmonXAppFlows
{
	//Flows Entry Points
	bool RunCollectionService();
	bool InstallCollectionService();
	bool UninstallCollectionService();
	bool RefreshCollectionServiceDataConfiguration();
	bool SetupWorkEnvironment();
	bool DumpConfigurationData();
	bool DumpSchemaData();
	bool InstallCollectionServiceManifests();
	bool UpdateConfiguration();

	//Flows Internal Actions
	namespace InternalActions
	{		
		bool UpdateLoggingData();
		bool ValidateConfigFileSyntax();
		bool SetupWorkBinaries();
		bool HardenSysmonXSecurity();
		bool DownloadAndExtract(const std::wstring &targetHost, const std::wstring &targetURLPath, const ProxyConfData &proxyData, const std::wstring &targetDirectory);
		bool ValidateFileAndExtract(const std::wstring &zipFile, const std::wstring &targetDirectory);
		bool UninstallPreviousInstances(const CommonTypes::TraceBackendType &backendType);
		bool SetupTraceBackend(const CommonTypes::TraceBackendType &backendType, const std::wstring &targetDirectory,
			std::wstring &target32BitsBackend, std::wstring &target64BitsBackend);
		bool InstallTraceBackend(const CommonTypes::TraceBackendType &backendType, std::wstring &target32BitsBackend, std::wstring &target64BitsBackend);
		bool SetupCollectionService(std::wstring &targetCollectionServiceFile);
		bool InstallCollectionServiceHelper(const std::wstring &targetCollectionServiceFile);
	}
}


namespace SysmonXServiceFlows
{
	//Flows Entry Points
	bool IsServiceWorkEnvironmentReady();
	bool EnableServiceProcessing();
	bool DisableServiceProcessing();

	//Flows Internal Actions
	namespace InternalActions
	{
		bool CheckServiceSanity();
		bool EnsureRunningConditions(const CommonTypes::TraceBackendType &backendType);
		bool SetupSysmonXComponents();
		bool EnableSysmonXComponents(const CommonTypes::TraceBackendType &backendType);
		bool UpdateSysmonXConfiguration(const CommonTypes::TraceBackendType &backendType, bool shouldServiceBeRestarted = false);
		bool DisableSysmonXComponents(const CommonTypes::TraceBackendType &backendType);
		bool CheckServiceImageFileIntegrity();
		bool CheckServiceDirectorySecurity();
		bool GetSysmonServiceNameFromFile(const std::wstring &serviceImageFileName, std::wstring &serviceName);
		bool RunUpdateSysmonConfigurationCommand(const std::wstring &backendFile, const std::wstring &configFile);
	}
}