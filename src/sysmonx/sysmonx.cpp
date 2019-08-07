#include "common.h"


void ShowHelp()
{
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();

	logger.TraceConsole("\n");
	logger.TraceConsole("  #####                                    #     # ");
	logger.TraceConsole(" #     # #   #  ####  #    #  ####  #    #  #   #  ");
	logger.TraceConsole(" #        # #  #      ##  ## #    # ##   #   # #   ");
	logger.TraceConsole("  #####    #    ####  # ## # #    # # #  #    #    ");
	logger.TraceConsole("       #   #        # #    # #    # #  # #   # #   ");
	logger.TraceConsole(" #     #   #   #    # #    # #    # #   ##  #   #  ");
	logger.TraceConsole("  #####    #    ####  #    #  ####  #    # #     # ");
	logger.TraceConsole("                                                   ");
	logger.TraceConsole("        OpenSource Alternative to Sysmon        ");
	logger.TraceConsole("\nSysmonX Version: %s", SysmonXDefs::SYSMONX_VERSION.c_str());

	logger.TraceConsole("\nSysmonX Options:");
	logger.TraceConsole(" -c <Configuration File> (Update configuration to installed trace backend. Optionally take a configuration file as path.)");
	logger.TraceConsole(" -d <Trace Backend Name> (The name of the trace backend image can be specified.)");
	logger.TraceConsole(" -z <Collection Service Name> (The name of the service image and service name can be specified.)");
	logger.TraceConsole(" -h <sha1|md5|sha256|imphash> (The hash algorithm to be used by the trace backends can be specified. Default is SHA1.)");
	logger.TraceConsole(" -l <Processes To Track> (It forces the trace backends to load the loading of modules. A list of process to track can be feed optionally.)");
	logger.TraceConsole(" -n <Processes To Track> (It forces the trace backends to log network connections. A list of process to track can be feed optionally.)");
	logger.TraceConsole(" -r (It forces the trace backend to check signature revocation.)");
	logger.TraceConsole(" -s (It prints latest trace backends configuration schema definitions.)");
	logger.TraceConsole(" -i (It installs trace backends and collection service. This will enable ETW trace backend and it will grab and install Sysmon as trace backend.)");
	logger.TraceConsole(" –accepteula (It accepts trace backend EULA. This needs to be passed together with -i.)");
	logger.TraceConsole(" -u <force> (It uninstalls collection service and trace backends.)");
	logger.TraceConsole(" -m (It installs collection service manifests.)");
	logger.TraceConsole(" -t <web|filesystem> <Directory with Installer Files from where to grab target backends in case filesystem was choosed>");
	logger.TraceConsole("    (It installs trace backends and collection service. This will enable ETW trace collection and it will grab and install trace backend (Sysmon for now) from target location.)");
	logger.TraceConsole("    (If web is choosed, Sysmon Installers will be downloaded from the Microsoft Sysinternals website. )");
	logger.TraceConsole("    (If filesystem is choosed, Sysmon Installers will be grabbed from the target filesystem location. Expected filename is Sysmon.zip for now.)");
	logger.TraceConsole(" -p <proxy:port:username:password> (When web is required to download the Sysmon as trace backend, this option might help on specifying a proxy for outgoing https connection.)");
	logger.TraceConsole(" -x (It prints collection service and enabled trace backends information.)");
	logger.TraceConsole(" -e <DebugEvents|ETW|File|Off> <Target Logfile> (Collection Service Logging Options.)");
	logger.TraceConsole("    (Off by default for collection service)");	
	logger.TraceConsole("    (Target Logfile when File option is required as output. Default is sysmonx.log)");
	logger.TraceConsole(" -f <Console|File|Off> <Target Logfile> (Management App Logging Options.)");
	logger.TraceConsole("    (Console by default for management application)");
	logger.TraceConsole("    (Target Logfile when File option is required as output. Default is sysmonx.log)");
	logger.TraceConsole(" -v <TRACE|DEBUG|INFO|WARNING|ERROR|CRITICAL> (It sets the logging verbosity mode.)");
	logger.TraceConsole(" -o <DebugEvents|File|EventLog|ETW|Off> <Target Logfile> (Report Output Options for security events.)");
	logger.TraceConsole("    (Target Logfile when File option is required as output)");
	logger.TraceConsole("    (Target Event Log Location. Default is on \"Applications and Services/Logs/SysmonX/Operational\")");
	logger.TraceConsole("    If sysmon is used as tracing backend, Sysmon events will keep appearing at \"Applications and Services Logs/Microsoft/Windows/Sysmon/Operational\")");
	logger.TraceConsole(" -g <number of worker event processing threads> (It defines the number of worker processing threads. Default to 4.)");
	logger.TraceConsole(" -? (Display help information)");

	logger.TraceConsole("\nUsage Example:");
	logger.TraceConsole(" -? (for help)");
	logger.TraceConsole(" -i (to install the collection service and target backends)");
	logger.TraceConsole(" -u (to uninstall the collection service and target backends)");	
}

