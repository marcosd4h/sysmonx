#include "common.h"

namespace SysmonXAppFlows
{
	//Running Collection Service Main Logic
	bool RunCollectionService()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() )
		{	
			logger.Info("RunCollectionService - About to start collection service");

			if (CollectorService::Instance().RunService())
			{
				logger.Info("RunCollectionService - Collection service was properly started");
				ret = true;
			}
			else
			{
				logger.Error("RunCollectionService - There was a problem running collector service");
			}			
		}
		else
		{
			logger.Error("RunCollectionService - Configuration was not initialized");
		}

		return ret;
	}


	//Installing Collection Service
	bool InstallCollectionService()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Info("InstallCollectionService - About to install Collection Service");

			//Getting configuration data
			logger.Trace("InstallCollectionService - Gathering running config");			
			
			//Asumming Sysmon as Driver Trace Backend for the moment
			const CommonTypes::TraceBackendType &traceBackendType = CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON; 			
			const CommonTypes::BackendInstallVector &traceBackendInstallVector = config.GetBackendInstallerVector();
			const std::wstring &traceBackendFileLocation = config.GetInstallLocation();
			const std::wstring &collectionServiceName = config.GetCollectionServiceName();
			const std::wstring &workingDirectory = config.GetSysmonXWorkingDirectory();
			const ProxyConfData &proxyData = config.GetProxyConfData();
			std::wstring target32BitsBackend;
			std::wstring target64BitsBackend;
			std::wstring targetCollectionService;

			//Only sysmon is supported backend at the moment
			if (traceBackendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{				
				logger.Trace("InstallCollectionService - Stopping previous instances");
				//TODO: mjo - add an argument to force uninstall. This should not be a default action
				if (!InternalActions::UninstallPreviousInstances(traceBackendType))
				{
					logger.Error("InstallCollectionService - There was a problem stopping previous instances of collection and trace backend services");
					return ret;
				}

				//TODO: mjo - check if a check for latest trace backend (downloaded sysmon version) can be added here	
				if (traceBackendInstallVector == CommonTypes::BackendInstallVector::BACKEND_INSTALLER_FILES)
				{
					logger.Trace("InstallCollectionService - About to extract sysmon as extended trace backend from filesystem location");
					if (!InternalActions::ValidateFileAndExtract(traceBackendFileLocation, workingDirectory))
					{
						logger.Error("InstallCollectionService - There was a problem installing sysmon from given location");
						return ret;
					}
				}
				else if (traceBackendInstallVector == CommonTypes::BackendInstallVector::BACKEND_INSTALLER_WEB)
				{
					logger.Trace("InstallCollectionService - About to download and extract sysmon as extended trace backend");
					if (!InternalActions::DownloadAndExtract(SysmonXDefs::SYSMON_DEFAULT_HOST, SysmonXDefs::SYSMON_DEFAULT_URI, proxyData, workingDirectory))
					{
						logger.Error("InstallCollectionService - There was a problem dowloading sysmon");
						return ret;
					}
				}
				else
				{
					logger.Error("InstallCollectionService - The given install vector is not valid");
					return ret;
				}
							
				logger.Trace("InstallCollectionService - Sysmon was downloaded and extracted, now setting it up");
				if (!InternalActions::SetupTraceBackend(traceBackendType, workingDirectory, target32BitsBackend, target64BitsBackend))
				{
					logger.Error("InstallCollectionService - There was a problem setting up sysmon files");
					return ret;
				}

				logger.Trace("InstallCollectionService - About to install sysmon");
				if (!InternalActions::InstallTraceBackend(traceBackendType, target32BitsBackend, target64BitsBackend))
				{
					logger.Error("InstallCollectionService - There was a problem installing sysmon");
					return ret;
				}

				logger.Trace("InstallCollectionService - Now checking current state of collection service");
				if (InternalActions::IsNewCollectionServiceInstallRequired())
				{
					logger.Trace("InstallCollectionService - About to setup collection Service");
					if (!InternalActions::SetupCollectionService(targetCollectionService))
					{
						InternalActions::UninstallPreviousInstances(traceBackendType);
						logger.Error("InstallCollectionService - There was a problem setting up collection service");
						return ret;
					}

					logger.Trace("InstallCollectionService - About to install collection Service");
					if (!InternalActions::InstallHelperCollectionService(targetCollectionService))
					{
						InternalActions::UninstallPreviousInstances(traceBackendType);
						logger.Error("InstallCollectionService - There was a problem installing collection service");
						return ret;
					}

					logger.Info("InstallCollectionService - Collection service was successfully installed.");
				}
				else
				{
					logger.Info("InstallCollectionService - Collection service was already installed.");
				}

				ret = true;
			}
			else
			{
				logger.Error("InstallCollectionService - Backend type is not supported");
			}			
		}
		else
		{
			logger.Error("InstallCollectionService - Configuration was not initialized");
		}

		return ret;
	}


	//Uninstalling Collection Service
	bool UninstallCollectionService()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Info("UninstallCollectionService - About to uninstall Collection Service");

			//Asumming Sysmon as Driver Trace Backend for the moment
			const CommonTypes::TraceBackendType &traceBackendType = CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON;
			std::wstring target32BitsBackend;
			std::wstring target64BitsBackend;
			std::wstring targetCollectionServiceFile;
			std::wstring targetCollectionService(config.GetCollectionServiceName());

			//Only sysmon is supported backend at the moment
			if (traceBackendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{
				logger.Trace("UninstallCollectionService - About to stop and uninstall backend service");

				//uninstalling target backend
				if (InternalActions::UninstallPreviousInstances(traceBackendType))
				{
					logger.Info("UninstallCollectionService - Collection and Backend Services were properly uninstalled");
					ret = true;
				}
				else
				{
					logger.Error("UninstallCollectionService - There was an error uninstalling collection and backend services");
				}
			}
			else
			{
				logger.Trace("UninstallCollectionService - Backend type is not supported");
			}			
		}
		else
		{
			logger.Error("UninstallCollectionService - Configuration was not initialized");
		}

		return ret;
	}


	//Updating Collection Service Data
	bool RefreshCollectionServiceDataConfiguration()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Info("RefreshCollectionServiceDataConfiguration - About to make the service to refresh its configuration");

			//Configuration is already updated on the registry, so only thing needed is around 
			//restarting collection service so new data is picked up
			if (ServiceHelpers::RestartTargetService(config.GetCollectionServiceName()))
			{
				logger.Info("RefreshCollectionServiceDataConfiguration - Collection service was restarted and new config was picked up");
				ret = true;
			}
			else
			{
				logger.Trace("RefreshCollectionServiceDataConfiguration - There was a problem restarting collector service for config update");
			}
		}
		else
		{
			logger.Error("RefreshCollectionServiceDataConfiguration - Configuration was not initialized");
		}

		return ret;
	}

	//Setting up running conditions for working service and mgmt app
	bool SetupWorkEnvironment()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Info("SetupWorkEnvironment - About to setup the current work environment");

			//Checking Working Directory configuration data
			if (GeneralHelpers::IsValidDirectory(config.GetSysmonXWorkingDirectory()))
			{
				logger.Trace(L"SetupWorkEnvironment - Working directory set to {}", config.GetSysmonXWorkingDirectory());
			}
			else
			{
				logger.Error(L"SetupWorkEnvironment - There was a problem accessing working directory at {}", config.GetSysmonXWorkingDirectory());
				return ret;
			}

			//Updating logging data
			if (InternalActions::UpdateLoggingData())
			{
				logger.Trace("SetupWorkEnvironment - Logger data was updated");				
			}
			else
			{
				logger.Error("SetupWorkEnvironment - There was a problem updating logging data");
				return ret;
			}

			//Not doing any of this on install or uninstall scenario
			if (!(config.WasUninstallRequested() || config.WasInstallRequested()))
			{
				//Validate configuration file
				logger.Trace("SetupWorkEnvironment - About to validate configuration file");
				if (InternalActions::ValidateConfigFileSyntax())
				{
					logger.Trace("SetupWorkEnvironment - Given configuration file syntax is valid");
				}
				else
				{
					logger.Error("SetupWorkEnvironment - There was a problem with given configuration file syntax");
					return ret;
				}

				//Setup work locations
				logger.Trace("SetupWorkEnvironment - About to setup work locations");
				if (InternalActions::AreWorkBinariesInSync())
				{
					logger.Trace("SetupWorkEnvironment - Work locations were properly setup");
				}
				else
				{
					logger.Error("SetupWorkEnvironment - There was a problem setting up work locations");
					return ret;
				}

				//Hardening SysmonX security
				logger.Trace("SetupWorkEnvironment - About to harden SysmonX security");
				if (InternalActions::HardenSysmonXSecurity())
				{
					logger.Trace("SetupWorkEnvironment - SysmonX security was hardened");
				}
				else
				{
					logger.Error("SetupWorkEnvironment - There was a problem hardening sysmonx security");
					return ret;
				}
			}

			logger.Info("SetupWorkEnvironment - Work environment was properly setup");

			ret = true;
		}
		else
		{
			logger.Error("SetupWorkEnvironment - Configuration was not initialized");
		}

		return ret;
	}

	//TODO: improve meaningfulness of stored data (ie SHA1 instead of just printting raw value 0)
	bool DumpConfigurationData()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		SysmonXTypes::ConfigSerializedData *configData;
		CommonTypes::ByteContainer rawBytes;

		if (config.IsInitialized() && 
			config.GetRegistryConfigData(rawBytes, configData) &&
			(configData != nullptr))
		{
			logger.Info("About to dump working configuration");
			logger.Info(" Version: {}", configData->Version);
			logger.Info(" HashAlgorithmToUse: {}", GeneralHelpers::GetSerializedString(configData->HashAlgorithmToUse));
			logger.Info(" ShouldCheckSignatureRevocation: {}", configData->ShouldCheckSignatureRevocation);
			logger.Info(" ShouldLogLoadingModules: {}", configData->OptionsFlags & SysmonBackendOptionFlags::SYSMON_OPTIONS_IMAGE_LOADING_ENABLED);
			logger.Info(" ShouldLogNetworkConnections: {}", configData->OptionsFlags & SysmonBackendOptionFlags::SYSMON_OPTIONS_NETWORK_TRACKING_ENABLED);
			logger.Info(" WorkerThreads: {}", configData->WorkerThreads);
			logger.Info(" CollectionServiceLoggingVerbosity: {}", configData->CollectionServiceLoggingVerbosity);

			logger.Info(" CollectionServiceName: {}", GeneralHelpers::GetSerializedString(configData->CollectionServiceName));
			logger.Info(" CollectionServiceLogfile: {}", GeneralHelpers::GetSerializedString(configData->CollectionServiceLogfile));
			logger.Info(" Backend32ServiceName: {}", GeneralHelpers::GetSerializedString(configData->Backend32BitsName));
			logger.Info(" Backend64ServiceName: {}", GeneralHelpers::GetSerializedString(configData->Backend64BitsName));
			logger.Info(" WorkingDirectory: {}", GeneralHelpers::GetSerializedString(configData->WorkingDirectory));
			logger.Info(" ConfigFile: {}", GeneralHelpers::GetSerializedString(configData->ConfigFile));

			logger.Info(" ProcessesLoadingModules - Size is {} - Dumping elements below:", configData->ProcessesLoadingModules.size());
			for (auto it = configData->ProcessesLoadingModules.begin(); it = configData->ProcessesLoadingModules.end(); it++)
			{
				cista::offset::string& workString(*it);
				logger.Info("   {}", workString.data());
			}

			logger.Info(" ProcessesNetworkConnections - Size is {} - Dumping elements below:", configData->ProcessesNetworkConnections.size());
			for (auto it = configData->ProcessesNetworkConnections.begin(); it = configData->ProcessesNetworkConnections.end(); it++)
			{
				cista::offset::string& workString(*it);
				logger.Info("   {}", workString.data());
			}

			logger.Info(" CollectionServiceLoggingChannel - Size is {} - Dumping elements below:", configData->CollectionServiceLoggingChannel.size());
			for (auto it = configData->CollectionServiceLoggingChannel.begin(); it = configData->CollectionServiceLoggingChannel.end(); it++)
			{
				unsigned int& workValue(*it);
				logger.Info("   {}", workValue);
			}

			logger.Info(" CollectionServiceReportChannel - Size is {} - Dumping elements below:", configData->CollectionServiceReportChannel.size());
			for (auto it = configData->CollectionServiceReportChannel.begin(); it = configData->CollectionServiceReportChannel.end(); it++)
			{
				unsigned int& workValue(*it);
				logger.Info("   {}", workValue);
			}

			logger.Info("Configuration dump is done");
			ret = true;
		}
		else
		{
			logger.Error("DumpConfigurationData - Configuration was not initialized or config data could not be read from registry");
		}

		return ret;
	}


	//TODO: mjo implement this
	bool DumpSchemaData()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Info("DumpSchemaData - About to dump current schema.");

			logger.Info("DumpSchemaData - Schema dumping finished");

			ret = true;
		}
		else
		{
			logger.Error("DumpSchemaData - Configuration was not initialized");
		}

		return ret;
	}

	//TODO: mjo implement this
	bool InstallCollectionServiceManifests()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Info("InstallCollectionServiceManifests - About to initialize collection service manifest.");

			logger.Info("InstallCollectionServiceManifests - Collection service manifest were properly initialized");

			ret = true;
		}
		else
		{
			logger.Error("InstallCollectionServiceManifests - Configuration was not initialized");
		}

		return ret;
	}

	bool UpdateConfiguration()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() &&
			SysmonXServiceFlows::InternalActions::UpdateSysmonXConfiguration(CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON, true))
		{			
			ret = true;
		}
		else
		{
			logger.Error("UpdateConfiguration - Configuration was not initialized");
		}

		return ret;
	}
}

