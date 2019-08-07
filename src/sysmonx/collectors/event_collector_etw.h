#pragma once

#include "common.h"

class EventCollectorETW : public EventCollectorBase
{
public:
	bool Initialize();
	bool UpdatePolicy(const SysmonXTypes::RulesContainer &reportData);
	bool StartEventsCollection();
	bool StopEventsCollection();
	bool IsInitialized() { return m_isInitialized; };
	bool IsCollectionRunning() { return m_isEnabled; };

	//Lifecycle management
	EventCollectorETW() :
		EventCollectorBase(L"Event Collector ETW Technology", SysmonXTypes::EventCollectorTechID::EVENT_COLLECTOR_TECH_ETW),
		m_isInitialized(false),
		m_isEnabled(false),
		m_isKernelTraceStarted(false),
		m_isUserTraceStarted(false),
		m_eventsProcessor(MatchingEngine::Instance()),
		m_logger(TraceHelpers::Logger::Instance()),
		m_config(ConfigManager::Instance()),
		m_kernelTrace(nullptr),
		m_userTrace(nullptr),		
		m_etwKernelTraceThread(nullptr), 
		m_etwUserTraceThread(nullptr),
		m_sysmonProvider(nullptr) {}

	virtual ~EventCollectorETW() 
	{ 
		if (m_kernelTrace)
		{
			m_kernelTrace->stop();
		}

		if (m_userTrace)
		{
			m_userTrace->stop();
		}

		//Waiting for threads to finish
		if (m_etwUserTraceThread && m_etwKernelTraceThread)
		{
			m_etwUserTraceThread->join();
			m_etwKernelTraceThread->join();
		}

		//Wait for internal KrabsETW threads to finish
		while (m_isUserTraceStarted) Sleep(500);
		while (m_isKernelTraceStarted) Sleep(500);
	}

private:

	//ETW providers cCallbacks
	bool SetupCallbackSysmonEventsHandler();
	bool SetupCallbackPowershellEventsHandler();
	bool SetupCallbackKernelProcessCreateHandler();

	//Internal Logic
	bool InitETWCollectors();
	bool InitializeCustomETWProviders();
	bool InitializeProvidersCallbacks();

	//private variables
	std::atomic<bool> m_isInitialized;
	std::atomic<bool> m_isEnabled;
	std::atomic<bool> m_isKernelTraceStarted;
	std::atomic<bool> m_isUserTraceStarted;
	MatchingEngine &m_eventsProcessor;
	TraceHelpers::Logger &m_logger;
	ConfigManager &m_config;
	std::shared_ptr<krabs::kernel_trace> m_kernelTrace;
	std::shared_ptr<krabs::user_trace> m_userTrace;
	std::shared_ptr<std::thread> m_etwKernelTraceThread;
	std::shared_ptr<std::thread> m_etwUserTraceThread;
	boost::mutex m_mutex;

	//KrabsETW convenience providers
	krabs::kernel::process_provider m_kernelProcessProvider;
	krabs::kernel::alpc_provider m_kernelALPCProvider;
	krabs::kernel::debug_print_provider m_kernelDebugPrintProvider;
	krabs::kernel::disk_io_provider m_kernelDiskIOProvider;
	krabs::kernel::disk_init_io_provider m_kernelDiskInitProvider;
	krabs::kernel::file_io_provider m_kernelFileIOProvider;
	krabs::kernel::file_init_io_provider m_kernelFileInitProvider;
	krabs::kernel::thread_dispatch_provider m_kernelThreadDispatcherProvider;
	krabs::kernel::image_load_provider m_kernelImageLoadProvider;
	krabs::kernel::memory_hard_fault_provider m_kernelMemoryHardFaultProvider;
	krabs::kernel::memory_page_fault_provider m_kernelMemoryPageFaultProvider;
	krabs::kernel::network_tcpip_provider m_kernelNetworkTCPIPProvider;
	krabs::kernel::process_counter_provider m_kernelProcessCounterProvider;
	krabs::kernel::profile_provider m_kernelProfileProvider;
	krabs::kernel::registry_provider m_kernelRegistryProvider;
	krabs::kernel::split_io_provider m_kernelSplitIOProvider;
	krabs::kernel::system_call_provider m_kernelSystemCallProvider;
	krabs::kernel::thread_provider m_kernelThreadProvider;

	//Custom ETW providers
	std::shared_ptr <krabs::provider<>> m_sysmonProvider;
	std::shared_ptr <krabs::provider<>> m_powershellProvider;
};

