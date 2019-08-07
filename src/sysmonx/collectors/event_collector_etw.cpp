#include "common.h"

bool EventCollectorETW::Initialize()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;

	m_logger.Trace("EventCollectorETW::Initialize() - About to initialize Event Collector ETW Technology");

	if (m_config.IsInitialized() && !IsInitialized())
	{
		//Initializing KrabsETW collectors
		m_kernelTrace = std::make_shared<krabs::kernel_trace>(SysmonXDefs::SYSMONX_KERNEL_TRACE_NAME);
		m_userTrace = std::make_shared<krabs::user_trace>(SysmonXDefs::SYSMONX_USERSPACE_TRACE_NAME);

		if (InitializeCustomETWProviders() && m_kernelTrace && m_userTrace)
		{
			//Stopping trace sessions for now
			m_kernelTrace->stop();
			m_userTrace->stop();

			//Setting running flags
			m_isEnabled = false;
			m_isInitialized = true;
			ret = true;
		}
	}
	else
	{
		ret = true;
	}

	return ret;
}


bool EventCollectorETW::UpdatePolicy(const SysmonXTypes::RulesContainer &reportData)
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;

	if (IsInitialized())
	{
		m_isEnabled = false;
		m_logger.Trace("EventCollectorETW::UpdatePolicy() - About to update ETW Event collection policy");

		ret = true;
	}

	return ret;
}

bool EventCollectorETW::StartEventsCollection()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;

	if (IsInitialized() && InitETWCollectors())
	{
		m_logger.Trace("EventCollectorETW::StartEventsCollection() - About to start ETW event collection");

		//Starting KrabsETW collection
		m_etwKernelTraceThread = std::make_shared<std::thread>([&]() { m_isKernelTraceStarted = true; m_kernelTrace->start(); m_isKernelTraceStarted = false; });
		m_etwUserTraceThread = std::make_shared<std::thread>([&]() { m_isUserTraceStarted = true; m_userTrace->start(); m_isUserTraceStarted = false; });

		//Check if threads are still running
		if (m_isKernelTraceStarted && m_isUserTraceStarted && m_etwUserTraceThread && m_etwKernelTraceThread)
		{
			m_logger.Trace("EventCollectorETW::StartEventsCollection() - ETW Collection Started!");
			m_isEnabled = true;
			ret = true;
		}
		else
		{
			m_logger.Error("EventCollectorETW::StartEventsCollection() - There was a problem starting ETW collection");
		}
	}

	return ret;
}

bool EventCollectorETW::StopEventsCollection()
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	bool ret = false;

	m_logger.Trace("EventCollectorETW::StopEventsCollection() - About to stop ETW event collection");

	if (IsInitialized())
	{
		//Waiting running thread to finish
		if (m_isEnabled)
		{
			//Waiting for current thread to finish
			m_isEnabled = false;

			if (m_etwUserTraceThread && m_etwKernelTraceThread)
			{
				m_kernelTrace->stop();
				m_userTrace->stop();
			
				//Wait for internal KrabsETW threads to finish
				while (m_isUserTraceStarted) Sleep(500);
				while (m_isKernelTraceStarted) Sleep(500);

				ret = true;
			}
		}
		else
		{
			//Thread is already finished
			ret = true;
		}
	}

	return ret;
}
