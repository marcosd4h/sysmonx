#include "common.h"

bool ConfigManager::Initialize(int argc, wchar_t *argv[])
{
	bool ret = false;
	CmdArgsParser cmdArgs;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();

	try 
	{
		//attempting to initialize config using user provided data
		if (cmdArgs.Initialize(argc, argv))
		{
			//Parsing boolean values: -service. Service Mode
			if (cmdArgs.WasOptionRequested(SysmonXDefs::SERVICE_MODE_ARGS))
			{
				m_isServiceMode = true;

				//TODO: mjo delete this!
				if (cmdArgs.WasOptionRequested(L"-x")) m_wasPrintCurrentConfigurationRequested = true;

				//Build System data
				if (SyncRuntimeConfigData())
				{
					//No exceptions and parsing went OK
					m_isInitialized = true;
					ret = true;
					return ret;
				}
				else
				{
					logger.TraceDown("There was a problem syncing runtime configuration data");
				}
			}
			else
			{
				m_isServiceMode = false;
			}

			//Parsing boolean values: -i. Install Request
			if (cmdArgs.WasOptionRequested(L"-i"))
			{
				m_wasInstallRequested = true;

				//checking if EULA accept was requested
				if (cmdArgs.WasOptionRequestedInsensitive(L"-accepteula"))
				{
					m_wasInstallAcceptEulaRequested = true;
				}
				else
				{
					m_wasInstallAcceptEulaRequested = false;
				}
			}
			else
			{
				m_wasInstallRequested = false;
			}

			//Parsing boolean values: -u. Uninstall Request
			if (cmdArgs.WasOptionRequested(L"-u"))
			{
				m_wasUninstallRequested = true;

				std::wstring workingValue = cmdArgs.GetOptionValue(L"-u");
				
				//fuzzy matching of force request
				if (GeneralHelpers::StrContainsPatternInsensitive(workingValue, L"force"))
				{
					m_wasUninstallForceRequested = true;
				}
				else
				{
					m_wasUninstallForceRequested = false;
				}
			}
			else
			{
				m_wasUninstallRequested = false;
			}

			//Parsing boolean values: -r. Check Revocation Request
			if (cmdArgs.WasOptionRequested(L"-r"))
			{
				m_wasSignatureRevocationCheckRequested = true;
			}
			else
			{
				m_wasSignatureRevocationCheckRequested = false;
			}


			//Parsing boolean values: -s. Schema Dump
			if (cmdArgs.WasOptionRequested(L"-s"))
			{
				m_wasDumpOfSchemaDefinitionsRequested = true;
			}
			else
			{
				m_wasDumpOfSchemaDefinitionsRequested = false;
			}

			//Parsing boolean values: -m. Install event manifest
			if (cmdArgs.WasOptionRequested(L"-m"))
			{
				m_wasInstallEventsManifestRequested = true;
			}
			else
			{
				m_wasInstallEventsManifestRequested = false;
			}

			//Parsing boolean values: -x. Prints collection service info
			if (cmdArgs.WasOptionRequested(L"-x"))
			{
				m_wasPrintCurrentConfigurationRequested = true;
			}
			else
			{
				m_wasPrintCurrentConfigurationRequested = false;
			}

			//Parsing boolean values: -?. Prints help info
			if (cmdArgs.WasOptionRequested(L"-?"))
			{
				m_wasHelpRequested = true;
			}
			else
			{
				m_wasHelpRequested = false;
			}

			//Parsing string values: -c. Configuration file
			if (cmdArgs.WasOptionRequested(L"-c"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-c");

				//checking if value is not empty
				if (!workingValue.empty() && ValidateConfigurationFile(workingValue))
				{
					m_wasNewConfigFileRequested = true;
					m_configFileSyntaxOK = true;
					m_configurationFile.assign(workingValue);
				}
				else
				{
					std::wstring currentDirectoryWorkingFile;
					if (GeneralHelpers::GetCurrentProcessModuleDirectory(currentDirectoryWorkingFile))
					{
						if (currentDirectoryWorkingFile.back() != '\\') currentDirectoryWorkingFile.push_back('\\');

						currentDirectoryWorkingFile.append(workingValue);
						if (!currentDirectoryWorkingFile.empty() && ValidateConfigurationFile(currentDirectoryWorkingFile))
						{
							m_wasNewConfigFileRequested = true;
							m_configFileSyntaxOK = true;
							m_configurationFile.assign(workingValue);
						}
						else
						{
							logger.TraceConsoleDown(" The given configuration file cannot be found.");
							return ret;
						}
					}
				}
			}

			//Parsing string values: -d. Trace backend driver name
			if (cmdArgs.WasOptionRequested(L"-d"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-d");

				//checking if value is not empty
				if (!workingValue.empty())
				{
					m_traceBackendName.assign(workingValue);

					GenerateTraceBackendNames(m_traceBackendName, m_backend32ServiceName, m_backend64ServiceName);
				}
			}

			//Parsing string values: -z. Trace backend driver name
			if (cmdArgs.WasOptionRequested(L"-z"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-z");

				//checking if value is not empty
				if (!workingValue.empty())
				{
					m_collectionServiceName.assign(workingValue);
				}
			}

			//Parsing string values: -t. Install Location
			if (cmdArgs.WasOptionRequested(L"-t"))
			{
				std::wstring workingValue1;
				std::wstring workingValue2;

				//checking if value is not empty
				if (cmdArgs.GetTwoOptionValue(L"-t", workingValue1, workingValue2))
				{
					if (!workingValue1.empty()) m_backendInstallerVector = GetBackendLocation(workingValue1);
					if (!workingValue2.empty()) m_installLocation.assign(workingValue2);
				}
			}

			//Parsing string values: -p. Proxy Details
			if (cmdArgs.WasOptionRequested(L"-p"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-p");

				//checking if value is not empty
				if (!workingValue.empty())
				{				
					if (GetParsedProxyConfData(workingValue, m_proxyConfData))
					{
						m_proxyRawData.assign(workingValue);
					}
				}
			}

			//Parsing string values: -e. Collection Service Log File
			if (cmdArgs.WasOptionRequested(L"-e"))
			{
				std::wstring workingValue1;
				std::wstring workingValue2;
				m_wasCollectionLoggingRequested = true;
				//checking if value is not empty
				if (cmdArgs.GetTwoOptionValue(L"-e", workingValue1, workingValue2))
				{
					if(!workingValue1.empty()) m_loggingCollectionService = GetLoggerMode(workingValue1);
					if(!workingValue2.empty()) m_collectionServiceLogfile.assign(workingValue2);
				}
			}

			//Parsing string values: -f. Mgmt App Log File
			if (cmdArgs.WasOptionRequested(L"-f"))
			{
				std::wstring workingValue1;
				std::wstring workingValue2;

				//checking if value is not empty
				if (cmdArgs.GetTwoOptionValue(L"-f", workingValue1, workingValue2))
				{
					if (!workingValue1.empty()) m_loggingMgmtApp = GetLoggerMode(workingValue1);
					if (!workingValue2.empty()) m_consoleLogfile.assign(workingValue2);
				}
			}

			//Parsing string values: -v. Verbosity Mode
			if (cmdArgs.WasOptionRequested(L"-v"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-v");
				m_wasVerbosityRequested = true;
				//checking if value is not empty
				if (!workingValue.empty())
				{
					m_loggingVerbosity = GetVerbosityMode(workingValue);
				}
			}

			//Parsing string values: -o. Report Options
			if (cmdArgs.WasOptionRequested(L"-o"))
			{
				std::wstring workingValue1;
				std::wstring workingValue2;
				m_wasReportOptionsRequested = true;
				//checking if value is not empty
				if (cmdArgs.GetTwoOptionValue(L"-o", workingValue1, workingValue2))
				{					
					if (!workingValue1.empty()) m_reportOutputList = GetReportList(workingValue1);
					if (!workingValue2.empty()) m_reportLogfile.assign(workingValue2);
				}
			}

			//Parsing string values: -g. Worker Threads
			if (cmdArgs.WasOptionRequested(L"-g"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-g");
				m_wasNumberOfWorkersRequested = true;
				//checking if value is not empty
				if (!workingValue.empty() && GeneralHelpers::IsNumber(workingValue))
				{
					m_nrWorkerOrchThreads = std::stoi(workingValue);
				}
			}

			//Parsing string values: -w. Working Directory
			if (cmdArgs.WasOptionRequested(L"-w"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-w");

				//checking if value is not empty
				if (!workingValue.empty() && GeneralHelpers::IsValidDirectory(m_workingDirectory))
				{
					m_workingDirectory.assign(workingValue);
				}
			}

			//Parsing string values: -h. Working hash
			if (cmdArgs.WasOptionRequested(L"-h"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-h");

				//checking if value is not empty
				if (!workingValue.empty())
				{
					m_hashAlgorithmToUse = GetHashAlgorithm(workingValue);
				}
			}


			//Parsing string values: -l. Log loading modules
			if (cmdArgs.WasOptionRequested(L"-l"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-l");

				//checking if value is not empty
				if (!workingValue.empty())
				{
					m_processesToTrackOnLoadingModules = GetProcessList(workingValue);
				}
			}

			//Parsing string values: -n. Log processes on network connections
			if (cmdArgs.WasOptionRequested(L"-n"))
			{
				std::wstring workingValue = cmdArgs.GetOptionValue(L"-n");

				//checking if value is not empty
				if (!workingValue.empty())
				{
					m_processesToTrackOnNetworkConnections = GetProcessList(workingValue);
				}
			}

			//Check system data available
			if (IsSystemDataAvailable())
			{
				m_isServiceDataAvailable = true;
			}
			else
			{
				m_isServiceDataAvailable = false;
			}

			//Build System data
			if (SyncRuntimeConfigData())
			{
				//No exceptions and parsing went OK
				m_isInitialized = true;
				ret = true;
			}
			else
			{
				if (m_isServiceMode)
				{
					logger.TraceDown("There was a problem syncing runtime configuration data");
				}
				else
				{
					logger.TraceConsoleDown("There was a problem syncing runtime configuration data");
				}
			}
		}
	}
	catch (...)
	{
		logger.TraceDown("There was a problem initializing given configuration");
	}

	return ret;
}

const BackendInstallVector ConfigManager::GetBackendLocation(const std::wstring &backendLocation)
{
	BackendInstallVector ret = BackendInstallVector::BACKEND_INSTALLER_NA;

	if (!backendLocation.empty())
	{
		if (GeneralHelpers::StrCompareInsensitive(L"web", backendLocation))
		{
			ret = BackendInstallVector::BACKEND_INSTALLER_WEB;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"files", backendLocation))
		{
			ret = BackendInstallVector::BACKEND_INSTALLER_FILES;
		}
		else
		{
			TraceHelpers::Logger::Instance().TraceConsoleDown("There was a problem with given backend location. Backend location will be disabled");
			ret = BackendInstallVector::BACKEND_INSTALLER_NA;
		}
	}

	return ret;
}


const LoggerContainer ConfigManager::GetLoggerMode(const std::wstring &loggerMode)
{
	LoggerContainer ret;

	std::wstring workString(loggerMode);

	if (!workString.empty() && GeneralHelpers::TrimSpaces(workString))
	{
		CommonTypes::WStringsContainer tokensVector;
		if ((GeneralHelpers::GetVectorByToken(workString, L',', tokensVector)) && (tokensVector.size() > 0))
		{
			for (auto it = tokensVector.begin(); it != tokensVector.end(); ++it)
			{
				const std::wstring &targetMode(*it);
				if (!loggerMode.empty())
				{
					if (GeneralHelpers::StrCompareInsensitive(L"console", loggerMode))
					{
						ret.push_back(LoggerMode::LOGGER_CONSOLE);
					}
					else if (GeneralHelpers::StrCompareInsensitive(L"file", loggerMode))
					{
						ret.push_back(LoggerMode::LOGGER_FILE);
					}
				}
			}
		}
	}

	return ret;
}


const LoggerVerbose ConfigManager::GetVerbosityMode(const std::wstring &verbosityMode)
{
	LoggerVerbose ret = LoggerVerbose::LOGGER_TRACE;

	if (!verbosityMode.empty())
	{
		if (GeneralHelpers::StrCompareInsensitive(L"critical", verbosityMode))
		{
			ret = LoggerVerbose::LOGGER_CRITICAL;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"debug", verbosityMode))
		{
			ret = LoggerVerbose::LOGGER_DEBUG;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"error", verbosityMode) || 
				 GeneralHelpers::StrCompareInsensitive(L"err", verbosityMode))
		{
			ret = LoggerVerbose::LOGGER_ERR;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"info", verbosityMode))
		{
			ret = LoggerVerbose::LOGGER_INFO;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"off", verbosityMode))
		{
			ret = LoggerVerbose::LOGGER_OFF;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"trace", verbosityMode))
		{
			ret = LoggerVerbose::LOGGER_TRACE;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"warning", verbosityMode) ||
				 GeneralHelpers::StrCompareInsensitive(L"warn", verbosityMode))
		{
			ret = LoggerVerbose::LOGGER_WARN;
		}
		else
		{
			TraceHelpers::Logger::Instance().TraceConsoleDown("There was a problem with choosen verbosity. Defaulting it to INFO");
			ret = LoggerVerbose::LOGGER_INFO;
		}
	}

	return ret;
}


const ReportOutputList ConfigManager::GetReportList(const std::wstring &reportList)
{
	ReportOutputList ret;
	std::wstring workString(reportList);

	if (!reportList.empty() && GeneralHelpers::TrimSpaces(workString))
	{
		CommonTypes::WStringsContainer tokensVector;
		if ((GeneralHelpers::GetVectorByToken(workString, L',', tokensVector)) && (tokensVector.size() > 0))
		{
			for (auto it = tokensVector.begin(); it != tokensVector.end(); ++it)
			{
				const std::wstring &targetReportModuleStr(*it);
				ReportChannelID targetReportModule = ReportChannelID::REPORT_CHANNEL_NA;

				if (!targetReportModuleStr.empty())
				{
					targetReportModule = GetReportChannelID(targetReportModuleStr);

					//Adding report if it was valid
					if (targetReportModule != ReportChannelID::REPORT_CHANNEL_NA)
					{
						ret.push_back(targetReportModule);
					}
				}
			}
		}
	}

	return ret;
}


const ProcessesList ConfigManager::GetProcessList(const std::wstring &processesList)
{
	ProcessesList ret;
	std::wstring workString(processesList);

	if (!workString.empty() && GeneralHelpers::TrimSpaces(workString))
	{	
		CommonTypes::WStringsContainer tokensVector;
		if ((GeneralHelpers::GetVectorByToken(workString, L',', tokensVector)) && (tokensVector.size() > 0))
		{
			for (auto it = tokensVector.begin(); it != tokensVector.end(); ++it)
			{
				const std::wstring &targetProcessToMonitor(*it);

				if (!targetProcessToMonitor.empty())
				{
					ret.push_back(GeneralHelpers::WStrToStr(targetProcessToMonitor));
				}
			}
		}
	}

	return ret;
}

const HashAlgorithm ConfigManager::GetHashAlgorithm(const std::wstring &selectedHash)
{
	HashAlgorithm ret = HashAlgorithm::HASH_ALGORITHM_NA;

	if (!selectedHash.empty())
	{
		if (GeneralHelpers::StrCompareInsensitive(L"imphash", selectedHash))
		{
			ret = HashAlgorithm::HASH_ALGORITHM_IMPHASH;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"md5", selectedHash))
		{
			ret = HashAlgorithm::HASH_ALGORITHM_MD5;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"sha1", selectedHash))
		{
			ret = HashAlgorithm::HASH_ALGORITHM_SHA1;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"sha256", selectedHash))
		{
			ret = HashAlgorithm::HASH_ALGORITHM_SHA256;
		}
		else
		{
			TraceHelpers::Logger::Instance().TraceConsoleDown("There was a problem with given hash type parsing. Defaulting it to SHA1");
			ret = HashAlgorithm::HASH_ALGORITHM_SHA1;
		}
	}

	return ret;
}