int wmain(int argc, wchar_t *argv[])
{
	int ret = EXIT_SUCCESS;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	//Checking if process is running as Administrator and DEBUG token privileges can be enabled
	if (!GeneralHelpers::IsRunningAsAdmin() || !GeneralHelpers::EnableTokenPrivilege(SE_DEBUG_NAME))
	{
		logger.TraceConsoleDown("This process should be run with administrator privileges. Showing help and quitting now.");
		ShowHelp();
		return EXIT_FAILURE;
	}

	//Checking if min number of arguments were requested
	if (argc < SysmonXDefs::SYSMONX_MIN_NUMBER_OF_ARGS)
	{
		ShowHelp();
		return EXIT_FAILURE;
	}

	//Initializing configuration
	if (!config.Initialize(argc, argv))
	{
		logger.TraceConsoleDown("There was a problem initializing working configuration. Quitting now.");
		return EXIT_FAILURE;
	}

	//Help request takes priority here
	if (config.WasHelpRequested())
	{
		logger.TraceUp("Help was requested. Showing help and quitting now.");
		ShowHelp();
		return EXIT_FAILURE;
	}

	//Setting up mgmt app and collection service prerequisites
	//This includes setting up logging and reporting configuration, along with working directory data
	if (!SysmonXAppFlows::SetupWorkEnvironment())
	{
		logger.Error("There was a problem setting up mgmt app and and collection service running conditions.");
		return EXIT_FAILURE;
	}

	logger.Info("Welcome to SysmonX Version: {}", SysmonXDefs::SYSMONX_VERSION.c_str());

	//Same binary supports two running modes (mgmt app mode and service mode)
	//Checking if main thread needs to run as a service 
	if (config.IsServiceMode())
	{
		/*
		//====================== MJO REMOVE THIS!! ======================//
		if (config.WereStandaloneActionsRequested())
		{
			CollectorService &collectorService = CollectorService::Instance();
			collectorService.RunFakeLogic();
			return EXIT_FAILURE;
		}
		*/
		
		//Running as a service
		if (!SysmonXAppFlows::RunCollectionService())
		{
			logger.Error("There was a problem running collector service.");
			return EXIT_FAILURE;
		}		
	}
	else
	{
		//standalone mgmt appt mode
		logger.Info("Standalone management app mode.");

		//standalone management actions happens here
		if (config.WereStandaloneActionsRequested())
		{
			logger.Info("About to process special requests.");

			//New config was requested	
			if (config.WasNewConfigFileProvided())
			{
				logger.Info("Processing New configuration ");
				if (!SysmonXAppFlows::UpdateConfiguration())
				{
					logger.Error("There was a problem updating configurationd data.");
					return EXIT_FAILURE;
				}
			}
			
			//Dumping current configuration			
			if (config.WasConfigurationDumpRequested())
			{
				logger.Info("Processing Configuration Request Dump");
				if (!SysmonXAppFlows::DumpConfigurationData())
				{
					logger.Error("There was a problem dumping configurationd data.");
					return EXIT_FAILURE;
				}
			}
			
			//Dumping Configuration Schema			
			if (config.WasDumpOfSchemaRequested())
			{
				logger.Info("Processing Schema Information Dump");
				if (!SysmonXAppFlows::DumpSchemaData())
				{
					logger.Error("There was a problem dumping schema data.");
					return EXIT_FAILURE;
				}
			}

			//Install collection service manifests			
			if (config.WasInstallManifestRequested())
			{
				logger.Info("Installing of Collection Service Manifests");
				if (!SysmonXAppFlows::InstallCollectionServiceManifests())
				{
					logger.Error("There was a problem installing collection service manifests.");
					return EXIT_FAILURE;
				}
			}
		}
		//Collection service related work below
		//Install request
		else if (config.WasInstallRequested())
		{
			logger.Info("About to install collection service.");
			if (SysmonXAppFlows::InstallCollectionService())
			{
				logger.Info("Installation of collection service was successful.");
			}
			else
			{
				logger.Error("There was a problem installing collection service. Quitting now.");
				return EXIT_FAILURE;
			}
		}
		//Uninstall request
		else if (config.WasUninstallRequested())
		{
			logger.Info("About to uninstall collection service.");
			if (SysmonXAppFlows::UninstallCollectionService())
			{
				logger.Info("Uninstallation of collection service was successful. Quitting now.");
			}
			else
			{
				logger.Error("There was a problem uninstalling collection service. Quitting now.");
				return EXIT_FAILURE;
			}
		}
		//Config update request
		else
		{
			logger.Info("About to update collection service configuration.");
			if (SysmonXAppFlows::RefreshCollectionServiceDataConfiguration())
			{
				logger.Info("Updating of collection service was successful.");
			}
			else
			{
				logger.Error("You need to provide an action to update the collection service data or to install/uninstall it. Quitting now.");
				return EXIT_FAILURE;
			}
		}

		//Checking if service preconditions are there, and then made sure that service is running
		if (config.IsServiceDataAvailable() && !config.WasUninstallRequested())
		{
			logger.Trace("Checking if collection service needs to be started.");

			//and making sure that service is still running
			if (ServiceHelpers::IsServiceStopped(config.GetCollectionServiceName()))
			{
				logger.Trace("Collection service requires restart.");
				//Starting the service					
				if (ServiceHelpers::StartTargetService(config.GetCollectionServiceName()))
				{
					logger.Trace("Collection service was succesfully started. Quitting now.");
				}
				else
				{
					logger.Error("There was a problem starting collector service. Service is currently stopped and cannot be started.");
					return EXIT_FAILURE;
				}					
			}
			else
			{
				logger.Trace("Collection service does not require restart. Quitting now.");
			}
		}
	}

    return ret;
}

