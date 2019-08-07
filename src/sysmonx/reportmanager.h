#pragma once

#include "common.h"

using boost::asio::dispatch;
using boost::asio::spawn;
using boost::asio::strand;
using boost::asio::thread_pool;
using boost::asio::yield_context;

class ReportManager
{
public:
		static ReportManager& Instance()
		{
			static ReportManager instance;
			return instance;
		}

	bool IsInitialized() { return m_isInitialized; };
	bool Initialize();

	// Entry points for reports arriving to the report manager
	// Code return inmediately after enqueuing report events to be handled by the execution thread
	void DispatchReport(const SysmonXTypes::ReportObject &newReport)
	{
		if (IsInitialized())
		{
			spawn(m_pool, boost::bind(&ReportManager::NewReportHandler, this, newReport));
		}
	}

	//Set report channels selected through configuration
	bool SetTargetReportChannels(const SysmonXTypes::ReportOutputList &reportList);

protected:
	boost::asio::io_service m_service;

private:

	//Adding New Reporter
	template <typename T>
	bool AddNewReporter(const SysmonXTypes::ReportChannelID &reporterID, std::shared_ptr<T>& reporterPtr)
	{
		bool ret = false;

		if (reporterPtr)
		{
			//Checking if element is not there
			if (m_reporters.find(reporterID) == m_reporters.end())
			{
				m_reporters.insert(std::make_pair(reporterID, reporterPtr));
				ret = true;
			}
		}

		return ret;
	}

	//Handler of new report request
	void NewReportHandler(const SysmonXTypes::ReportObject &newReport)
	{
		//Reports are dispatched in a serialized fashion
		dispatch(m_dispatchStrand, boost::bind(&ReportManager::SerializedReportHandler, this, newReport));
	}

	//m_service is initialized first and then passed to m_work as parameter
	ReportManager(const size_t nrThreads = SysmonXDefs::SYSMONX_DEFAULT_WORKER_THREADS) :
		m_isInitialized(false), m_foundReportersEnabledByConfig(false),
		m_pool(nrThreads), m_dispatchStrand(m_pool.get_executor()),
		m_logger(TraceHelpers::Logger::Instance()), m_config(ConfigManager::Instance()) {}

	~ReportManager() 	
	{
		if (IsInitialized())
		{
			// Unregister ETW provider
			EventUnregisterSysmonX();

			// execute all unfinished work in case this object is leaving scope. 
			m_pool.join();
		}
	}

	//Report handling should happen here
	void SerializedReportHandler(const SysmonXTypes::ReportObject &newReport);
	bool WasReportChannelEnabledThroughConfig(const SysmonXTypes::ReportChannelID &reportID);

	//Forcing singleton here
	ReportManager(const ReportManager&) = delete;
	ReportManager(ReportManager&&) = delete;
	ReportManager& operator=(const ReportManager&) = delete;
	ReportManager& operator=(ReportManager&&) = delete;

	// Private vars
	bool m_isInitialized;
	bool m_foundReportersEnabledByConfig;
	thread_pool m_pool;
	strand<thread_pool::executor_type> m_dispatchStrand;
	TraceHelpers::Logger& m_logger;
	ConfigManager& m_config;
	boost::mutex m_mutex;
	SysmonXTypes::ReportersContainer m_reporters;
	SysmonXTypes::ReportOutputList m_targetConfigurationReporters;
	ReportOutputList m_enabledReportersLists;
};