bool ConfigManager::IsSystemDataAvailable()
{
	bool ret = true;

	//TODO: Future placeholder for system event DB

	return ret;
}

bool ConfigManager::ValidateConfigurationFile(std::wstring &configFilename)
{
	bool ret = false;

	if (!configFilename.empty() && GeneralHelpers::IsValidFile(configFilename))
	{
		ret = true;
	}

	return ret;
}

bool ConfigManager::IsValidHashAlgorithm(const unsigned int &hashAlgorithm)
{
	bool ret = false;

	if ((HashAlgorithm::HASH_ALGORITHM_SHA1 >= hashAlgorithm) && (hashAlgorithm < HashAlgorithm::HASH_ALGORITHM_NA))
	{
		ret = true;
	}

	return ret;
}

bool ConfigManager::GenerateTraceBackendNames(const std::wstring &traceBackendName, std::wstring &traceBackend32BitsName, std::wstring &traceBackend64BitsName)
{
	bool ret = false;

	if (!traceBackendName.empty())
	{
		traceBackend32BitsName.append(traceBackendName);
		traceBackend32BitsName.append(L"32.exe");

		traceBackend64BitsName.append(traceBackendName);
		traceBackend64BitsName.append(L"64.exe");

		if (!traceBackend32BitsName.empty() && !traceBackend64BitsName.empty())
		{
			ret = true;
		}		
	}

	return ret;
}

