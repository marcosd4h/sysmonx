#include "common.h"

bool CollectorService::Continue()
{
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();

	logger.TraceUp("About to run CollectorService::Continue()");

	try
	{
		logger.TraceUp("CollectorService::Continue - Continue is not yet supported");
		ret = true;

	}
	catch (...)
	{
		logger.TraceDown("CollectorService::Continue - There was an error continuing the service. Setting state to original one.");
	}

	return ret;
}


bool CollectorService::Pause()
{
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();

	logger.TraceUp("About to run CollectorService::Pause()");

	try
	{
		logger.TraceUp("CollectorService::Pause - Paused is not yet supported");
		ret = true;

	}
	catch (...)
	{
		logger.TraceDown("CollectorService::Pause - There was an error pausing the service. Setting state to original one.");
	}

	return ret;
}


bool CollectorService::Shutdown()
{
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	CollectorService &service = CollectorService::Instance();

	DWORD originalState = service.ServiceStatus.dwCurrentState;

	logger.TraceUp("About to run CollectorService::Shutdown()");

	try
	{
		if (UpdateServiceStatus(SERVICE_STOP_PENDING))
		{
			SetEvent(service.ServiceStopEvent);

			if (UpdateServiceStatus(SERVICE_STOPPED))
			{
				ret = true;
			}
		}

		if (!ret)
		{
			logger.TraceUp("CollectorService::Shutdown - There was an error stopping the service. Setting state to original one");
			UpdateServiceStatus(originalState);
		}
	}
	catch (...)
	{
		logger.TraceDown("CollectorService::Shutdown - There was an error stopping the service. Setting state to original one.");
		UpdateServiceStatus(originalState);
	}

	return ret;
}


bool CollectorService::Run(DWORD dwArgc, PWSTR *pszArgv)
{
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	CollectorService &service = CollectorService::Instance();

	logger.TraceUp("About to run CollectorService::Run()");

	try
	{
		if (pszArgv)
		{
			if (UpdateServiceStatus(SERVICE_START_PENDING))
			{
				//Service is registered, now initialize stop event
				service.ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				if (service.ServiceStopEvent)
				{
					HANDLE hThread = CreateThread(NULL, 0, CollectorService::WorkerThread, NULL, 0, NULL);
					if ((hThread != INVALID_HANDLE_VALUE) && UpdateServiceStatus(SERVICE_RUNNING))
					{
						ret = true;
					}
				}
			}
		}

		if (!ret)
		{
			logger.TraceUp("CollectorService::Run - There was an error starting the service. Shutting it down.");
			UpdateServiceStatus(SERVICE_STOPPED);
		}
	}
	catch (...)
	{
		logger.TraceDown("CollectorService::Run - There was an error starting the service. Shutting it down.");
		UpdateServiceStatus(SERVICE_STOPPED);
	}

	return ret;
}

bool WINAPI CollectorService::UpdateServiceStatus(DWORD updateState, DWORD exitCode, DWORD waitTime)
{
	bool ret = false;
	DWORD acceptedCtrls = 0;
	CollectorService &service = CollectorService::Instance();

	acceptedCtrls |= SERVICE_ACCEPT_STOP;
	acceptedCtrls |= SERVICE_ACCEPT_SHUTDOWN;
	//acceptedCtrls |= SERVICE_ACCEPT_PAUSE_CONTINUE;

	Sleep(waitTime);

	service.ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	service.ServiceStatus.dwControlsAccepted = acceptedCtrls;
	service.ServiceStatus.dwCurrentState = updateState;
	service.ServiceStatus.dwWin32ExitCode = exitCode;
	service.ServiceStatus.dwServiceSpecificExitCode = 0;
	service.ServiceStatus.dwCheckPoint = 0;
	service.ServiceStatus.dwWaitHint = 0;

	if (SetServiceStatus(service.ServiceStatusHandle, &service.ServiceStatus))
	{
		ret = true;
	}

	return ret;
}

void WINAPI CollectorService::ServiceControlHandler(DWORD ctrlCode)
{
	CollectorService &service = CollectorService::Instance();

	switch (ctrlCode)
	{
		case SERVICE_CONTROL_PAUSE: 
			service.Pause();
			break;
		case SERVICE_CONTROL_CONTINUE: 
			service.Continue();
			break;
		case SERVICE_CONTROL_STOP: 
		case SERVICE_CONTROL_SHUTDOWN:
			service.Shutdown();
			break;
		case SERVICE_CONTROL_INTERROGATE:
		default:
			break;
	}
}