namespace SysmonXAppFlows::InternalActions
{
	//Updating logging data for both service and mgmt app modes
	bool UpdateLoggingData()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();
		LoggerContainer loggerMode;
		std::wstring loggerFile;

		if (config.IsInitialized())
		{
			if (config.IsServiceMode())
			{
				loggerMode = config.GetLoggerOutputCollectionService();

				if (config.IsCollectionServiceLogfileAvailable())
				{
					loggerFile = config.GetCollectionServiceLogFile();
				}
				else
				{
					loggerFile = SysmonXDefs::DEFAULT_LOG_FILE;
				}
			}
			else
			{
				loggerMode = config.GetLoggerOutputModuleMGMTApp();

				if (config.IsConsoleLogfileAvailable())
				{
					loggerFile = config.GetConsoleLogFile();
				}
				else
				{
					loggerFile = SysmonXDefs::DEFAULT_LOG_FILE;
				}
			}

			if (logger.Initialize(loggerMode, GeneralHelpers::WStrToStr(loggerFile)))
			{
				//Setting verbosity once logging is initialized
				if (logger.IsInitialized())
				{
					logger.SetLevel(config.GetLoggingVerbosity());
					ret = true;
				}				
			}
			else
			{
				logger.Error("UpdateLoggingData - There was a problem initializing logging configuration");
			}
		}
		else
		{
			logger.Error("UpdateLoggingData - Configuration was not initialized");
		}