bool ConfigManager::GetParsedProxyConfData(const std::wstring &proxyStr, ProxyConfData &proxyData)
{
	bool ret = false;

	if (!proxyStr.empty())
	{
		CommonTypes::WStringsContainer tokensVector;
		if ((GeneralHelpers::GetVectorByToken(proxyStr, L':', tokensVector)) && (!tokensVector.empty()))
		{
			if (tokensVector.size() >= 0)
			{
				proxyData.proxy = tokensVector[0];
				ret = true;
			}

			if (tokensVector.size() >= 1)
			{
				proxyData.port = tokensVector[1];
			}

			if (tokensVector.size() >= 2)
			{
				proxyData.username = tokensVector[2];
			}

			if (tokensVector.size() >= 3)
			{
				proxyData.password = tokensVector[3];
			}
		}
	}

	return ret;
}

bool ConfigManager::IsNewCollectionServiceInstance()
{
	bool ret = false;

	if (m_collectionServiceName.compare(m_previousCollectionServiceName) != 0)
	{
		ret = true;
	}

	return ret;
}

bool ConfigManager::IsNewBackend32Instance()
{
	bool ret = false;

	if (m_backend32ServiceName.compare(m_previousBackend32ServiceName) != 0)
	{
		ret = true;
	}

	return ret;
}

