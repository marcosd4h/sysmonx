#pragma once

#include "common.h"

using boost::asio::dispatch;
using boost::asio::spawn;
using boost::asio::strand;
using boost::asio::thread_pool;
using boost::asio::yield_context;

class CacheManager
{
public:
	static CacheManager& Instance()
	{
		static CacheManager instance;
		return instance;
	}

	bool IsInitialized() { return m_isInitialized; };

	//Public Members
	bool Initialize();
	void ClearCacheData();

	// Entry points for new cache update request arriving to the cache manager
    // Code return inmediately after enqueuing report events to be handled by the execution thread
	void DispatchCacheUpdate(SysmonXTypes::CacheData& newCacheUpdateRequest)
	{
		if (IsInitialized())
		{
			spawn(m_pool, boost::bind(&CacheManager::NewCacheUpdateHandler, this, newCacheUpdateRequest));
		}
	}

	//Cache helpers
	bool GetProcessCacheData(const PROCESS_ID_T &processID, SysmonXTypes::CacheProcessData& cacheData);
	bool GetThreadCacheData(const PROCESS_ID_T& processID, SysmonXTypes::CacheThreadData& cacheData);
	bool GetExclusionsCacheData(const PROCESS_ID_T& processID, SysmonXTypes::CacheExclusionsData& cacheData);

	bool IsProcessRunning(const PROCESS_ID_T& processID);
	bool IsThreadRunning(const PROCESS_ID_T& processID);
	bool IsProcessExcluded(const PROCESS_ID_T& processID);

private:

	//Handler of new report request
	void NewCacheUpdateHandler(SysmonXTypes::CacheData& newCacheUpdateRequest)
	{
		//Reports are dispatched in a serialized fashion
		dispatch(m_dispatchStrand, boost::bind(&CacheManager::SerializedCacheUpdateHandler, this, newCacheUpdateRequest));
	}

	//m_service is initialized first and then passed to m_work as parameter
	CacheManager(const size_t nrThreads = SysmonXDefs::SYSMONX_DEFAULT_WORKER_THREADS) :
		m_pool(nrThreads), m_dispatchStrand(m_pool.get_executor()) {}

	~CacheManager()
	{
		if (IsInitialized())
		{
			// execute all unfinished work in case this object is leaving scope. 
			m_pool.join();
		}
	}


	//Cache handling should happen here
	void SerializedCacheUpdateHandler(SysmonXTypes::CacheData& newCacheUpdateRequest);
	bool UpdateProcessCache(SysmonXTypes::CacheProcessData& newProcessCacheUpdate);
	bool UpdateThreadCache(SysmonXTypes::CacheThreadData& newThreadCacheUpdate);
	bool UpdateExclusionsCache(SysmonXTypes::CacheExclusionsData& newExclusionsCacheUpdate);

	//Forcing singleton here
	CacheManager(const CacheManager&) = delete;
	CacheManager(CacheManager&&) = delete;
	CacheManager& operator=(const CacheManager&) = delete;
	CacheManager& operator=(CacheManager&&) = delete;

	// Private vars
	bool m_isInitialized = false;
	TraceHelpers::Logger& m_logger = TraceHelpers::Logger::Instance();
	ConfigManager& m_config = ConfigManager::Instance();
	thread_pool m_pool;
	strand<thread_pool::executor_type> m_dispatchStrand;
	boost::asio::io_service m_service;
	boost::mutex m_mutex;
	SysmonXTypes::ProcessCacheContainer m_ProcessCache;
	SysmonXTypes::ThreadCacheContainer m_ThreadCache;
	SysmonXTypes::ExclusionsCacheContainer m_ExclusionsCache;
};