		return ret;
	}

	//TODO: mjo implement this
	bool ValidateConfigFileSyntax()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{

			ret = true;
		}
		else
		{
			logger.Error("ValidateConfigFileSyntax - Configuration was not initialized");
		}

		return ret;
	}

	bool AreWorkBinariesInSync()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Trace("AreWorkBinariesInSync - About to ensure we are running with syncronized binaries");

			//Checking binaries logic. Nothing to do if we are running as a service
			if (config.IsServiceMode())
			{
				logger.Trace("AreWorkBinariesInSync - We are running as a service. Nothing to do here.");
				ret = true;
			}
			else
			{
				logger.Trace("AreWorkBinariesInSync - We are running as a management app. Let's check that app binary and service binary are in sync.");

				std::wstring mgmtAppFileLocation;
				std::wstring collectServiceFileLocation;
				if (GeneralHelpers::GetCurrentProcessModuleFullPath(mgmtAppFileLocation) &&					
					GeneralHelpers::IsValidFile(mgmtAppFileLocation) &&
					config.GetFullPathCollectionServiceFile(collectServiceFileLocation))
				{
					std::wstring collectServiceName = config.GetCollectionServiceName();

					//Does collection service file exists?
					if (GeneralHelpers::IsValidFile(collectServiceFileLocation))
					{
						if (GeneralHelpers::AreFilesEqual(mgmtAppFileLocation, collectServiceFileLocation))
						{
							if (ServiceHelpers::IsServiceCreated(collectServiceName) &&
								ServiceHelpers::IsSameServiceExecutablePath(collectServiceName, collectServiceFileLocation))
							{
								logger.Trace("AreWorkBinariesInSync - Service is registered and both mgmt app and collection service binaries are in sync!");
								ret = true;
							}
							else
							{
								logger.Trace("AreWorkBinariesInSync - Service is not registered yet and both mgmt app and collection service binaries are in sync!");
								ret = true;
							}
						}
						else
						{
							logger.Trace("AreWorkBinariesInSync - Both mgmt app and collection service binaries are not in sync. Attempt syncing now.");

							if (!collectServiceName.empty() &&
								ServiceHelpers::IsServiceCreated(collectServiceName) &&
								ServiceHelpers::StopTargetService(collectServiceName))
							{
								logger.Trace("AreWorkBinariesInSync - Collection Service was found and it stop request was performed. Giving it some time for clean teardown.");
								Sleep(3000);

								if (GeneralHelpers::RemoveFile(collectServiceFileLocation) &&
									GeneralHelpers::FileCopy(mgmtAppFileLocation, collectServiceFileLocation))
								{
									//check service file registration if service 
									if (ServiceHelpers::IsServiceCreated(collectServiceName) &&
										ServiceHelpers::IsSameServiceExecutablePath(collectServiceName, collectServiceFileLocation) &&
										ServiceHelpers::StartTargetService(collectServiceName))
									{
										logger.Trace("AreWorkBinariesInSync - Service check was done. Both mgmt app and collection service binaries are now in sync!");
										ret = true;
									}
									else
									{
										logger.Trace("AreWorkBinariesInSync - Both mgmt app and collection service binaries are now in sync!");
										ret = true;
									}
								}
								else
								{
									logger.Error("AreWorkBinariesInSync - There was a problem trying to sync collection service and mgmt app.");
									return ret;
								}
							}
							else
							{
								logger.Error("AreWorkBinariesInSync - There was a problem stopping previous instance of collection service.");
								logger.Error("AreWorkBinariesInSync - Please try running -u command to uninstall the service before continuing.");
								return ret;
							}

						}
					}
					else
					{
						logger.Trace("AreWorkBinariesInSync - Collection service file doest not exist, so no need to sync.");
						ret = true;
					}
				}
				else
				{
					logger.Error("AreWorkBinariesInSync - There was a problem getting location of collection service and mgmt app");
					return ret;
				}
			}
		}
		else
		{
			logger.Error("AreWorkBinariesInSync - Configuration was not initialized");
		}

		return ret;
	}

	//TODO: mjo implement this
	bool HardenSysmonXSecurity()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			
			ret = true;
		}
		else
		{
			logger.Error("HardenSysmonXSecurity - Configuration was not initialized");
		}

		return ret;
	}

	//Downloading trace backend binaries
	bool DownloadAndExtract(const std::wstring &targetHost, const std::wstring &targetURLPath, const ProxyConfData &proxyData, const std::wstring &targetDirectory)
	{
		bool ret = false;
		const std::wstring TARGET_EXTENSION = L".zip";
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();
		std::wstring tempDirectory;
		std::wstring tempFilename;

		if (config.IsInitialized() &&
			!targetHost.empty() &&
			!targetURLPath.empty() &&
			!targetDirectory.empty())
		{
			logger.Trace(L"DownloadAndExtract - About download {} - {} and extract it into {}", targetHost, targetURLPath, targetDirectory);

			if (GeneralHelpers::IsValidDirectory(targetDirectory) &&
				GeneralHelpers::GetTemporaryDirLocation(tempDirectory) &&
				!tempDirectory.empty() &&
				GeneralHelpers::IsValidDirectory(tempDirectory) &&
				GeneralHelpers::GetTemporaryFileName(tempDirectory, TARGET_EXTENSION, tempFilename) &&
				!tempFilename.empty())
			{
				logger.Trace(L"DownloadAndExtract - Temp directory {} and Temp file {}", tempDirectory, tempFilename);

				if (GeneralHelpers::DownloadFile(targetHost, targetURLPath, proxyData, tempFilename) &&
					GeneralHelpers::IsValidFile(tempFilename) &&
					GeneralHelpers::UnzipZIPFileToFolder(tempFilename, targetDirectory))
				{
					logger.Trace(L"DownloadAndExtract - Temp directory {} and Temp file {}", tempDirectory, tempFilename);

					ret = true;
				}
				else
				{
					logger.Error("DownloadAndExtract - There was a problem downloading and unzipping the target url file");
				}

				//Removing file temp resources
				if (GeneralHelpers::IsValidFile(tempFilename))
				{
					GeneralHelpers::RemoveFile(tempFilename);
				}

				//Removing dir temp resources
				if (GeneralHelpers::IsValidDirectory(tempDirectory))
				{
					GeneralHelpers::RemoveWorkingDirectory(tempDirectory);
				}
			}
			else
			{
				logger.Error("DownloadAndExtract - There was a problem getting temp directory and temp file name");
			}
		}
		else
		{
			logger.Error("DownloadAndExtract - Configuration was not initialized or there was a problem with given arguments");
		}
		
		return ret;
	}

	bool ValidateFileAndExtract(const std::wstring &zipFile, const std::wstring &targetDirectory)
	{
		bool ret = false;
		const std::wstring TARGET_EXTENSION = L".zip";
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() &&
			!zipFile.empty() &&
			!targetDirectory.empty() &&
			GeneralHelpers::IsValidFile(zipFile) &&
			GeneralHelpers::IsValidDirectory(targetDirectory))
		{
			logger.Trace(L"ValidateFileAndExtract - About extract {} into {}", zipFile, targetDirectory);

			if (GeneralHelpers::UnzipZIPFileToFolder(zipFile, targetDirectory))
			{
				logger.Trace("ValidateFileAndExtract - Files were succesfully extracted");
				ret = true;
			}
			else
			{
				logger.Error("ValidateFileAndExtract - There was a problem with given files");
			}
		}
		else
		{
			logger.Error("ValidateFileAndExtract - Configuration was not initialized or there was a problem with given arguments");
		}

		return ret;
	}

	//Stopping previous instances
	//TODO: A bit aggresive. Probably would be better to make this optional through command line. Also, we should remove old files and old working directory
	bool UninstallPreviousInstances(const CommonTypes::TraceBackendType &backendType)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();
		std::wstring targetArchBinaryBackend;
		std::wstring targetToDeleteArchBinaryBackend;
		std::wstring targetArchBinary32Backend;
		std::wstring targetArchBinary64Backend;
		std::wstring targetCollectionServiceName;

		if (config.IsInitialized())
		{
			logger.Trace("UninstallPreviousInstances - About to uninstall previous trace backends and collection service instances");
			if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_CUSTOM)
			{
				logger.Error("UninstallPreviousInstances - Custom trace backend not currently supported");
			}
			else if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{
				logger.Trace("UninstallPreviousInstances - Uninstalling sysmon backend");

				logger.Trace("UninstallPreviousInstances - Checking Previous backend instance first");

				//TODO: FIX WORKING DIRECTORY
				if (GeneralHelpers::IsRunningAs64BitProcess())
				{
					if (config.IsNewBackend64Instance())
					{						
						targetArchBinaryBackend.assign(config.GetSysmonXWorkingDirectory() + config.GetPreviousBackend64ServiceName());
					}
					else
					{
						targetArchBinaryBackend.assign(config.GetSysmonXWorkingDirectory() + config.GetBackend64ServiceName());
					}
				}
				else
				{
					if (config.IsNewBackend32Instance())
					{
						targetArchBinaryBackend.assign(config.GetSysmonXWorkingDirectory() + config.GetPreviousBackend32ServiceName());
					}
					else
					{
						targetArchBinaryBackend.assign(config.GetSysmonXWorkingDirectory() + config.GetBackend32ServiceName());
					}
				}

				if (!targetArchBinaryBackend.empty() && 
					GeneralHelpers::IsValidFile(targetArchBinaryBackend) &&
					ServiceHelpers::IsServiceCreated(GeneralHelpers::GetFileNameWithoutExtension(GeneralHelpers::GetBaseFileName(targetArchBinaryBackend))))
				{
					logger.Trace(L"UninstallPreviousInstances - Previous instance of Sysmon found at {}", targetArchBinaryBackend);

					logger.Trace("UninstallPreviousInstances - Uninstalling backend now");

					//Running Sysmon Backend now
					CommonTypes::StringsContainer processOutput;
					DWORD exitCode = 0;
					std::wstring uninstallCommandLine(L" -nologo -u ");

					//checking about adding force only for uninstall flow, otherwise forcing it
					if (config.WasForceUninstallRequested())
					{
						logger.Trace("Force uninstall was requested.");
					}

					//Cheatting a little bit and always using force to uninstall sysmon trace backend
					uninstallCommandLine.append(L" force");

					if (GeneralHelpers::RunProcess(targetArchBinaryBackend, uninstallCommandLine, processOutput, exitCode) &&
						!processOutput.empty())
					{
						//Printing output
						for (auto it = processOutput.begin(); it != processOutput.end(); it++)
						{
							std::string line(*it);
							logger.Trace("UninstallPreviousInstances - {}", line.c_str());
						}
					}

					//dummy sync delay
					Sleep(2000);

					//Is backend service still there?
					if (ServiceHelpers::IsServiceCreated(GeneralHelpers::GetFileNameWithoutExtension(GeneralHelpers::GetBaseFileName(targetArchBinaryBackend))))
					{
						logger.Error("UninstallPreviousInstances - There was a problem uninstalling previous backend instance. Backend service is still there");
						return ret;
					}
					
					if (!GeneralHelpers::RemoveFile(targetArchBinaryBackend))
					{
						logger.Error("UninstallPreviousInstances - There was a problem removing old backend instance file");
						return ret;
					}
				}
				else
				{
					logger.Trace("UninstallPreviousInstances - There was no previous backend instance found");
				}
			}


			logger.Trace("UninstallPreviousInstances - Now checking previous collection service instance");

			if (config.IsNewCollectionServiceInstance())
			{
				targetCollectionServiceName.assign(config.GetPreviousCollectionServiceName());
			}
			else
			{
				targetCollectionServiceName.assign(config.GetCollectionServiceName());
			}

			//Now checking collection service instance
			if (!targetCollectionServiceName.empty())
			{
				if (ServiceHelpers::IsServiceCreated(targetCollectionServiceName))
				{
					if (ServiceHelpers::StopTargetService(targetCollectionServiceName) &&
						ServiceHelpers::DeleteService(targetCollectionServiceName))
					{
						logger.Trace("UninstallPreviousInstances - Previous collection service instance was deleted");
					}
					else
					{
						logger.Error("UninstallPreviousInstances - There was a problem uninstalling previous collection service instance");
						return ret;
					}

					//Best effort to try to delete old collection service file
					std::wstring workingServiceFile;
					if (config.GetFullPathCollectionServiceFile(workingServiceFile) && GeneralHelpers::IsValidFile(workingServiceFile))
					{
						if (!GeneralHelpers::RemoveFile(workingServiceFile))
						{
							logger.Error("UninstallPreviousInstances - There was a problem removing old collection service instance file");
							return ret;
						}
					}
				}

			}
			else
			{
				logger.Error("UninstallPreviousInstances - There was no previous collection service instance found");
				return ret;
			}

			ret = true;
		}
		else
		{
			logger.Error("UninstallPreviousInstances - Configuration was not initialized or there was a problem with given arguments");
		}

		return ret;
	}

	//Setting up and installing trace backends
	bool SetupTraceBackend(const CommonTypes::TraceBackendType &backendType, const std::wstring &targetDirectory, 
		std::wstring &target32BitsBackend, std::wstring &target64BitsBackend)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		logger.Trace("SetupTraceBackend - About to setup trace backend files");
		if (config.IsInitialized() &&
			!targetDirectory.empty() &&
			GeneralHelpers::IsValidDirectory(targetDirectory))
		{
			logger.Trace("SetupTraceBackend - About to setup target backend");

			std::wstring workingDirectory(targetDirectory);

			GeneralHelpers::AddPathTrailCharsIfNeeded(workingDirectory);

			if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_CUSTOM)
			{
				logger.Error("SetupTraceBackend - Custom trace backend not currently supported");
			}
			else if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{
				logger.Trace("SetupTraceBackend - Trace backend set to Sysmon.");
				logger.Trace("SetupTraceBackend - Checking if files are present.");

				std::wstring workingTraceBackend32Bits(workingDirectory);
				std::wstring workingTraceBackend64Bits(workingDirectory);

				workingTraceBackend32Bits.append(SysmonXDefs::SYSMON_TRACE_BACKEND_32BITS);
				workingTraceBackend64Bits.append(SysmonXDefs::SYSMON_TRACE_BACKEND_64BITS);

				if (!workingTraceBackend32Bits.empty() &&
					!workingTraceBackend64Bits.empty() &&
					GeneralHelpers::IsValidFile(workingTraceBackend32Bits) &&
					GeneralHelpers::IsValidFile(workingTraceBackend64Bits))
				{
					logger.Trace("SetupTraceBackend - Trace backend found, about to prepare it for usage.");

					std::wstring configAlignedTraceBackend32Bits;
					std::wstring configAlignedTraceBackend64Bits;
				
					//Renaming 32 and 64 bits backend files
					if (config.GetBackendFiles(configAlignedTraceBackend32Bits, configAlignedTraceBackend64Bits))
					{
						if ((workingTraceBackend32Bits != configAlignedTraceBackend32Bits) &&
							!GeneralHelpers::RenameFile(workingTraceBackend32Bits, configAlignedTraceBackend32Bits))
						{
							logger.Error("SetupTraceBackend - There was a problem setting up 32bits trace binaries.");
							return ret;
						}

						if ((workingTraceBackend64Bits != configAlignedTraceBackend64Bits) &&
							!GeneralHelpers::RenameFile(workingTraceBackend64Bits, configAlignedTraceBackend64Bits))
						{
							logger.Error("SetupTraceBackend - There was a problem setting up 64bits trace binaries.");
							return ret;
						}

						if (GeneralHelpers::IsValidFile(configAlignedTraceBackend32Bits) && GeneralHelpers::IsValidFile(configAlignedTraceBackend64Bits))
						{
							target32BitsBackend.assign(configAlignedTraceBackend32Bits);
							target64BitsBackend.assign(configAlignedTraceBackend64Bits);
							ret = true;
						}
					}
				}
			}
			else
			{
				logger.Error("SetupTraceBackend - Trace Backend not supported.");
			}			
		}
		else
		{
			logger.Error("SetupTraceBackend - Configuration was not initialized or there was a problem with given arguments");
		}

		return ret;
	}

	bool InstallTraceBackend(const CommonTypes::TraceBackendType &backendType, std::wstring &target32BitsBackend, std::wstring &target64BitsBackend)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Trace("InstallTraceBackend - About to install trace backend");
			if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_CUSTOM)
			{
				logger.Error("InstallTraceBackend - Custom trace backend not currently supported");
			}
			else if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{
				logger.Trace("InstallTraceBackend - Installing sysmon backend");

				std::wstring targetArchBinaryBackend;
				CommonTypes::StringsContainer processOutput;
				DWORD exitCode = 0;
				if (GeneralHelpers::IsRunningAs64BitProcess())
				{
					targetArchBinaryBackend.assign(target64BitsBackend);
					//Removing different arch instance
					GeneralHelpers::RemoveFile(target32BitsBackend);
				}
				else
				{
					targetArchBinaryBackend.assign(target32BitsBackend);
					//Removing different arch instance
					GeneralHelpers::RemoveFile(target64BitsBackend);
				}

				//Running Sysmon Backend now

				//Checking EULA status
				std::wstring installCMDArgs;
				bool shouldCreateWindow = false;
				DWORD sysmonAcceptEulaValue = 0;
				installCMDArgs.append(L" -i -nologo");
				if (config.WasInstallAcceptEulaRequested())
				{
					installCMDArgs.append(L" -accepteula");
				}
				else
				{
					if (RegistryHelpers::GetRegDWORDValue(HKEY_CURRENT_USER, SysmonXDefs::SYSMON_REGISTRY_EULA_KEY_LOCATION, SysmonXDefs::SYSMON_REGISTRY_EULA_VALUE_LOCATION, sysmonAcceptEulaValue))
					{
						if (sysmonAcceptEulaValue != 1)
						{
							shouldCreateWindow = true;
						}
					}
					else
					{
						//eula location does not exists
						shouldCreateWindow = true;							
					}
				}



				if (GeneralHelpers::RunProcess(targetArchBinaryBackend, installCMDArgs, processOutput, exitCode, shouldCreateWindow) &&
					!processOutput.empty())
				{
					//Printing output
					for (auto it = processOutput.begin(); it != processOutput.end(); it++)
					{
						std::string line(*it);
						logger.Trace("InstallTraceBackend - {}", line.c_str());
					}
				}

				//dummy sync delay
				Sleep(2000);

				//Is backend service still there?
				if (ServiceHelpers::IsServiceCreated(GeneralHelpers::GetFileNameWithoutExtension(GeneralHelpers::GetBaseFileName(targetArchBinaryBackend))))
				{
					logger.Trace("InstallTraceBackend - Sysmon was succesfully installed");
					ret = true;
				}
			}
		}
		else
		{
			logger.Error("InstallTraceBackend - Configuration was not initialized ");
		}

		return ret;
	}

	bool SetupCollectionService(std::wstring &targetCollectionServiceFile)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Trace("SetupCollectionService - About to setup collection service data");

			std::wstring currentProcessFullPath;
			
			if (GeneralHelpers::GetCurrentProcessFullPath(currentProcessFullPath))
			{
				logger.Trace(L"SetupCollectionService - Current process is at {}", currentProcessFullPath);
				logger.Trace(L"SetupCollectionService - About to copy current process to working dir at {}", config.GetSysmonXWorkingDirectory());

				std::wstring targetFile;
				if (config.GetFullPathCollectionServiceFile(targetFile) &&
					!targetFile.empty() &&
					GeneralHelpers::FileCopy(currentProcessFullPath, targetFile) &&
					GeneralHelpers::IsValidFile(targetFile))
				{
					targetCollectionServiceFile.append(targetFile);
					logger.Trace(L"SetupCollectionService - Collection service file was copied to {}", targetCollectionServiceFile);
					ret = true;
				}
				else
				{
					logger.Error("SetupCollectionService - There was a problem copying the target file");
				}
			}
			else
			{
				logger.Error("SetupCollectionService - There was a problem getting path to current process");
			}
		}
		else
		{
			logger.Error("SetupCollectionService - Configuration was not initialized ");
		}

		return ret;
	}

	bool InstallHelperCollectionService(const std::wstring &targetCollectionServiceFile)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		std::wstring collectionServiceName(config.GetCollectionServiceName());
		std::wstring collectionServiceDisplay(config.GetCollectionServiceName());

		if (config.IsInitialized())
		{
			logger.Trace(L"InstallCollectionService - About to install collection service {}", collectionServiceName);

			if (ServiceHelpers::IsServiceCreated(collectionServiceName))
			{
				logger.Trace("InstallCollectionService - Previous instance of service detected. Uninstalling it first");

				if (ServiceHelpers::StopTargetService(collectionServiceName) &&
					ServiceHelpers::DeleteService(collectionServiceName))
				{
					logger.Trace(L"InstallCollectionService - Previous instance of service {} was deleted", collectionServiceName);
				}
				else
				{
					logger.Error("InstallCollectionService - There was a problem removing previous instance of service");
				}
			}

			if (!ServiceHelpers::IsServiceCreated(collectionServiceName) &&				
				ServiceHelpers::RegisterService(
				 targetCollectionServiceFile,
				 SysmonXDefs::SERVICE_MODE_ARGS,
				 collectionServiceName,
				 collectionServiceDisplay))
			{
				logger.Trace(L"InstallCollectionService - Service {} was properly registered", collectionServiceName);
				
				logger.Trace(L"InstallCollectionService - About to start service {}", collectionServiceName);
				if (ServiceHelpers::StartTargetService(collectionServiceName))
				{
					logger.Trace(L"InstallCollectionService - Service {} was properly started", collectionServiceName);
					ret = true;
				}
				else
				{
					logger.Error(L"InstallCollectionService - There was a problem starting service {} ", collectionServiceName);
				}
			}
			else
			{
				logger.Error(L"InstallCollectionService - There was a problem registering service {} ", collectionServiceName);
			}

			ret = true;
		}
		else
		{
			logger.Error("InstallCollectionService - Configuration was not initialized ");
		}

		return ret;
	}


	bool IsNewCollectionServiceInstallRequired()
	{
		bool ret = true;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		std::wstring collectionServiceName(config.GetCollectionServiceName());

		if (config.IsInitialized())
		{
			if (AreWorkBinariesInSync() && ServiceHelpers::IsServiceStarted(collectionServiceName))
			{
				ret = false;
			}
		}

		return ret;
	}
}