bool ConfigManager::IsNewBackend64Instance()
{
	bool ret = false;

	if (m_backend64ServiceName.compare(m_previousBackend64ServiceName) != 0)
	{
		ret = true;
	}

	return ret;
}

bool ConfigManager::IsNewWorkingDirectory()
{
	bool ret = false;

	if (m_workingDirectory.compare(m_previousWorkingDirectory) != 0)
	{
		ret = true;
	}

	return ret;
}

//TODO: check security of new file
bool ConfigManager::GenerateDefaultConfigFile(std::wstring &newConfigFile)
{
	bool ret = false;

	std::wstring workingConfigFile;

	try
	{
		if (GeneralHelpers::IsValidDirectory(m_workingDirectory))
		{
			if (m_workingDirectory.back() != L'\\') m_workingDirectory.push_back(L'\\');

			workingConfigFile.assign(m_workingDirectory);
			workingConfigFile.append(SysmonXDefs::DEFAULT_CONFIG_FILE);

			std::wofstream configFile;
			configFile.open(workingConfigFile);
			configFile << SysmonXDefs::DEFAULT_SYSMON_CONFIG_FILE_CONTENT.c_str();
			configFile.close();

			newConfigFile.assign(workingConfigFile);

			ret = true;
		}
	}
	catch (...)
	{
		ret = false;
	}

	return ret;
}


