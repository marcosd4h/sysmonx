#include "common.h"

bool CacheManager::Initialize()
{
	boost::lock_guard<boost::mutex> lk(m_mutex);

	bool ret = true;

	m_logger.Trace("CacheManager::Initialize - About to initialize cache management");

	if (m_config.IsInitialized() && !IsInitialized())
	{
		m_isInitialized = true;
		ret = true;
	}
	else
	{
		ret = true;
	}

	return ret;
}

bool CacheManager::UpdateProcessCache(SysmonXTypes::CacheProcessData& newProcessCacheUpdate)
{
	bool ret = false;

	if (newProcessCacheUpdate)
	{
		SysmonXTypes::ProcessCacheContainer::iterator processCacheIT = m_ProcessCache.find(newProcessCacheUpdate->ProcessId);
		if (processCacheIT != m_ProcessCache.end())
		{
			CacheProcessData& currentProcessCacheData = processCacheIT->second;
			if (currentProcessCacheData->IsRunning)
			{
				//entry is present, so swap with new content
				currentProcessCacheData.swap(newProcessCacheUpdate);
			}
			else
			{
				//new entry
				m_ProcessCache.insert(std::pair<SysmonXTypes::PROCESS_ID_T, SysmonXTypes::CacheProcessData>(newProcessCacheUpdate->ProcessId, newProcessCacheUpdate));
			}

			ret = true;
		}
	}

	return ret;
}

bool CacheManager::UpdateThreadCache(SysmonXTypes::CacheThreadData& newThreadCacheUpdate)
{
	bool ret = false;

	if (newThreadCacheUpdate)
	{
		SysmonXTypes::ThreadCacheContainer::iterator threadCacheIT = m_ThreadCache.find(newThreadCacheUpdate->ProcessId);
		if (threadCacheIT != m_ThreadCache.end())
		{
			CacheThreadData& currentThreadCacheData = threadCacheIT->second;
			if (currentThreadCacheData->IsRunning)
			{
				//entry is present, so swap with new content
				currentThreadCacheData.swap(newThreadCacheUpdate);
			}
			else
			{
				//new entry
				m_ThreadCache.insert(std::pair<SysmonXTypes::PROCESS_ID_T, SysmonXTypes::CacheThreadData>(newThreadCacheUpdate->ProcessId, newThreadCacheUpdate));
			}

			ret = true;
		}
	}

	return ret;
}

bool CacheManager::UpdateExclusionsCache(SysmonXTypes::CacheExclusionsData& newExclusionsCacheUpdate)
{
	bool ret = false;

	if (newExclusionsCacheUpdate)
	{
		SysmonXTypes::ExclusionsCacheContainer::iterator exclusionsCacheIT = m_ExclusionsCache.find(newExclusionsCacheUpdate->ProcessId);
		if (exclusionsCacheIT != m_ExclusionsCache.end())
		{
			CacheExclusionsData& currentExclusionsCacheData = exclusionsCacheIT->second;
			if (currentExclusionsCacheData->IsEnabled)
			{
				//entry is present, so swap with new content
				currentExclusionsCacheData.swap(newExclusionsCacheUpdate);
			}
			else
			{
				//new entry
				m_ExclusionsCache.insert(std::pair<SysmonXTypes::PROCESS_ID_T, SysmonXTypes::CacheExclusionsData>(newExclusionsCacheUpdate->ProcessId, newExclusionsCacheUpdate));
			}

			ret = true;
		}
	}

	return ret;
}

//Cache handling should happen here
void CacheManager::SerializedCacheUpdateHandler(SysmonXTypes::CacheData& newCacheUpdateRequest)
{
	if ((newCacheUpdateRequest.Type == CacheUpdateRequestType::CACHE_UPDATE_REQUEST_PROCESS) &&
		newCacheUpdateRequest.ProcessData)
	{
		if (!UpdateProcessCache(newCacheUpdateRequest.ProcessData))
		{
			m_logger.Trace("There was a problem updating Process Cache");
		}
	}
	else if ((newCacheUpdateRequest.Type == CacheUpdateRequestType::CACHE_UPDATE_REQUEST_THREAD) &&
			newCacheUpdateRequest.ProcessData)
	{
		if (!UpdateThreadCache(newCacheUpdateRequest.ThreadData))
		{
			m_logger.Trace("There was a problem updating Thread Cache");
		}
	}
	else if ((newCacheUpdateRequest.Type == CacheUpdateRequestType::CACHE_UPDATE_REQUEST_EXCLUSION) &&
			newCacheUpdateRequest.ExclusionsData)
	{
		if (!UpdateExclusionsCache(newCacheUpdateRequest.ExclusionsData))
		{
			m_logger.Trace("There was a problem updating Exclusions Cache");
		}
	}
}