namespace SysmonXServiceFlows
{
	//Preparing service work environment
	bool IsServiceWorkEnvironmentReady()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		//Asumming Sysmon as Driver Trace Backend for the moment
		const CommonTypes::TraceBackendType &traceBackendType = CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON;

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("IsServiceWorkEnvironmentReady - About to prepare collection service work environment");

			//Checking Service Sanity
			if (InternalActions::CheckServiceSanity())
			{
				logger.Trace("IsServiceWorkEnvironmentReady - Service Sanity is OK.");
			}
			else
			{
				logger.Error("IsServiceWorkEnvironmentReady - There was a problem found when checking Service Sanity.");
				return ret;
			}

			//Ensuring Running Conditions
			if (InternalActions::EnsureRunningConditions(traceBackendType))
			{
				logger.Trace("IsServiceWorkEnvironmentReady - Running conditions were met, Service Collection is OK to run.");
			}
			else
			{
				logger.Error("IsServiceWorkEnvironmentReady - There was a problem found when checking collection service running conditions.");
				return ret;
			}

			//Setting up SysmonX components
			if (InternalActions::SetupSysmonXComponents())
			{
				logger.Trace("IsServiceWorkEnvironmentReady - Collection Service components were properly setup.");
			}
			else
			{
				logger.Error("IsServiceWorkEnvironmentReady - There was a problem setting up collection service components.");
				return ret;
			}