bool ConfigManager::SyncRuntimeConfigData()
{
	bool ret = false;
	namespace data = cista::offset;
	SysmonXTypes::ConfigSerializedData workingRegistryConfigData = { 0 };

	//Getting system32 dir
	std::wstring workingSystemDirectory;
	if (!GeneralHelpers::GetWindowsSystemDirectory(workingSystemDirectory))
	{
		workingSystemDirectory.assign(CommonDefs::DEFAULT_SYSTEM32_DIRECTORY);
	}
	m_systemDirectory.assign(workingSystemDirectory);

	//Getting system root dir
	std::wstring workingSystemRoot;
	if (!GeneralHelpers::GetWindowsSystemRootDirectory(workingSystemRoot))
	{
		workingSystemRoot.assign(CommonDefs::DEFAULT_SYSTEM_ROOT_DIRECTORY);
	}
	m_systemRootDirectory.assign(workingSystemRoot);

	try
	{
		//Checking if previous configuration is already there
		CommonTypes::ByteContainer encryptedConfDataBytes;
		CommonTypes::ByteContainer decryptedConfDataBytes;

		if (RegistryHelpers::RegistryValueExists(HKEY_LOCAL_MACHINE, SysmonXDefs::SYSMONX_REGISTRY_KEY_LOCATION, SysmonXDefs::SYSMONX_REGISTRY_VALUE_LOCATION) &&
			RegistryHelpers::GetRegBinaryValue(HKEY_LOCAL_MACHINE, SysmonXDefs::SYSMONX_REGISTRY_KEY_LOCATION, SysmonXDefs::SYSMONX_REGISTRY_VALUE_LOCATION, encryptedConfDataBytes) &&
			!encryptedConfDataBytes.empty() &&
			GeneralHelpers::DPAPIDecrypt(encryptedConfDataBytes, SysmonXDefs::DEFAULT_SYSMONX_ENTROPY, decryptedConfDataBytes) &&
			!decryptedConfDataBytes.empty())
		{
			//-------- Going into not-first run scenario --------//
			SysmonXTypes::ConfigSerializedData *workingSerializedPtr = cista::offset::deserialize<SysmonXTypes::ConfigSerializedData>(decryptedConfDataBytes);

			if (workingSerializedPtr)
			{
				//Config was found in registry, so populate it 

				//Trace Backend 64 Bits Name
				if (m_backend64ServiceName.empty())
				{
					//backend64 keeps being the same as previous run
					m_backend64ServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->Backend64BitsName);
					m_previousBackend64ServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->PreviousBackend64BitsName);
				}
				else
				{
					//new backend64 was requested
					m_previousBackend64ServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->Backend64BitsName);
					workingSerializedPtr->PreviousBackend64BitsName = GeneralHelpers::GetSerializedVector(m_previousBackend64ServiceName);
					workingSerializedPtr->Backend64BitsName = GeneralHelpers::GetSerializedVector(m_backend64ServiceName);
				}

				//Trace Backend 32 Bits Name
				if (m_backend32ServiceName.empty())
				{
					//backend32 keeps being the same as previous run
					m_backend32ServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->Backend32BitsName);
					m_previousBackend32ServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->PreviousBackend32BitsName);
				}
				else
				{
					//new backend32 was requested
					m_previousBackend32ServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->Backend32BitsName);
					workingSerializedPtr->PreviousBackend32BitsName = GeneralHelpers::GetSerializedVector(m_previousBackend32ServiceName);
					workingSerializedPtr->Backend32BitsName = GeneralHelpers::GetSerializedVector(m_backend32ServiceName);
				}

				//Collection Service Names
				if (m_collectionServiceName.empty())
				{
					//collectionServiceName keeps being the same as previous run
					m_collectionServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->CollectionServiceName);
					m_previousCollectionServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->PreviousCollectionServiceName);
				}
				else
				{
					//new collectionServiceName was requested
					m_previousCollectionServiceName = GeneralHelpers::GetSerializedWString(workingSerializedPtr->CollectionServiceName);
					workingSerializedPtr->PreviousCollectionServiceName = GeneralHelpers::GetSerializedVector(m_previousCollectionServiceName);
					workingSerializedPtr->CollectionServiceName = GeneralHelpers::GetSerializedVector(m_collectionServiceName);
				}

				//Working Directory
				if (m_workingDirectory.empty())
				{
					//working directory keeps being the same as previous run
					m_workingDirectory = GeneralHelpers::GetSerializedWString(workingSerializedPtr->WorkingDirectory);
					m_previousWorkingDirectory = GeneralHelpers::GetSerializedWString(workingSerializedPtr->PreviousWorkingDirectory);
				}
				else
				{
					//new working directory was requested
					m_previousWorkingDirectory = GeneralHelpers::GetSerializedWString(workingSerializedPtr->WorkingDirectory);
					workingSerializedPtr->PreviousWorkingDirectory = GeneralHelpers::GetSerializedVector(m_previousWorkingDirectory);
					workingSerializedPtr->WorkingDirectory = GeneralHelpers::GetSerializedVector(m_workingDirectory);
				}


				//Configuration File
				if (!WasNewConfigFileProvided())
				{
					m_configurationFile = GeneralHelpers::GetSerializedWString(workingSerializedPtr->ConfigFile);
				}
				else
				{
					workingSerializedPtr->ConfigFile = GeneralHelpers::GetSerializedVector(m_configurationFile);
				}

				//checking config files
				std::wstring newConfigFile;
				if (!GeneralHelpers::IsValidFile(m_configurationFile))
				{
					if (GenerateDefaultConfigFile(newConfigFile) &&
						GeneralHelpers::IsValidFile(newConfigFile))
					{
						m_configurationFile = newConfigFile;
					}
					else
					{
						//cannot run without config file
						return ret;
					}
				}

				//Hash Algorithm to use				
				if (IsValidHashAlgorithm(workingSerializedPtr->HashAlgorithmToUse))
				{
					m_hashAlgorithmToUse = (HashAlgorithm) workingSerializedPtr->HashAlgorithmToUse;
				}
				else
				{
					m_hashAlgorithmToUse = HashAlgorithm::HASH_ALGORITHM_SHA1;
				}

				//Process to Track on Loading Modules
				if (m_processesToTrackOnLoadingModules.empty())
				{
					m_processesToTrackOnLoadingModules = GeneralHelpers::GetSerializedStringVector(workingSerializedPtr->ProcessesLoadingModules);
				}
				else
				{
					workingSerializedPtr->ProcessesLoadingModules = GeneralHelpers::GetSerializedStringVector(m_processesToTrackOnLoadingModules);
				}

				//Logging target file
				if (m_processesToTrackOnLoadingModules.empty())
				{
					m_processesToTrackOnLoadingModules = GeneralHelpers::GetSerializedStringVector(workingSerializedPtr->ProcessesLoadingModules);
				}
				else
				{
					workingSerializedPtr->ProcessesLoadingModules = GeneralHelpers::GetSerializedStringVector(m_processesToTrackOnLoadingModules);
				}

				//Process to Track on Network Connections
				if (m_processesToTrackOnNetworkConnections.empty())
				{
					m_processesToTrackOnNetworkConnections = GeneralHelpers::GetSerializedStringVector(workingSerializedPtr->ProcessesNetworkConnections);
				}
				else
				{
					workingSerializedPtr->ProcessesNetworkConnections = GeneralHelpers::GetSerializedStringVector(m_processesToTrackOnNetworkConnections);
				}

				//Revocation requested
				if (WasSignatureRevocationRequested())
				{
					workingSerializedPtr->ShouldCheckSignatureRevocation = m_wasSignatureRevocationCheckRequested;
				}
				else
				{
					m_wasSignatureRevocationCheckRequested = workingSerializedPtr->ShouldCheckSignatureRevocation;
				}
				
				//Worker Threads requested
				if (WasNumberOfWorkerThreadsRequested())
				{
					workingSerializedPtr->WorkerThreads = m_nrWorkerOrchThreads;
				}
				else
				{
					m_nrWorkerOrchThreads = workingSerializedPtr->WorkerThreads;
				}

				//Collection Service Logging Verbosity requested
				if (WasVerbosityRequested())
				{
					workingSerializedPtr->CollectionServiceLoggingVerbosity = m_loggingVerbosity;
				}
				else
				{
					m_loggingVerbosity = (LoggerVerbose) workingSerializedPtr->CollectionServiceLoggingVerbosity;
				}


				//Collection Service Logging Channels requested
				CommonTypes::UIntContainer workingLoggingOutputList;
				if (WasCollectionLoggingRequested())
				{
					for (auto it = m_loggingCollectionService.begin(); it != m_loggingCollectionService.end(); it++)
					{
						workingLoggingOutputList.push_back((unsigned int)*it);
					}
					workingSerializedPtr->CollectionServiceLoggingChannel = GeneralHelpers::GetSerializedVector(workingLoggingOutputList);
				}
				else
				{
					workingLoggingOutputList = GeneralHelpers::GetSerializedVector(workingSerializedPtr->CollectionServiceLoggingChannel);

					for (auto it = workingLoggingOutputList.begin(); it != workingLoggingOutputList.end(); it++)
					{
						m_loggingCollectionService.push_back((LoggerMode)*it);
					}
				}

				//Collection Service Report Channels requested
				CommonTypes::UIntContainer workingReportOutputList;
				if (WasReportOptionsRequested())
				{					
					for (auto it = m_reportOutputList.begin(); it != m_reportOutputList.end(); it++)
					{
						workingReportOutputList.push_back((unsigned int) *it);
					}
					workingSerializedPtr->CollectionServiceReportChannel = GeneralHelpers::GetSerializedVector(workingReportOutputList);
				}
				else
				{
					workingReportOutputList = GeneralHelpers::GetSerializedVector(workingSerializedPtr->CollectionServiceReportChannel);

					for (auto it = workingReportOutputList.begin(); it != workingReportOutputList.end(); it++)
					{
						m_reportOutputList.push_back((ReportChannelID)*it);
					}
				}

				//Serialized version
				workingSerializedPtr->Version = SysmonXDefs::SYSMONX_SERIALIZED_CONFIG_VERSION;

				//Serializing structure
				decryptedConfDataBytes.clear();
				decryptedConfDataBytes = cista::serialize(*workingSerializedPtr);
			}
		}
		else
		{
			//-------- Going into first run scenario --------//

			//trace backend name
			if (!IsTraceBackendDriverNameAvailable())
			{
				m_traceBackendName = SysmonXDefs::BACKEND_NAME;
			}
			GenerateTraceBackendNames(m_traceBackendName, m_backend32ServiceName, m_backend64ServiceName);

			//trace backend image file 32 bits
			if (!IsBackend32ServiceNameAvailable())
			{
				m_backend32ServiceName = SysmonXDefs::SYSMON_TRACE_BACKEND_32BITS;				
			}
			m_previousBackend32ServiceName = m_backend32ServiceName;

			//trace backend image file 64 bits
			if (!IsBackend64ServiceNameAvailable())
			{
				m_backend64ServiceName = SysmonXDefs::SYSMON_TRACE_BACKEND_64BITS;
				
			}
			m_previousBackend64ServiceName = m_backend64ServiceName;

			//collecting service name
			if (!IsCollectionServiceNameAvailable())
			{
				m_collectionServiceName = SysmonXDefs::SERVICE_NAME;				
			}
			m_previousCollectionServiceName = m_collectionServiceName;

			//working directory
			if (!IsWorkingDirectoryAvailable())
			{
				std::wstring tempContainer;
				if (GeneralHelpers::GetNewSecureSysmonXDirectory(tempContainer) &&
					GeneralHelpers::IsValidDirectory(tempContainer))
				{
					m_workingDirectory.assign(tempContainer);
				}
				else
				{
					m_workingDirectory.assign(workingSystemRoot);
				}
			}		
			m_previousWorkingDirectory.assign(m_workingDirectory);

			//configuration file
			std::wstring newConfigFile;
			if (!IsConfigurationFileAvailable())
			{
				if (GenerateDefaultConfigFile(newConfigFile) &&
					GeneralHelpers::IsValidFile(newConfigFile))
				{
					m_configurationFile = newConfigFile;
				}
				else
				{
					//cannot run wo config file
					return ret;
				}
			}

			//collecting service log file
			if (!IsCollectionServiceLogfileAvailable())
			{
				m_collectionServiceLogfile = SysmonXDefs::DEFAULT_LOG_FILE;
			}

			//hash algorithm to use
			if (GetHashAlgorithmToUse() == HashAlgorithm::HASH_ALGORITHM_NA)
			{
				m_hashAlgorithmToUse = HashAlgorithm::HASH_ALGORITHM_SHA1;
			}

			//number of worker threads
			if (!WasNumberOfWorkerThreadsRequested())
			{
				m_nrWorkerOrchThreads = SysmonXDefs::SYSMONX_DEFAULT_WORKER_THREADS;
			}

			//verbosity
			if (!WasVerbosityRequested())
			{
				m_loggingVerbosity = LoggerVerbose::LOGGER_TRACE;
			}

			//Adding default report channel in case list is empty
			if (!WasReportOptionsRequested() || m_reportOutputList.empty())
			{
				m_reportOutputList.push_back(ReportChannelID::REPORT_CHANNEL_EVENTLOG);
				m_reportOutputList.push_back(ReportChannelID::REPORT_CHANNEL_DEBUG_EVENTS);
			}			

			//Config couldn't not be parsed or it was not available. Either the case, overwritting it now
			workingRegistryConfigData.Backend32BitsName = GeneralHelpers::GetSerializedVector(m_backend32ServiceName);
			workingRegistryConfigData.Backend64BitsName = GeneralHelpers::GetSerializedVector(m_backend64ServiceName);
			workingRegistryConfigData.CollectionServiceName = GeneralHelpers::GetSerializedVector(m_collectionServiceName);
			workingRegistryConfigData.PreviousCollectionServiceName = GeneralHelpers::GetSerializedVector(m_previousCollectionServiceName);
			workingRegistryConfigData.PreviousBackend32BitsName = GeneralHelpers::GetSerializedVector(m_previousBackend32ServiceName);
			workingRegistryConfigData.PreviousBackend64BitsName = GeneralHelpers::GetSerializedVector(m_previousBackend64ServiceName);
			workingRegistryConfigData.ConfigFile = GeneralHelpers::GetSerializedVector(m_configurationFile);
			workingRegistryConfigData.WorkingDirectory = GeneralHelpers::GetSerializedVector(m_workingDirectory);
			workingRegistryConfigData.PreviousWorkingDirectory = GeneralHelpers::GetSerializedVector(m_workingDirectory);
			workingRegistryConfigData.CollectionServiceLogfile = GeneralHelpers::GetSerializedVector(m_collectionServiceLogfile);
			workingRegistryConfigData.HashAlgorithmToUse = m_hashAlgorithmToUse;
			workingRegistryConfigData.ShouldCheckSignatureRevocation = m_wasSignatureRevocationCheckRequested;
			workingRegistryConfigData.CollectionServiceLoggingVerbosity = m_loggingVerbosity;
			workingRegistryConfigData.Version = SysmonXDefs::SYSMONX_SERIALIZED_CONFIG_VERSION;

			//track of loading modules
			if (!m_processesToTrackOnLoadingModules.empty())
			{
				workingRegistryConfigData.ProcessesLoadingModules = GeneralHelpers::GetSerializedStringVector(m_processesToTrackOnLoadingModules);
			}

			//track of network connection processes
			if (!m_processesToTrackOnNetworkConnections.empty())
			{
				workingRegistryConfigData.ProcessesNetworkConnections = GeneralHelpers::GetSerializedStringVector(m_processesToTrackOnNetworkConnections);
			}

			//report channels
			if (!m_reportOutputList.empty())
			{
				CommonTypes::UIntContainer workingOutputList;
				for (auto it = m_reportOutputList.begin(); it != m_reportOutputList.end(); it++)
				{
					workingOutputList.push_back((unsigned int)*it);
				}
				workingRegistryConfigData.CollectionServiceReportChannel = GeneralHelpers::GetSerializedVector(workingOutputList);
			}

			//logging channels
			if (WasCollectionLoggingRequested())
			{
				CommonTypes::UIntContainer workingOutputList;
				for (auto it = m_loggingCollectionService.begin(); it != m_loggingCollectionService.end(); it++)
				{
					workingOutputList.push_back((unsigned int)*it);
				}
				workingRegistryConfigData.CollectionServiceLoggingChannel = GeneralHelpers::GetSerializedVector(workingOutputList);
			}

			//Serializing structure
			decryptedConfDataBytes.clear();
			decryptedConfDataBytes = cista::serialize(workingRegistryConfigData);
		}

		//Checking if serialized data are available
		if (!decryptedConfDataBytes.empty() &&
			GeneralHelpers::DPAPIEncrypt(decryptedConfDataBytes, SysmonXDefs::DEFAULT_SYSMONX_ENTROPY, encryptedConfDataBytes) &&
			!encryptedConfDataBytes.empty() &&
			RegistryHelpers::SetRegBinaryValue(HKEY_LOCAL_MACHINE, SysmonXDefs::SYSMONX_REGISTRY_KEY_LOCATION, SysmonXDefs::SYSMONX_REGISTRY_VALUE_LOCATION, encryptedConfDataBytes))
		{
			ret = true;
		}
	}
	catch (...)
	{
		ret = false;
	}

	return ret;
}