//Clear cache data helper
void CacheManager::ClearCacheData()
{
	boost::lock_guard<boost::mutex> lk(m_mutex);
	m_ProcessCache.clear();
	m_ThreadCache.clear();
	m_ExclusionsCache.clear();
}


bool CacheManager::GetProcessCacheData(const PROCESS_ID_T& processID, SysmonXTypes::CacheProcessData& cacheData)
{
	bool ret = false;

	if (processID != SysmonXDefs::INVALID_PROCESS_ID)
	{
		SysmonXTypes::ProcessCacheContainer::iterator processCacheIT = m_ProcessCache.find(processID);
		if (processCacheIT != m_ProcessCache.end())
		{
			if (processCacheIT->second)
			{
				cacheData = processCacheIT->second;
				ret = true;
			}
		}
	}

	return ret;
}

bool CacheManager::GetThreadCacheData(const PROCESS_ID_T& processID, SysmonXTypes::CacheThreadData& cacheData)
{
	bool ret = false;

	if ((processID != SysmonXDefs::INVALID_PROCESS_ID) && (processID != SysmonXDefs::INVALID_THREAD_ID))
	{
		SysmonXTypes::ThreadCacheContainer::iterator threadCacheIT = m_ThreadCache.find(processID);
		if (threadCacheIT != m_ThreadCache.end())
		{
			if (threadCacheIT->second)
			{
				cacheData = threadCacheIT->second;
				ret = true;
			}
		}
	}

	return ret;
}

bool CacheManager::GetExclusionsCacheData(const PROCESS_ID_T& processID, SysmonXTypes::CacheExclusionsData& cacheData)
{
	bool ret = false;

	if (processID != SysmonXDefs::INVALID_PROCESS_ID)
	{
		SysmonXTypes::ExclusionsCacheContainer::iterator exclusionsCacheIT = m_ExclusionsCache.find(processID);
		if (exclusionsCacheIT != m_ExclusionsCache.end())
		{
			if (exclusionsCacheIT->second)
			{
				cacheData = exclusionsCacheIT->second;
				ret = true;
			}
		}
	}

	return ret;
}


bool CacheManager::IsProcessRunning(const PROCESS_ID_T& processID)
{
	bool ret = false;

	if (processID != SysmonXDefs::INVALID_PROCESS_ID)
	{
		SysmonXTypes::ProcessCacheContainer::iterator processCacheIT = m_ProcessCache.find(processID);
		if (processCacheIT != m_ProcessCache.end())
		{
			if (processCacheIT->second)
			{
				CacheProcessData& currentProcessCacheData = processCacheIT->second;
				if (currentProcessCacheData->IsRunning)
				{
					ret = true;
				}
			}
		}
	}

	return ret;
}

bool CacheManager::IsThreadRunning(const PROCESS_ID_T& processID)
{
	bool ret = false;

	if (processID != SysmonXDefs::INVALID_PROCESS_ID)
	{
		SysmonXTypes::ThreadCacheContainer::iterator threadCacheIT = m_ThreadCache.find(processID);
		if (threadCacheIT != m_ThreadCache.end())
		{
			if (threadCacheIT->second)
			{
				CacheThreadData& currentThreadCacheData = threadCacheIT->second;
				if (currentThreadCacheData->IsRunning)
				{
					ret = true;
				}
			}
		}
	}

	return ret;
}

bool CacheManager::IsProcessExcluded(const PROCESS_ID_T& processID)
{
	bool ret = false;

	if (processID != SysmonXDefs::INVALID_PROCESS_ID)
	{
		SysmonXTypes::ExclusionsCacheContainer::iterator exclusionsCacheIT = m_ExclusionsCache.find(processID);
		if (exclusionsCacheIT != m_ExclusionsCache.end())
		{
			if (exclusionsCacheIT->second)
			{
				CacheExclusionsData& currentExclusionsCacheData = exclusionsCacheIT->second;
				if (currentExclusionsCacheData->IsEnabled)
				{
					ret = true;
				}
			}
		}
	}

	return ret;
}
