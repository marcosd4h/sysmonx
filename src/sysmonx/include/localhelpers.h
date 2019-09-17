#pragma once

#include "common.h"

namespace SysmonXHelpers
{
	bool GetSystemProcessesData(SysmonXCommon::ProcessCollection &sysProcHandlers);
	bool CleanupSystemProcessData(SysmonXCommon::ProcessCollection &procHandlers);
	bool GetMemoryRegionInfo(HANDLE &hProcess, size_t &dwStartAddress, MEMORY_BASIC_INFORMATION &info);
	bool GetThreadBasicInfo(HANDLE &thread, CustomWinTypes::THREAD_BASIC_INFORMATION &threadInfo);
	bool GetThreadStartAddress(HANDLE &thread, PVOID &threadStartAddress);
	bool IsTrustedFile(const std::wstring &fileName);
	bool IsSuspiciousSymbolsOnThreadCallStackBeginning(const SysmonXCommon::CallStackDataList &csElements);
	bool IsAPCInjectionPresent(const SysmonXCommon::CallStackDataList &csElements);
	bool IsPIDInCollection(SysmonXCommon::ProcessCollection &sysProcessesData, const DWORD &pid, SysmonXCommon::ProcessCollection::iterator &it);
	bool IsModuleInThreadsCollection(DWORD &dwProcessId, HANDLE &hProcess, const SysmonXCommon::ThreadsCollection &threadsData, const SysmonXCommon::ModulesCollection &modules, const std::wstring &module, DWORD &tid);
	bool IsModuleInThread(const DWORD &dwProcessId, const HANDLE &hProcess, const DWORD &tid, const SysmonXCommon::ModulesCollection &modules, const std::wstring &module);
	bool GetListOfCallStackModules(const DWORD &dwProcessId, const HANDLE &hProcess, const HANDLE &hThread, const SysmonXCommon::ModulesCollection &modules, SysmonXCommon::CallStackDataList &csElements, bool &suspiciousFound);
	bool IsFuzzyPEHeaderPresent(HANDLE &hProcess, size_t &addressToCheck, size_t &regionSize);
	bool PopulateModulesIfNeededByWalkingPEHeaders(SysmonXCommon::ProcessDataPtr &processData);
	bool PopulateModulesIfNeededByMemorySections(SysmonXCommon::ProcessDataPtr &processData);
	bool GetParentPid(const DWORD &pid, DWORD &parentPID);
	bool GetPEBAddress(const HANDLE &processHandle, PVOID &baseaddr);
	bool GetProcessCommandLine(blackbone::Process &targetProc, std::wstring &cmdline);	

	class ThreadCallstackWalker
	{
	public:
		static ThreadCallstackWalker& GetInstance()
		{
			static ThreadCallstackWalker instance;
			return instance;
		}

		bool GetThreadCallStack(const DWORD &pid, HANDLE hProcess, HANDLE hThread, const SysmonXCommon::ModulesCollection &modules, SysmonXCommon::CallStackDataList &element);

	private:
		ThreadCallstackWalker() : currentProcessHandle(INVALID_HANDLE_VALUE), currentPID(0) 
		{
			SymSetOptions(SymGetOptions() |
				SYMOPT_AUTO_PUBLICS | SYMOPT_CASE_INSENSITIVE | 
				SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_INCLUDE_32BIT_MODULES |
				SYMOPT_OMAP_FIND_NEAREST | SYMOPT_UNDNAME |
				SYMOPT_LOAD_ANYTHING | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS | SYMOPT_IGNORE_NT_SYMPATH | SYMOPT_NO_IMAGE_SEARCH
			);
		};

		ThreadCallstackWalker(const ThreadCallstackWalker&) {};

		HANDLE currentProcessHandle; //assuming that handle will be closed by caller
		DWORD currentPID;
	};


	class ModuleExclusionsManagement
	{
	public:
		ModuleExclusionsManagement()
		{
			m_analyzedFiles.clear();
			//const std::wstring &systemDir = ConfigManager::Instance().GetWindowsSystemDirectory();
			std::wstring systemDir;
			if (GeneralHelpers::GetWindowsSystemDirectory(systemDir))
			{
				m_systemDirectory.assign(systemDir);
			}
		}

		bool ShouldBeExcluded(const std::wstring &moduleFullPath)
		{
			bool ret = false;

			if (m_analyzedFiles[moduleFullPath]) // Was the file already scanned
			{
				ret = true;
			}
			else if (GeneralHelpers::StrContainsPatternInsensitive(moduleFullPath, m_systemDirectory)) // Is part of win32 directory?
			{
				m_analyzedFiles[moduleFullPath] = true;
				ret = true;
			}

			return ret;
		}

		void AddToExclusions(const std::wstring &moduleFullPath)
		{
			m_analyzedFiles[moduleFullPath] = true;
		}


	private:

		std::wstring m_systemDirectory;
		std::unordered_map<std::wstring, bool> m_analyzedFiles;
	};


	class RangesExclusionsManagement
	{

	public:
		RangesExclusionsManagement()
		{
			m_rangesToLookFor.clear();
			m_lookupMap.clear();
		}

		void Reset()
		{
			m_rangesToLookFor.clear();
			m_lookupMap.clear();
		}

		bool IsInRange(const size_t &value)
		{
			bool ret = false;

			for (auto it = m_rangesToLookFor.begin(); it != m_rangesToLookFor.end(); ++it)
			{
				//check if value is in range
				if ((value <= it->maxValue) && (value >= it->minValue))
				{
					ret = true;
					break;
				}
			}

			return ret;
		}

		bool IsInRangeFastLookup(const size_t &value)
		{
			return m_lookupMap[value];
		}

		void AddNewRange(const size_t &lower, const size_t &upper, const size_t &regionSize)
		{
			SysmonXCommon::RangeData newData(lower, upper);
			m_rangesToLookFor.push_back(newData);

			for (size_t addrToAdd = lower; addrToAdd <= upper; addrToAdd += regionSize)
			{
				m_lookupMap.insert({ addrToAdd , true });
			}			
		}


	private:
		std::vector<SysmonXCommon::RangeData> m_rangesToLookFor;
		std::unordered_map<size_t, bool> m_lookupMap;
	};
}

namespace WindowsTypes
{
	typedef std::shared_ptr<uint8_t[]> BytePTR;
}

namespace ScanHelpers
{
	bool GetBlackboneProcessFromPID(const size_t targetPID, blackbone::Process &bbProcess);
	bool GetMainModuleFromMemory(blackbone::Process &process, blackbone::pe::PEImage &module);
	bool GetMainModuleFromDisk(blackbone::Process &process, blackbone::pe::PEImage &module);
	bool IsProcessMainModuleHollowed(SysmonXTypes::EventObject& data);
}