bool ConfigManager::GetRegistryConfigData(CommonTypes::ByteContainer& rawBytes, SysmonXTypes::ConfigSerializedData* data)
{
	bool ret = false;

	//Checking if previous configuration is already there
	CommonTypes::ByteContainer encryptedConfDataBytes;
	
	try 
	{
		if (RegistryHelpers::RegistryValueExists(HKEY_LOCAL_MACHINE, SysmonXDefs::SYSMONX_REGISTRY_KEY_LOCATION, SysmonXDefs::SYSMONX_REGISTRY_VALUE_LOCATION) &&
			RegistryHelpers::GetRegBinaryValue(HKEY_LOCAL_MACHINE, SysmonXDefs::SYSMONX_REGISTRY_KEY_LOCATION, SysmonXDefs::SYSMONX_REGISTRY_VALUE_LOCATION, encryptedConfDataBytes) &&
			!encryptedConfDataBytes.empty() &&
			GeneralHelpers::DPAPIDecrypt(encryptedConfDataBytes, SysmonXDefs::DEFAULT_SYSMONX_ENTROPY, rawBytes) &&
			!rawBytes.empty())
		{
			data = cista::offset::deserialize<SysmonXTypes::ConfigSerializedData>(rawBytes);

			if (data)
			{
				ret = true;
			}
		}
	}
	catch (...)
	{
		ret = false;
	}

	return ret;
}