void WINAPI CollectorService::ServiceMain(DWORD argc, LPWSTR *argv)
{
	DWORD status = E_FAIL;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	CollectorService &service = CollectorService::Instance();
	ConfigManager &config = ConfigManager::Instance();

	std::wstring workingName;
	logger.TraceUp("About to run CollectorService::ServiceMain()");

	//setting handler
	service.ServiceStatusHandle = RegisterServiceCtrlHandler(workingName.c_str(), ServiceControlHandler);
	if (service.ServiceStatusHandle != NULL)
	{
		if (!service.Run(argc, argv))
		{
			logger.TraceDown("CollectorService::ServiceMain - There was a problem starting sysmonx collector service");
		}
	}
}



bool CollectorService::RunService()
{
	bool ret = false;
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();
	ConfigManager &config = ConfigManager::Instance();

	logger.TraceUp("About to run CollectorService::RunService()");

	SERVICE_TABLE_ENTRY sTable[] =
	{
		{ (wchar_t *)config.GetCollectionServiceName().c_str(), ServiceMain },
		{ NULL, NULL }
	};

	//connects with SCM
	if (StartServiceCtrlDispatcher(sTable))
	{
		ret = true;
		logger.TraceUp("CollectorService::RunService - ServiceMain call was succesfully dispatched");
	}
	else
	{
		DWORD err = GetLastError();
		logger.TraceDown("CollectorService::RunService - There was a problem connecting with SCM. Error is 0x%x", err);
	}

	return ret;
}

bool CollectorService::StopService()
{
	bool ret = false;

	if (Shutdown())
	{
		ret = true;
	}

	return ret;
}

DWORD WINAPI CollectorService::WorkerThread(LPVOID lpParam)
{
	DWORD ret = ERROR_SUCCESS;
	CollectorService &collectorService = CollectorService::Instance();
	TraceHelpers::Logger &logger = TraceHelpers::Logger::Instance();

	logger.TraceUp("About to run CollectorService::WorkerThread()");

	if (collectorService.ServiceStopEvent != INVALID_HANDLE_VALUE)
	{
		//Initializing Service Work Environment
		logger.TraceUp("CollectorService::WorkerThread - About to setup working environment");

#ifdef SERVICE_DEBUG
		while (WaitForSingleObject(collectorService.ServiceStopEvent, 0) != WAIT_OBJECT_0)
		{
			Sleep(SysmonXDefs::SYSMONX_DEFAULT_SERVICE_OP_TIMEOUT_IN_MS);
		}
#else
		if (SysmonXServiceFlows::IsServiceWorkEnvironmentReady())
		{
			logger.TraceUp("CollectorService::WorkerThread - Working environment is ready, about to enable processing");
			if (SysmonXServiceFlows::EnableServiceProcessing())
			{
				//Checking periodically if we need to stop
				while (WaitForSingleObject(collectorService.ServiceStopEvent, 0) != WAIT_OBJECT_0)
				{
					Sleep(SysmonXDefs::SYSMONX_DEFAULT_SERVICE_OP_TIMEOUT_IN_MS);
				}

				logger.TraceUp("CollectorService::WorkerThread - Processing requested to be stopped. About to Disable processing");
				if (SysmonXServiceFlows::DisableServiceProcessing())
				{
					logger.TraceUp("CollectorService::WorkerThread - Processing is stopped. Quitting service now");
				}
				else
				{
					logger.TraceDown("CollectorService::WorkerThread - There was a problem stopping service processing");
				}

				ret = ERROR_SUCCESS;
			}
			else
			{
				logger.TraceDown("CollectorService::WorkerThread - There was a problem enabling service processing");
				SysmonXServiceFlows::DisableServiceProcessing();
				ret = ERROR_NO_WORK_DONE;
			}
		}
		else
		{
			logger.TraceDown("CollectorService::WorkerThread - There was a problem enabling service work environment");
			ret = ERROR_NOT_READY;
		}
#endif // SERVICE_DEBUG
	}
	else
	{
		ret = ERROR_BAD_ENVIRONMENT;
	}

	Sleep(SysmonXDefs::SYSMONX_DEFAULT_SERVICE_OP_TIMEOUT_IN_MS);

	return ret;
}


void CollectorService::RunFakeLogic()
{	
	CollectorService &collectorService = CollectorService::Instance();

	//Initializing Collection Workers
	if (SysmonXServiceFlows::IsServiceWorkEnvironmentReady())
	{
		if (SysmonXServiceFlows::EnableServiceProcessing())
		{
			system("pause");

			if (SysmonXServiceFlows::DisableServiceProcessing())
			{
				int a = 0;
				a++;
			}
		}
	}	
}