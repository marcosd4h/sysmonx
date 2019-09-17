#pragma once

#include "common.h"

class CollectorService
{
public:

	static CollectorService& Instance()
	{
		static CollectorService instance;
		return instance;
	}

	//Helpers functions
	void RunDebugLogic();
	bool PlaceMonitoringHook();

	//Control functions
	bool RunService();
	bool StopService();

	//Public Methods needed by static SCM-reachable members
	SERVICE_STATUS ServiceStatus = { 0 };
	HANDLE ServiceStopEvent = INVALID_HANDLE_VALUE;
	HANDLE ServiceUnitializedEvent = INVALID_HANDLE_VALUE;
	SERVICE_STATUS_HANDLE ServiceStatusHandle = NULL;
	std::atomic<bool> IsShuttingDown = false;
	HKEY ConfigDataKey = NULL;

private:

	CollectorService() = default;

	~CollectorService()
	{
		IsShuttingDown = true;

		if (ServiceStopEvent != INVALID_HANDLE_VALUE)
		{
			CloseHandle(ServiceStopEvent);
			ServiceStopEvent = NULL;
		}

		if (ServiceUnitializedEvent != INVALID_HANDLE_VALUE)
		{
			CloseHandle(ServiceUnitializedEvent);
			ServiceUnitializedEvent = NULL;
		}

		if (ConfigDataKey != NULL)
		{
			RegistryHelpers::CloseKey(ConfigDataKey);
		}
	}

	//Forcing singleton here
	CollectorService(const CollectorService&) = delete;
	CollectorService(CollectorService&&) = delete;
	CollectorService& operator=(const CollectorService&) = delete;
	CollectorService& operator=(CollectorService&&) = delete;

	//Helpers functions
	bool WINAPI UpdateServiceStatus(DWORD updateState, DWORD exitCode = NO_ERROR, DWORD waitTime = 0);

	// Static SCM functions
	static void WINAPI ServiceMain(DWORD argc, LPWSTR *argv);
	static void WINAPI ServiceControlHandler(DWORD ctrlCode);
	static DWORD WINAPI WorkerThread(LPVOID lpParam);

	//SCM Callbacks
	bool Run(DWORD dwArgc, PWSTR *pszArgv);
	bool Pause();
	bool Continue();
	bool Shutdown();
};