bool ConfigManager::GetBackendFiles(std::wstring &backend32BitsFile, std::wstring &backend64BitsFile)
{
	bool ret = false;

	if (!m_workingDirectory.empty() &&
		!m_backend32ServiceName.empty() &&
		!m_backend64ServiceName.empty())
	{
		std::wstring workingDir(m_workingDirectory);
		if (workingDir.back() != L'\\') workingDir.push_back(L'\\');

		backend32BitsFile.clear();
		backend64BitsFile.clear();

		backend32BitsFile.append(workingDir);
		backend32BitsFile.append(m_backend32ServiceName);

		backend64BitsFile.append(workingDir);
		backend64BitsFile.append(m_backend64ServiceName);

		if (!backend32BitsFile.empty() && !backend64BitsFile.empty())
		{
			ret = true;
		}		
	}

	return ret;
}

bool ConfigManager::GetFullPathConfigFile(std::wstring &configFile)
{
	bool ret = false;

	if (GeneralHelpers::IsValidDirectory(m_workingDirectory))
	{
		std::wstring workingDir(m_workingDirectory);
		std::wstring workingConfigFile;
		if (workingDir.back() != L'\\') workingDir.push_back(L'\\');

		workingConfigFile.append(workingDir);
		workingConfigFile.append(m_configurationFile);

		if (GeneralHelpers::IsValidFile(workingConfigFile))
		{
			configFile.assign(workingConfigFile);
			ret = true;
		}
	}

	return ret;
}