			ret = true;
		}
		else
		{
			logger.Error("IsServiceWorkEnvironmentReady - Configuration was not initialized ");
		}

		return ret;
	}


	//Enabling service components to start generating and processing data
	bool EnableServiceProcessing()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		//Asumming Sysmon as Driver Trace Backend for the moment
		const CommonTypes::TraceBackendType &traceBackendType = CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON;

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("EnableServiceProcessing - About to enable collection service components for data generation and data processing");

			//Enabling SysmonX components
			if (InternalActions::EnableSysmonXComponents(traceBackendType))
			{
				logger.Trace("EnableServiceProcessing - Collection Service components were properly enabled.");
			}
			else
			{
				logger.Error("EnableServiceProcessing - There was a problem enabling collection service components.");
				return ret;
			}

			ret = true;
		}
		else
		{
			logger.Error("EnableServiceProcessing - Configuration was not initialized ");
		}

		return ret;
	}


	//Disabling service components from generating and processing data
	bool DisableServiceProcessing()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		//Asumming Sysmon as Driver Trace Backend for the moment
		const CommonTypes::TraceBackendType &traceBackendType = CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON;

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("DisableServiceProcessing - About to disable collection service components for data generation and data processing");

			//Disabling SysmonX components
			if (InternalActions::DisableSysmonXComponents(traceBackendType))
			{
				logger.Trace("DisableServiceProcessing - Collection Service components were properly disabled.");
			}
			else
			{
				logger.Error("DisableServiceProcessing - There was a problem disabling collection service components.");
				return ret;
			}

			ret = true;
		}
		else
		{
			logger.Error("DisableServiceProcessing - Configuration was not initialized ");
		}

		return ret;
	}
}


