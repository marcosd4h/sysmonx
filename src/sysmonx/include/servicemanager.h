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

	//TODO: mjo delete this
	void RunFakeLogic();

	//Control functions
	bool RunService();
	bool StopService();

	//Public Methods needed by static SCM-reachable members
	SERVICE_STATUS ServiceStatus;
	HANDLE ServiceStopEvent;
	SERVICE_STATUS_HANDLE ServiceStatusHandle;
	
private:

	CollectorService()
	{
		ServiceStopEvent = INVALID_HANDLE_VALUE;
		ServiceStatusHandle = NULL;
		memset(&ServiceStatus, 0, sizeof(ServiceStatus));
	};

	~CollectorService()
	{
		if (ServiceStopEvent)
		{
			CloseHandle(ServiceStopEvent);
			ServiceStopEvent = NULL;
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