bool ConfigManager::GetFullPathBackendConfigFile(std::wstring &configFile)
{
	bool ret = false;

	if (GeneralHelpers::IsValidDirectory(m_workingDirectory))
	{
		std::wstring workingDir(m_workingDirectory);
		std::wstring workingConfigFile;
		if (workingDir.back() != L'\\') workingDir.push_back(L'\\');

		std::wstring baseFileName;
		if (GeneralHelpers::GetBaseFileName(m_configurationFile, baseFileName))
		{
			workingConfigFile.append(workingDir);
			workingConfigFile.append(L"trace_backend_");
			workingConfigFile.append(baseFileName);

			if (!workingConfigFile.empty())
			{
				configFile.assign(workingConfigFile);
				ret = true;
			}
		}
	}

	return ret;
}

bool ConfigManager::GetFullPathCollectionServiceFile(std::wstring &collectionServiceFile)
{
	bool ret = false;

	if (IsWorkingDirectoryAvailable() && IsCollectionServiceNameAvailable())
	{
		collectionServiceFile.clear();
		std::wstring workingFile(GetSysmonXWorkingDirectory());
		if (workingFile.back() != L'\\') workingFile.push_back(L'\\');
		workingFile.append(GetCollectionServiceName());
		workingFile.append(L".exe");

		collectionServiceFile.append(workingFile);

		if (!collectionServiceFile.empty())
		{
			ret = true;
		}		
	}	

	return ret;
}


const ReportChannelID ConfigManager::GetReportChannelID(const std::wstring &reportChannel)
{
	ReportChannelID ret = ReportChannelID::REPORT_CHANNEL_NA;

	if (!reportChannel.empty())
	{
		if (GeneralHelpers::StrCompareInsensitive(L"debugevents", reportChannel))
		{
			ret = ReportChannelID::REPORT_CHANNEL_DEBUG_EVENTS;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"etw", reportChannel))
		{
			ret = ReportChannelID::REPORT_CHANNEL_ETW;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"eventlog", reportChannel))
		{
			ret = ReportChannelID::REPORT_CHANNEL_EVENTLOG;
		}
		else if (GeneralHelpers::StrCompareInsensitive(L"file", reportChannel))
		{
			ret = ReportChannelID::REPORT_CHANNEL_LOGFILE;
		}
		else
		{
			TraceHelpers::Logger::Instance().Error(L"ConfigManager::GetReportChannel() - Given channel {} is not supported", reportChannel);
		}
	}

	return ret;
}