namespace SysmonXServiceFlows::InternalActions
{

	//Checking service current sanity status
	bool CheckServiceSanity()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("CheckServiceSanity - About to check for service sanity");

			//Checking Service Image File Integrity
			if (CheckServiceImageFileIntegrity())
			{
				logger.Trace("CheckServiceSanity - Service Image File integrity is OK.");
			}
			else
			{
				logger.Error("CheckServiceSanity - There was a problem found when checking Service Image File Integrity.");
				return ret;
			}

			//Checking Service Directory Security
			if (CheckServiceDirectorySecurity())
			{
				logger.Trace("CheckServiceSanity - Service Directory security is OK.");
			}
			else
			{
				logger.Error("CheckServiceSanity - There was a problem found when checking Service Directory Security.");
				return ret;
			}

			ret = true;
		}
		else
		{
			logger.Error("CheckServiceSanity - Configuration was not initialized.");
		}

		return ret;
	}


	//Making running service collection conditions are there (i.e. trace backend is running)
	bool EnsureRunningConditions(const CommonTypes::TraceBackendType &backendType)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();
		std::wstring workingBackendServiceName;

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("EnsureRunningConditions - About to check that required service conditions are there");

			if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_CUSTOM)
			{
				logger.Error("EnsureRunningConditions - Custom trace backend not currently supported");
			}
			else if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{
				logger.Trace("EnsureRunningConditions - Checking running conditions of sysmon backend");

				std::wstring targetBackendService;
				if (GeneralHelpers::IsRunningAs64BitProcess())
				{
					targetBackendService.assign(config.GetBackend64ServiceName());
				}
				else
				{
					targetBackendService.assign(config.GetBackend32ServiceName());
				}

				//Getting Service Name
				if (GetSysmonServiceNameFromFile(targetBackendService, workingBackendServiceName))
				{
					//Now checking if target backend service is created and on stopped state
					if (ServiceHelpers::IsServiceCreated(workingBackendServiceName))
					{
						//Service is available
						if (ServiceHelpers::IsServiceStopped(workingBackendServiceName))
						{
							//Service is started
							logger.Trace(L"EnsureRunningConditions - Sysmon backend under name {} is stopped. Attempting start operation!", workingBackendServiceName);

							//Service is stopped, starting it
							if (ServiceHelpers::StartTargetService(workingBackendServiceName))
							{
								//service is now stopped
								logger.Trace(L"EnsureRunningConditions - Sysmon backend under name {} is now started!", workingBackendServiceName);
							}
							else
							{
								//service cannot be stopped
								logger.Error(L"EnsureRunningConditions - Sysmon backend under name {} cannot be started!", workingBackendServiceName);
								return ret;
							}
						}

						//Update configuration of backend trace sysmon
						if (UpdateSysmonXConfiguration(CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON, false))
						{
							//service is now stopped
							logger.Trace(L"EnsureRunningConditions - Trace Backend configuration {} was updated!", workingBackendServiceName);
						}
						else
						{
							//service cannot be stopped
							logger.Error(L"EnsureRunningConditions - There was a problem updating the configuration of trace backedn {}", workingBackendServiceName);
							return ret;
						}
					}
					else
					{
						//service is not available
						logger.Error(L"EnsureRunningConditions - Sysmon backend under name {} is not available!", workingBackendServiceName);
						return ret;
					}
				}
				else
				{
					//service name is not available
					logger.Error("EnsureRunningConditions - There was a problem getting trace backend service name");
					return ret;
				}

				ret = true;
			}			
		}
		else
		{
			logger.Error("EnsureRunningConditions - Configuration was not initialized.");
		}

		return ret;
	}


	//Setting up internal collection service components. This basically means few things
	// - Initialization of components
	// - passing configuration data to different components (config file, report channels enabled, etc)
	bool SetupSysmonXComponents()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() && config.IsServiceMode())
		{
			//Report Manager Initialization
			if (ReportManager::Instance().Initialize())
			{
				logger.Trace("SetupSysmonXComponents - Report Manager Component was properly initialized");

				if (ReportManager::Instance().SetTargetReportChannels(config.GetReportOutputList()))
				{
					logger.Trace("SetupSysmonXComponents - Target report channels were properly set at Report Mananger Component");
				}
				else
				{
					logger.Error("SetupSysmonXComponents - There was a setting target report channels at Report Manager Component");
					return ret;
				}
			}
			else
			{
				logger.Error("SetupSysmonXComponents - There was a problem initializing Report Manager Component");
				return ret;
			}

			//Events Manager Initialization
			if (EventsManager::Instance().Initialize())
			{
				logger.Trace("SetupSysmonXComponents - Events Manager Component was properly initialized");

				if (EventsManager::Instance().AreCollectorsReady())
				{
					logger.Trace("SetupSysmonXComponents - Target Events Collectors were properly set at Events Mananger Component");
				}
				else
				{
					logger.Error("SetupSysmonXComponents - There was a setting target Events Collectors at Events Manager Component");
					return ret;
				}
			}
			else
			{
				logger.Error("SetupSysmonXComponents - There was a problem initializing Events Manager Component");
				return ret;
			}

			//Correlation Engine Initialization
			if (MatchingEngine::Instance().Initialize())
			{
				logger.Trace("SetupSysmonXComponents - Correlation Engine Component was properly initialized");
			}
			else
			{
				logger.Error("SetupSysmonXComponents - There was a problem initializing Correlation Engine Component");
				return ret;
			}

			//Rules Manager Initialization
			if (RulesManager::Instance().Initialize())
			{
				logger.Trace("SetupSysmonXComponents - Rules Manager Component was properly initialized");

				if (RulesManager::Instance().IsConfigFileSyntaxValid(config.GetConfigurationFile()))
				{
					logger.Trace("SetupSysmonXComponents - Configuration File syntax was validated");
				}
				else
				{
					logger.Error("SetupSysmonXComponents - There was a problem found with given configuration file syntax");
					return ret;
				}

				if (RulesManager::Instance().PopulateMatchingEngineMatchers(config.GetConfigurationFile()))
				{
					logger.Trace("SetupSysmonXComponents - Populating matching engines is done");
				}
				else
				{
					logger.Error("SetupSysmonXComponents - There was a populating matching engines with configuration");
					return ret;
				}
			}
			else
			{
				logger.Error("SetupSysmonXComponents - There was a problem initializing Rules Manager Component");
				return ret;
			}

			ret = true;
		}
		else
		{
			logger.Error("SetupSysmonXComponents - Configuration was not initialized.");
		}

		return ret;
	}


	//Updating SysmonX trace backend configuration
	bool UpdateSysmonXConfiguration(const CommonTypes::TraceBackendType &backendType, bool shouldServiceBeRestarted)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();
		std::wstring workingServiceName;
		std::wstring backendConfigFile;

		if (config.IsInitialized() && config.IsServiceMode())
		{
			//Enabling backend components
			if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_CUSTOM)
			{
				logger.Error("UpdateSysmonXConfiguration - Custom trace backend not currently supported");
			}
			else if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{
				logger.Trace("UpdateSysmonXConfiguration - Checking running conditions of sysmon backend");


				if (config.GetFullPathBackendConfigFile(backendConfigFile) &&
					!backendConfigFile.empty() &&
					ConfigManager::Instance().GenerateBackendConfigFile(backendConfigFile))
				{
					logger.Trace(L"UpdateSysmonXConfiguration - Trace backend config file was written at {}", backendConfigFile);
				}
				else
				{
					logger.Error("UpdateSysmonXConfiguration - There was a problem writting to trace backend config file");
					return ret;
				}

				std::wstring targetBackendService;
				std::wstring targetBackendServiceFullPath;
				std::wstring workingBackendFullPath32bits;
				std::wstring workingBackendFullPath64bits;
				if (config.GetBackendFiles(workingBackendFullPath32bits, workingBackendFullPath64bits) &&
					!workingBackendFullPath32bits.empty() &&
					!workingBackendFullPath64bits.empty())
				{
					if (GeneralHelpers::IsRunningAs64BitProcess())
					{
						targetBackendService.assign(config.GetBackend64ServiceName());
						targetBackendServiceFullPath.assign(workingBackendFullPath64bits);
					}
					else
					{
						targetBackendService.assign(config.GetBackend32ServiceName());
						targetBackendServiceFullPath.assign(workingBackendFullPath32bits);
					}
				}

				//Starting service by name
				if (GetSysmonServiceNameFromFile(targetBackendService, workingServiceName))
				{
					//Now checking if target backend service is created and on stopped state before starting it
					if (ServiceHelpers::IsServiceCreated(workingServiceName))
					{
						if (ServiceHelpers::IsServiceStopped(workingServiceName))
						{
							logger.Trace(L"UpdateSysmonXConfiguration - Sysmon backend under name {} is already stopped, about to start it!", workingServiceName);
							if (ServiceHelpers::StartTargetService(workingServiceName) && ServiceHelpers::IsServiceStarted(workingServiceName))
							{
								logger.Trace(L"UpdateSysmonXConfiguration - Sysmon backend under name {} is running!", workingServiceName);
							}
							else
							{
								logger.Error(L"UpdateSysmonXConfiguration - Sysmon backend under name {} cannot be started!", workingServiceName);
								return ret;
							}
						}
						else
						{
							logger.Trace(L"UpdateSysmonXConfiguration - Sysmon backend under name {} is already running!", workingServiceName);
						}
					}
					else
					{
						logger.Error(L"UpdateSysmonXConfiguration - Sysmon backend under name {} is not available!", workingServiceName);
						return ret;
					}
				}
				else
				{
					//service name is not available
					logger.Error("UpdateSysmonXConfiguration - There was a problem getting trace backend service name");
					return ret;
				}

				//Updating sysmon with current config file
				if (GeneralHelpers::IsValidFile(backendConfigFile))
				{
					//Now updating configuration file
					if (RunUpdateSysmonConfigurationCommand(targetBackendServiceFullPath, backendConfigFile))
					{
						logger.Trace(L"UpdateSysmonXConfiguration - Configuration file at {} was properly updated", backendConfigFile);
					}
					else
					{
						//service name is not available
						logger.Error(L"UpdateSysmonXConfiguration - There was a problem updating backend configuration file at {}", backendConfigFile);
						return ret;
					}
				}

				//Deleting trace backend config file
				/*
				if (GeneralHelpers::RemoveFile(backendConfigFile))
				{
					logger.Trace(L"UpdateSysmonXConfiguration - Trace backend config file was removed at {} ", backendConfigFile);
				}
				else
				{
					logger.Error("UpdateSysmonXConfiguration - There was a problem removing trace backend config file");
					return ret;
				}
				*/
				//Restart service so it can pick up new config
				if (shouldServiceBeRestarted)
				{
					//Now checking if target backend service is created and on stopped state before starting it
					std::wstring workingSysmonXServiceName = config.GetCollectionServiceName();
					if (ServiceHelpers::IsServiceCreated(workingSysmonXServiceName) && ServiceHelpers::RestartTargetService(workingSysmonXServiceName))
					{
						logger.Trace("UpdateSysmonXConfiguration - Collection service was properly started");
					}
					else
					{
						//service name is not available
						logger.Error("UpdateSysmonXConfiguration - There was a problem restarting collection service");
						return ret;
					}
				}

				ret = true;
			}
		}
		else
		{
			logger.Error("UpdateSysmonXConfiguration - Configuration was not initialized.");
		}

		return ret;
	}

	//Starting internal sysmonx collection service components
	bool EnableSysmonXComponents(const CommonTypes::TraceBackendType &backendType)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();
		std::wstring workingServiceName;

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("EnableSysmonXComponents - About to shutdown sysmonx collection service components");

			//Event Processing start collection signal
			if (EventsManager::Instance().StartEventsCollection())
			{
				logger.Trace("EnableSysmonXComponents - Events Manager events collection process was properly started");
			}
			else
			{
				logger.Error("EnableSysmonXComponents - There was a problem starting Events Manager collection process");
				return ret;
			}

			ret = true;
		}
		else
		{
			logger.Error("EnableSysmonXComponents - Configuration was not initialized.");
		}

		return ret;
	}


	//Stopping internal sysmonx collection service components
	bool DisableSysmonXComponents(const CommonTypes::TraceBackendType &backendType)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();
		std::wstring workingServiceName;

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("DisableSysmonXComponents - About to shutdown sysmonx collection service components");

			//Event Processing stop collection signal
			if (EventsManager::Instance().StopEventsCollection())
			{
				logger.Trace("DisableSysmonXComponents - Events Manager events collection process was properly stopped");
			}
			else
			{
				logger.Error("DisableSysmonXComponents - There was a problem stopping Events Manager collection process");
				return ret;
			}

			//Enabling backend components
			if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_CUSTOM)
			{
				logger.Error("DisableSysmonXComponents - Custom trace backend not currently supported");
			}
			else if (backendType == CommonTypes::TraceBackendType::TRACE_BACKEND_SYSMON)
			{
				logger.Trace("DisableSysmonXComponents - Checking running conditions of sysmon backend");

				std::wstring targetBackendService;
				if (GeneralHelpers::IsRunningAs64BitProcess())
				{
					targetBackendService.assign(config.GetBackend64ServiceName());
				}
				else
				{
					targetBackendService.assign(config.GetBackend32ServiceName());
				}
				
				//Stopping service by name
				if (GetSysmonServiceNameFromFile(targetBackendService, workingServiceName))
				{
					//Now checking if target backend service is created and on started state before stopping it
					if (ServiceHelpers::IsServiceCreated(workingServiceName))
					{
						if (ServiceHelpers::IsServiceStarted(workingServiceName))
						{
							logger.Trace(L"DisableSysmonXComponents - Sysmon backend under name {} is already started, about to stop it!", workingServiceName);
							if (ServiceHelpers::StopTargetService(workingServiceName) && ServiceHelpers::IsServiceStopped(workingServiceName))
							{
								logger.Trace(L"DisableSysmonXComponents - Sysmon backend under name {} is stopped!", workingServiceName);
							}
							else
							{
								logger.Error(L"DisableSysmonXComponents - Sysmon backend under name {} cannot be stopped!", workingServiceName);
								return ret;
							}
						}
						else
						{
							logger.Trace(L"DisableSysmonXComponents - Sysmon backend under name {} is already stopped!", workingServiceName);
						}
					}
					else
					{
						logger.Error(L"DisableSysmonXComponents - Sysmon backend under name {} is not available!", workingServiceName);
						return ret;
					}
				}
				else
				{
					//service name is not available
					logger.Error("DisableSysmonXComponents - There was a problem getting trace backend service name");
					return ret;
				}
			}

			ret = true;
		}
		else
		{
			logger.Error("DisableSysmonXComponents - Configuration was not initialized.");
		}

		return ret;
	}


	//TODO: MJO Implement this
	bool CheckServiceImageFileIntegrity()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("CheckServiceImageFileIntegrity - About to check collection service image file integrity");

			ret = true;
		}
		else
		{
			logger.Error("CheckServiceImageFileIntegrity - Configuration was not initialized.");
		}

		return ret;
	}


	//TODO: MJO Implement this
	bool CheckServiceDirectorySecurity()
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() && config.IsServiceMode())
		{
			logger.Trace("CheckServiceDirectorySecurity - About to check collection service working directory security checks");

			ret = true;
		}
		else
		{
			logger.Error("CheckServiceDirectorySecurity - Configuration was not initialized.");
		}

		return ret;
	}

	bool GetSysmonServiceNameFromFile(const std::wstring &serviceImageFileName, std::wstring &serviceName)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized())
		{
			logger.Trace("GetSysmonServiceNameFromFile - About to get Sysmon Service Name from Image File Name");

			std::wstring workingServiceName;
			if (GeneralHelpers::GetFileNameWithoutExtension(serviceImageFileName, workingServiceName) &&
				!workingServiceName.empty())
			{
				//Now checking if target backend service is created and on started state before stopping it
				if (ServiceHelpers::IsServiceCreated(workingServiceName))
				{
					serviceName.assign(workingServiceName);
					ret = true;
				}
				else
				{
					logger.Error(L"GetSysmonServiceNameFromFile - Sysmon backend under name {} is not available!", workingServiceName);
					return ret;
				}
			}
			else
			{
				//service name is not available
				logger.Error("GetSysmonServiceNameFromFile - There was a problem getting trace backend service name");
				return ret;
			}			
		}
		else
		{
			logger.Error("GetSysmonServiceNameFromFile - Configuration was not initialized.");
		}

		return ret;
	}

	bool RunUpdateSysmonConfigurationCommand(const std::wstring &backendFile, const std::wstring &configFile)
	{
		bool ret = false;
		TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
		ConfigManager &config = ConfigManager::Instance();

		if (config.IsInitialized() &&
			GeneralHelpers::IsValidFile(backendFile) &&
			GeneralHelpers::IsValidFile(configFile))
		{
			logger.Trace(L"UpdateSysmonConfiguration - About to update sysmon configuration with {} ", configFile);

			//Running Sysmon Backend now to update configuration
			DWORD exitCode;
			CommonTypes::StringsContainer processOutput;
			std::wstring configCMDArgs;
			configCMDArgs.append(L" -nologo -c ");
			configCMDArgs.append(configFile);

			//Running Process
			if (GeneralHelpers::RunProcess(backendFile, configCMDArgs, processOutput, exitCode) &&
				!processOutput.empty())
			{
				//Printing output
				for (auto it = processOutput.begin(); it != processOutput.end(); it++)
				{
					std::string line(*it);
					logger.Trace("UpdateSysmonConfiguration - {}", line.c_str());
				}
			}

			if ((exitCode == SysmonXDefs::SYSMON_EXIT_CODE_OK) ||
				(exitCode == SysmonXDefs::SYSMON_EXIT_CODE_ALREADY_INSTALLED) ||
				(exitCode == SysmonXDefs::SYSMON_EXIT_CODE_ALREADY_RUNNING))
			{
				logger.Trace("UpdateSysmonConfiguration - Sysmon configuration was succesfully updated");
				ret = true;
			}

		}
		else
		{
			logger.Error("UpdateSysmonConfiguration - Configuration was not initialized or there was a problem with given arguments.");
		}

		return ret;
	}
}