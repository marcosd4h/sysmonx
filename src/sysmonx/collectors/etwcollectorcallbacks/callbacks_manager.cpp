#include "common.h"

bool EventCollectorETW::InitializeCustomETWProviders()
{
	bool ret = false;

	if (m_config.IsInitialized())
	{
		m_sysmonProvider = std::make_shared<krabs::provider<>>(ETWProvidersName::SYSMON_PROVIDER_NAME);
		m_powershellProvider = std::make_shared<krabs::provider<>>(ETWProvidersName::POWERSHELL_PROVIDER_NAME);

		if (m_sysmonProvider)
		{
			ret = true;
		}
	}

	return ret;
}


bool EventCollectorETW::InitializeProvidersCallbacks()
{
	bool ret = false;

	m_logger.Trace("About to execute EventCollectorETW::InitializeProvidersCallbacks()");

	if (IsInitialized())
	{
		//Setting up sysmon events provider
		if (!SetupCallbackSysmonEventsHandler())
		{
			m_logger.Error("EventCollectorETW::InitializeProvidersCallbacks() - There was a problem setting up callback for sysmon provider");
			return ret;
		}

		//Setting up Kernel Callback Process provider
		if (!SetupCallbackKernelProcessCreateHandler())
		{
			m_logger.Trace("EventCollectorETW::InitializeProvidersCallbacks() - There was a problem setting up callback for kernel create process");
			return ret;
		}

		//Setting up ETW Powershell provider
		if (!SetupCallbackPowershellEventsHandler())
		{
			m_logger.Trace("EventCollectorETW::InitializeProvidersCallbacks() - There was a problem setting up callback for powershell events handler");
			return ret;
		}

		ret = true;
	}

	return ret;
}

bool EventCollectorETW::InitETWCollectors()
{
	bool ret = false;

	m_logger.Trace("InitETWCollectors() - About to execute EventCollectorETW::InitETWCollectors()");
	if (IsInitialized())
	{
		//Initializing different callbacks
		if (InitializeProvidersCallbacks() && m_kernelTrace && m_userTrace)
		{
			//Enabling convenience kernel providers
			m_kernelTrace->enable(m_kernelProcessProvider);
			m_kernelTrace->enable(m_kernelImageLoadProvider);
			m_kernelTrace->enable(m_kernelThreadProvider);
			//m_kernelTrace->enable(m_kernelRegistryProvider);
			//m_kernelTrace->enable(m_kernelALPCProvider);
			//m_kernelTrace->enable(m_kernelDebugPrintProvider);
			//m_kernelTrace->enable(m_kernelDiskIOProvider);
			//m_kernelTrace->enable(m_kernelDiskInitProvider);
			//m_kernelTrace->enable(m_kernelFileIOProvider);
			//m_kernelTrace->enable(m_kernelFileInitProvider);
			//m_kernelTrace->enable(m_kernelThreadDispatcherProvider);		
			//m_kernelTrace->enable(m_kernelMemoryHardFaultProvider);
			//m_kernelTrace->enable(m_kernelMemoryPageFaultProvider);
			//m_kernelTrace->enable(m_kernelNetworkTCPIPProvider);
			//m_kernelTrace->enable(m_kernelProcessCounterProvider);
			//m_kernelTrace->enable(m_kernelProfileProvider);		
			//m_kernelTrace->enable(m_kernelSplitIOProvider);
			//m_kernelTrace->enable(m_kernelSystemCallProvider);

			//Enabling Custom providers
			m_userTrace->enable(*m_sysmonProvider);
			m_userTrace->enable(*m_powershellProvider);

			ret = true;
		}
		else
		{
			m_logger.Trace("InitETWCollectors() - There was a problem initializing providers callbacks()");
		}
	}

	return ret;
}


