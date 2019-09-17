#include <libcommon.h>


namespace TraceHelpers
{
	void Logger::TraceUp(const char *buffer, ...)
	{
		if (buffer)
		{
			char formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };
			va_list varadic;
			va_start(varadic, buffer);
			_vsnprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::string outputBuff;
			outputBuff.append(CommonDefs::UPMARK);
			//outputBuff.append(CommonDefs::DEFAULT_SEP);
			//outputBuff.append(CommonDefs::SEPARATOR);
			outputBuff.append(formattedBuff);
			outputBuff.append(CommonDefs::ENDLINE);

			OutputDebugStringA(outputBuff.c_str());
		}
	}

	void Logger::TraceDown(const char *buffer, ...)
	{
		if (buffer)
		{
			char formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::string outputBuff;
			outputBuff.append(CommonDefs::DOWNMARK);
			//outputBuff.append(CommonDefs::DEFAULT_SEP);
			//outputBuff.append(CommonDefs::SEPARATOR);
			outputBuff.append(formattedBuff);
			outputBuff.append(CommonDefs::ENDLINE);

			OutputDebugStringA(outputBuff.c_str());
		}
	}

	void Logger::TraceConsoleUp(const char *buffer, ...)
	{
		if (buffer)
		{
			char formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::cout << CommonDefs::UPMARK << formattedBuff << CommonDefs::ENDLINE;
		}
	}

	void Logger::TraceConsoleDown(const char *buffer, ...)
	{
		if (buffer)
		{
			char formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::cerr << CommonDefs::DOWNMARK << formattedBuff << CommonDefs::ENDLINE;
		}
	}

	void Logger::TraceConsole(const char *buffer, ...)
	{
		if (buffer)
		{
			char formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::cout << formattedBuff << CommonDefs::ENDLINE;
		}
	}

	void Logger::TraceUp(const wchar_t *buffer, ...)
	{
		if (buffer)
		{
			wchar_t formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };
			va_list varadic;
			va_start(varadic, buffer);
			_vsnwprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::wstring outputBuff;
			outputBuff.append(CommonDefs::WUPMARK);
			//outputBuff.append(CommonDefs::WDEFAULT_SEP);
			outputBuff.append(CommonDefs::WSEPARATOR);
			outputBuff.append(formattedBuff);
			outputBuff.append(CommonDefs::WENDLINE);

			OutputDebugStringW(outputBuff.c_str());
		}
	}

	void Logger::TraceDown(const wchar_t *buffer, ...)
	{
		if (buffer)
		{
			wchar_t formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnwprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::wstring outputBuff;
			outputBuff.append(CommonDefs::WDOWNMARK);
			//outputBuff.append(CommonDefs::WDEFAULT_SEP);
			outputBuff.append(CommonDefs::WSEPARATOR);
			outputBuff.append(formattedBuff);
			outputBuff.append(CommonDefs::WENDLINE);

			OutputDebugStringW(outputBuff.c_str());
		}
	}

	void Logger::TraceConsoleUp(const wchar_t *buffer, ...)
	{
		if (buffer)
		{
			wchar_t formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnwprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::wcout << CommonDefs::WUPMARK << formattedBuff << CommonDefs::WENDLINE;
		}
	}

	void Logger::TraceConsoleDown(const wchar_t *buffer, ...)
	{
		if (buffer)
		{
			wchar_t formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnwprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::wcerr << CommonDefs::WDOWNMARK << formattedBuff << CommonDefs::WENDLINE;
		}
	}

	void Logger::TraceConsole(const wchar_t *buffer, ...)
	{
		if (buffer)
		{
			wchar_t formattedBuff[CommonDefs::MAX_BUFFER_SIZE] = { 0 };

			va_list varadic;
			va_start(varadic, buffer);
			_vsnwprintf_s(formattedBuff, (CommonDefs::MAX_BUFFER_SIZE - 1), CommonDefs::MAX_BUFFER_SIZE, buffer, varadic);
			va_end(varadic);

			std::wcout << formattedBuff << CommonDefs::WENDLINE;
		}
	}

	bool Logger::Initialize(const CommonTypes::LoggerContainer &modes, std::string logfile)
	{
		bool ret = false;

		if (!modes.empty())
		{
			spdlog::sink_ptr consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			spdlog::sink_ptr fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile.c_str(), true);

			bool isFileEnabled = false;
			bool isConsoleEnabled = false;

			for (auto it = modes.begin(); it != modes.end(); it++)
			{
				CommonTypes::LoggerMode workMode(*it);

				if (workMode == CommonTypes::LoggerMode::LOGGER_FILE)
				{
					isFileEnabled = true;
				}
				else if (workMode == CommonTypes::LoggerMode::LOGGER_CONSOLE)
				{
					isConsoleEnabled = true;
				}
			}

			if (isFileEnabled && isConsoleEnabled)
			{
				m_logger = new spdlog::logger(logfile.c_str(), { consoleSink, fileSink });
			}
			else if (isFileEnabled)
			{
				m_logger = new spdlog::logger(logfile.c_str(), fileSink);
			}
			else if (isConsoleEnabled)
			{
				m_logger = new spdlog::logger(logfile.c_str(), consoleSink);
			}
			else
			{
				m_logger = nullptr;
			}

			if (m_logger)
			{
				m_logger->set_level((spdlog::level::level_enum)CommonTypes::LoggerVerbose::LOGGER_ERR);
				m_logger->set_pattern(CommonDefs::DEFAULT_LOG_PATTERN);
				spdlog::flush_every(std::chrono::seconds(1));
				m_initialized = true;
				ret = true;
			}
		}

		return ret;
	}

	void Logger::SetLevel(CommonTypes::LoggerVerbose level)
	{
		if (m_initialized && m_logger)
		{
			m_logger->set_level((spdlog::level::level_enum)level);
		}
	}

}

namespace GeneralHelpers
{
	int ToInteger(const std::wstring &str)
	{
		return std::stoi(str);
	}


	bool StartProcess(const std::wstring &process, const std::wstring &args)
	{
		bool ret = false;


		if (!process.empty() && !args.empty())
		{
			std::wstring workingArgs(process);
			workingArgs.append(L" ");
			workingArgs.append(args);

			STARTUPINFOW si = { 0 };
			PROCESS_INFORMATION pi = { 0 };

			if (CreateProcessW(
				process.c_str(),
				(LPWSTR)workingArgs.c_str(),
				NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
			{
				WaitForSingleObject(pi.hProcess, INFINITE);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				ret = true;
			}
		}

		return ret;
	}

	bool IsValidFile(const std::wstring &fileName)
	{
		bool ret = false;
		HANDLE hFile = INVALID_HANDLE_VALUE;

		hFile = CreateFile(fileName.c_str(),       // file to open
			GENERIC_READ,          // open for reading
			FILE_SHARE_READ,       // share for reading
			NULL,                  // default security
			OPEN_EXISTING,         // existing file only
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
			NULL);                 // no attr. template

		if (hFile != INVALID_HANDLE_VALUE)
		{
			ret = true;
			CloseHandle(hFile);
		}

		return ret;
	}

	bool IsValidDirectory(const std::wstring &directory)
	{
		bool ret = false;

		if (!directory.empty())
		{
			DWORD dirAttributes = GetFileAttributes(directory.c_str());

			if ((dirAttributes != INVALID_FILE_ATTRIBUTES) &&
				(dirAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				ret = true;
			}
		}

		return ret;
	}

	bool GetFullPathToFile(const std::wstring &fileName, std::wstring &fullPathFile)
	{
		bool ret = false;
		wchar_t ptargetFile[MAX_PATH + 1] = { 0 };
		std::wstring workFilePath;

		if (GetFullPathNameW(fileName.c_str(), MAX_PATH, ptargetFile, NULL) != 0)
		{
			workFilePath.assign(ptargetFile);

			if (!workFilePath.empty() && IsValidFile(workFilePath))
			{
				fullPathFile.assign(workFilePath);
				ret = true;
			}
		}

		return ret;
	}

	bool IsNumber(const std::wstring& str)
	{
		bool ret = false;
		std::wstring::const_iterator it = str.begin();
		while (it != str.end() && iswdigit(*it)) ++it;
		if (!str.empty() && it == str.end())
		{
			ret = true;
		}
		return ret;
	}

	bool GetBaseFileName(const std::wstring &fullPath, std::wstring &baseName)
	{
		bool ret = false;
		wchar_t sep = '\\';
		std::wstring workingStr(fullPath);
		size_t it = workingStr.rfind(sep, workingStr.length());
		if (it != std::wstring::npos)
		{
			baseName.assign(workingStr.substr(it + 1, workingStr.length() - it));
			ret = true;
		}

		return ret;
	}

	bool GetCurrentProcessModuleFullPath(std::wstring &fullPath)
	{
		bool ret = false;
		DWORD readBytes = 0;
		wchar_t modulePathBuff[MAX_PATH] = { 0 };

		if (GetModuleFileNameEx(GetCurrentProcess(), NULL, modulePathBuff, (sizeof(modulePathBuff) / sizeof(*modulePathBuff))))
		{
			fullPath.assign(modulePathBuff);
			ret = true;
		}

		return ret;
	}

	bool GetCurrentProcessModuleDirectory(std::wstring &fullDirectoryPath)
	{
		bool ret = false;
		std::wstring currentExecPath;

		TCHAR currentDrive[MAX_PATH] = { 0 };
		TCHAR currentDir[MAX_PATH] = { 0 };

		if (GetCurrentProcessModuleFullPath(currentExecPath) && !currentExecPath.empty())
		{
			if ((_tsplitpath_s(currentExecPath.c_str(), currentDrive, MAX_PATH, currentDir, MAX_PATH, NULL, 0, NULL, 0) == 0))
			{
				fullDirectoryPath.assign(currentDrive);
				fullDirectoryPath.append(currentDir);
				if (!fullDirectoryPath.empty())
				{
					ret = true;
				}
			}
		}

		return ret;
	}

	bool TerminateProcess(const DWORD &processID, const uint32_t &exitCode)
	{
		bool ret = false;

		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
		if (hProcess != NULL)
		{
			if (::TerminateProcess(hProcess, exitCode))
			{
				ret = true;
			}

			CloseHandle(hProcess);
		}

		return ret;
	}

	template <typename StringBuffer>
	bool InternalStrCompare(StringBuffer& str1, StringBuffer& str2)
	{
		bool ret = false;

		if ((!str1.empty()) && (!str2.empty()) && (str1.compare(str2) == 0))
		{
			ret = true;
		}

		return ret;
	}

	bool StrCompare(const std::wstring& str1, const std::wstring& str2)
	{
		return InternalStrCompare(str1, str2);
	}

	bool StrCompare(const std::string& str1, const std::string& str2)
	{
		return InternalStrCompare(str1, str2);
	}

	bool StrCompareInsensitive(const std::wstring& str1, const std::wstring& str2)
	{
		std::wstring test1(str1);
		std::wstring test2(str2);
		StrToLowercase(test1);
		StrToLowercase(test2);
		return InternalStrCompare(str1, str2);
	}

	bool StrCompareInsensitive(const std::string& str1, const std::string& str2)
	{
		std::string test1(str1);
		std::string test2(str2);
		StrToLowercase(test1);
		StrToLowercase(test2);
		return InternalStrCompare(test1, test2);
	}

	bool StrContainsPattern(const std::wstring& str, const std::wstring& pattern)
	{
		bool ret = false;

		if (str.find(pattern) != std::wstring::npos)
		{
			ret = true;
		}

		return ret;
	}

	bool StrContainsPatternInsensitive(const std::wstring& str, const std::wstring& pattern)
	{
		bool ret = false;

		if (!str.empty() && !pattern.empty())
		{
			std::wstring test1(str);
			std::wstring test2(pattern);
			StrToLowercase(test1);
			StrToLowercase(test2);

			if (test1.find(test2) != std::wstring::npos)
			{
				ret = true;
			}
		}

		return ret;
	}

	bool StrContainsPattern(const std::string& str, const std::string& pattern)
	{
		bool ret = false;

		if (str.find(pattern) != std::string::npos)
		{
			ret = true;
		}

		return ret;
	}

	bool StrContainsPatternInsensitive(const std::string& str, const std::string& pattern)
	{
		bool ret = false;

		std::string test1(str);
		std::string test2(pattern);
		StrToLowercase(test1);
		StrToLowercase(test2);

		if (test1.find(test2) != std::string::npos)
		{
			ret = true;
		}

		return ret;
	}

	bool IsRunningAs64BitProcess()
	{
#if defined(_WIN64)
		return true;
#else
		bool ret = false;
		BOOL isWow64Process = false;

		if (IsWow64Process(GetCurrentProcess(), &isWow64Process))
		{
			if (isWow64Process)
			{
				ret = true;
			}
			else
			{
				ret = false;
			}
		}

		return ret;
#endif
	}

	bool IsRunningAsAdmin()
	{
		bool ret = false;
		DWORD tokenGroupSize = 0;
		SID adminGroupContainer = { 0 };
		PSID pAdminGroup = &adminGroupContainer;
		BOOL isRunningAsAdmin = false;
		HANDLE hToken = INVALID_HANDLE_VALUE;
		char localBuf[4096] = { 0 };
		PTOKEN_GROUPS pGroupInfo = (PTOKEN_GROUPS)localBuf;

		if (!IsWindows7OrGreater())
		{
			ret = true;
		}
		else
		{
			if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
			{
				// Getting group token data sizeof current process
				if ((!GetTokenInformation(hToken, TokenGroups, NULL, tokenGroupSize, &tokenGroupSize)) &&
					(GetLastError() == ERROR_INSUFFICIENT_BUFFER))
				{
					pGroupInfo = (PTOKEN_GROUPS)GlobalAlloc(GPTR, tokenGroupSize);

					if (pGroupInfo &&
						(GetTokenInformation(hToken, TokenGroups, pGroupInfo, tokenGroupSize, &tokenGroupSize)))
					{
						SID_IDENTIFIER_AUTHORITY authNTSID = SECURITY_NT_AUTHORITY;
						if (AllocateAndInitializeSid(&authNTSID, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminGroup))
						{
							if (pAdminGroup && CheckTokenMembership(NULL, pAdminGroup, &isRunningAsAdmin) && isRunningAsAdmin)
							{
								//Now comparing current token group with admin sid
								for (DWORD it = 0; it < pGroupInfo->GroupCount; it++)
								{
									if (EqualSid(pAdminGroup, pGroupInfo->Groups[it].Sid))
									{
										ret = true;
										break;
									}
								}
							}
						}
					}
				}

				if (pAdminGroup)
				{
					FreeSid(pAdminGroup);
					pAdminGroup = NULL;
				}

				if (pGroupInfo)
				{
					GlobalFree(pGroupInfo);
					pGroupInfo = NULL;
				}
			}
		}

		return ret;
	}

	bool GetSystemArchitecture(DWORD &arch)
	{
		bool ret = false;
		static SYSTEM_INFO systemInfo = { 0 };

		if (systemInfo.dwPageSize = 0)
		{
			GetNativeSystemInfo(&systemInfo);
			arch = systemInfo.wProcessorArchitecture;
			ret = true;
		}
		else
		{
			arch = systemInfo.wProcessorArchitecture;
			ret = true;
		}

		return ret;
	}

	bool GetProcessBitness(const HANDLE &hProcess, CommonTypes::ProcessType &processType)
	{
		bool ret = false;
		DWORD arch = 0;
		static CustomWinTypes::PFN_ISWOW64PROCESS IsWow64Process = NULL;
		HMODULE kernel32 = NULL;
		BOOL resultWow64Process = false;

		if ((hProcess != INVALID_HANDLE_VALUE) && GetSystemArchitecture(arch))
		{
			processType = CommonTypes::ProcessType::PROCESS_UNKNOWN;

			if (arch == PROCESSOR_ARCHITECTURE_INTEL)
			{
				processType = CommonTypes::ProcessType::PROCESS_32_BITS;
				ret = true;
			}
			else if (arch == PROCESSOR_ARCHITECTURE_AMD64)
			{
				//lazy siglenton
				if (IsWow64Process == NULL)
				{
					if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"kernel32.dll", &kernel32))
					{
						IsWow64Process = (CustomWinTypes::PFN_ISWOW64PROCESS)GetProcAddress(kernel32, "IsWow64Process");
					}
				}

				//IsWow64Process is not present, assuming 32 bits system
				if (IsWow64Process == NULL)
				{
					processType = CommonTypes::ProcessType::PROCESS_32_BITS;
					ret = true;
				}
				else
				{
					if (IsWow64Process(hProcess, &resultWow64Process))
					{
						if (resultWow64Process)
						{
							processType = CommonTypes::ProcessType::PROCESS_WOW_32_BITS;
						}
						else
						{
							processType = CommonTypes::ProcessType::PROCESS_64_BITS;
						}
						ret = true;
					}
				}

			}
		}

		return ret;
	}

	/******************************************************************************
	* This function adjusts the process token.
	* Source: http://cboard.cprogramming.com/c-programming/108648-help-readprocessmemory-function.html#post802074
	* Another good resource: http://winterdom.com/dev/security/tokens
	******************************************************************************/
	bool EnableTokenPrivilege(LPTSTR pPrivilege)
	{
		bool ret = false;
		HANDLE hToken;
		TOKEN_PRIVILEGES token_privileges;
		DWORD dwSize;
		ZeroMemory(&token_privileges, sizeof(token_privileges));
		token_privileges.PrivilegeCount = 1;

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
		{
			if (LookupPrivilegeValue(NULL, pPrivilege, &token_privileges.Privileges[0].Luid))
			{
				token_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				if (AdjustTokenPrivileges(hToken, FALSE, &token_privileges, 0, NULL, &dwSize))
				{
					ret = true;
				}
			}

			CloseHandle(hToken);
		}

		return TRUE;
	}

	bool IsProcessStillRunning(const HANDLE &hProcess)
	{
		bool ret = false;

		DWORD retExitCode;
		if ((GetExitCodeProcess(hProcess, &retExitCode)) && (retExitCode == STILL_ACTIVE))
		{
			ret = true;
		}

		return ret;
	}

	bool IsThreadStillRunning(const HANDLE &hThread)
	{
		bool ret = false;

		DWORD retExitCode;
		if ((GetExitCodeThread(hThread, &retExitCode)) && (retExitCode == STILL_ACTIVE))
		{
			ret = true;
		}

		return ret;
	}

	bool IsDotNETLib(const std::wstring &name)
	{
		bool ret = false;

		if ((StrContainsPatternInsensitive(name, L"clr.dll")) ||
			(StrContainsPatternInsensitive(name, L"mscorwks.dll")) ||
			(StrContainsPatternInsensitive(name, L"coreclr.dll")))
		{
			ret = true;
		}

		return ret;
	}

	bool GetWindowsSystemDirectory(std::wstring &system32Directory)
	{
		bool ret = false;
		TCHAR windir[MAX_PATH] = { 0 };

		system32Directory.clear();
		if (GetWindowsDirectory(windir, MAX_PATH))
		{
			system32Directory.assign(windir);
			system32Directory.append(L"\\SYSTEM32\\");
			ret = true;
		}

		return ret;
	}

	bool GetWindowsSystemRootDirectory(std::wstring &systemRootDirectory)
	{
		bool ret = false;
		TCHAR windir[MAX_PATH] = { 0 };

		systemRootDirectory.clear();
		if (GetWindowsDirectory(windir, MAX_PATH))
		{
			systemRootDirectory.assign(windir);
			systemRootDirectory.append(L"\\");
			ret = true;
		}

		return ret;
	}

	bool GetVectorByToken(const std::string& input, const char token, std::vector<std::string> &vector)
	{
		bool ret = false;
		std::stringstream str(input);
		std::string	line;

		vector.clear();

		while (std::getline(str, line, token))
		{
			vector.push_back(line);
		}

		if (!vector.empty())
		{
			ret = true;
		}

		return ret;
	}

	bool GetVectorByWindowsCRToken(const std::string& input, std::vector<std::string> &vector)
	{
		bool ret = false;

		if (!input.empty())
		{
			std::vector<std::string> workVector;

			if (input.find('\n') != std::string::npos)
			{
				if (GetVectorByToken(input, '\n', workVector))
				{
					for (auto it = workVector.begin(); it != workVector.end(); it++)
					{
						std::string workingStr(*it);
						workingStr.erase(std::remove(workingStr.begin(), workingStr.end(), '\r'), workingStr.end());
						vector.push_back(workingStr);
					}
				}
			}

			if (!vector.empty())
			{
				ret = true;
			}
		}

		return ret;
	}

	bool GetVectorByToken(const std::wstring& input, const wchar_t token, std::vector<std::wstring> &vector)
	{
		bool ret = false;
		std::wstringstream str(input);
		std::wstring line;

		vector.clear();

		while (std::getline(str, line, token))
		{
			vector.push_back(line);
		}

		if (!vector.empty())
		{
			ret = true;
		}

		return ret;
	}

	bool IsElementPresentOnList(const std::vector<std::wstring> &listToCheck, const std::wstring &element)
	{
		bool ret = false;

		if ((listToCheck.size() > 0) && (element.empty()))
		{
			for (auto it = listToCheck.begin(); it != listToCheck.end(); ++it)
			{
				const std::wstring &elementToCompare(*it);

				if ((!elementToCompare.empty()) && (elementToCompare.compare(element) == 0))
				{
					ret = true;
					break;
				}
			}
		}

		return ret;
	}


	template <typename StringBuffer>
	void InternalTrimSpaces(StringBuffer& buffer)
	{
		buffer.erase(remove_if(buffer.begin(), buffer.end(), isspace), buffer.end());
	}

	bool TrimSpaces(std::string &str)
	{
		bool ret = false;

		if (!str.empty())
		{
			InternalTrimSpaces(str);
			ret = true;
		}

		return ret;
	}

	bool TrimSpaces(std::wstring &str)
	{
		bool ret = false;

		if (!str.empty())
		{
			InternalTrimSpaces(str);
			ret = true;
		}

		return ret;
	}

	bool GetProcessnameByPID(DWORD pid, std::wstring &processName)
	{
		bool ret = false;
		//todo fix this

		return ret;
	}

	bool WStringFromVector(const CommonTypes::WCharContainer& source, std::wstring& target)
	{
		bool ret = false;

		if (!source.empty())
		{
			target.assign(source.begin(), source.end());

			if (!target.empty())
			{
				ret = true;
			}
		}

		return ret;
	}

	bool WStringToVector(const std::wstring& source, CommonTypes::WCharContainer& target)
	{
		bool ret = false;

		if (!source.empty())
		{
			CommonTypes::WCharContainer workingChars(source.begin(), source.end());

			if (!workingChars.empty())
			{
				target.swap(workingChars);
				ret = true;
			}
		}

		return ret;
	}

	std::wstring WStringFromVector(const CommonTypes::WCharContainer& source)
	{
		std::wstring target;

		if (!source.empty())
		{
			WStringFromVector(source, target);
		}

		return target;
	}

	CommonTypes::WCharContainer WStringToVector(const std::wstring& source)
	{
		CommonTypes::WCharContainer target;

		if (!source.empty())
		{
			WStringToVector(source, target);
		}

		return target;
	}

	CommonTypes::SerializedWCharVector GetSerializedVector(const std::wstring& source)
	{
		CommonTypes::SerializedWCharVector ret;

		if (!source.empty())
		{
			for (unsigned int it = 0; it < source.size(); it++)
			{
				ret.push_back(source[it]);
			}
		}

		return ret;
	}

	CommonTypes::SerializedCharVector GetSerializedVector(const std::string& source)
	{
		CommonTypes::SerializedCharVector ret;

		if (!source.empty())
		{
			for (unsigned int it = 0; it < source.size(); it++)
			{
				ret.push_back(source[it]);
			}
		}

		return ret;
	}

	CommonTypes::StringsContainer GetSerializedStringVector(const CommonTypes::SerializedCistaStringVector& source)
	{
		CommonTypes::StringsContainer ret;

		for (uint32_t it = 0; it < source.size(); it++)
		{
			std::string workString(source[it].data());
			ret.push_back(workString);
		}

		return ret;
	}

	CommonTypes::SerializedCistaStringVector GetSerializedStringVector(const CommonTypes::StringsContainer& source)
	{
		CommonTypes::SerializedCistaStringVector ret;

		if (!source.empty())
		{
			for (auto it = source.begin(); it != source.end(); it++)
			{
				std::string workIT(*it);
				if (!workIT.empty())
				{
					cista::offset::string workString(workIT.c_str());
					ret.push_back(workString);
				}
			}
		}

		return ret;
	}

	std::wstring GetSerializedWString(const CommonTypes::SerializedWCharVector& source)
	{
		std::wstring ret;

		if (!source.empty())
		{
			ret.assign(source.begin(), source.end());
		}

		return ret;
	}


	std::string GetRandomString(const unsigned int length)
	{
		std::string ret;
		const std::string DEFAULTCHARS = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

		if (length < MAX_PATH)
		{
			std::string randomStr;
			std::mt19937_64 gen{ std::random_device()() };
			std::uniform_int_distribution<size_t> dist{ 0, DEFAULTCHARS.length() - 1 };
			std::generate_n(std::back_inserter(randomStr), length, [&] { return DEFAULTCHARS[dist(gen)]; });

			if (!randomStr.empty())
			{
				ret.assign(randomStr);
			}
		}

		return ret;
	}

	std::string GetSerializedString(const CommonTypes::SerializedCharVector& source)
	{
		std::string ret;

		if (!source.empty())
		{
			ret.assign(source.begin(), source.end());
		}

		return ret;
	}

	std::string GetSerializedString(const CommonTypes::SerializedWCharVector& source)
	{
		std::string ret;

		if (!source.empty())
		{
			std::wstring workString;
			workString.assign(source.begin(), source.end());
			if (!workString.empty())
			{
				ret = GeneralHelpers::WStrToStr(workString);
			}			
		}

		return ret;
	}



	CommonTypes::CharContainer GetSerializedVector(const CommonTypes::SerializedCharVector& source)
	{
		CommonTypes::CharContainer ret;

		if (!source.empty())
		{
			ret.assign(source.begin(), source.end());
		}

		return ret;
	}

	CommonTypes::WCharContainer GetSerializedVector(const CommonTypes::SerializedWCharVector& source)
	{
		CommonTypes::WCharContainer ret;

		if (!source.empty())
		{
			ret.assign(source.begin(), source.end());
		}

		return ret;
	}

	CommonTypes::SerializedUIntVector GetSerializedVector(const CommonTypes::UIntContainer& source)
	{
		CommonTypes::SerializedUIntVector ret;

		if (!source.empty())
		{
			for (unsigned int it = 0; it < source.size(); it++)
			{
				ret.push_back(source[it]);
			}
		}

		return ret;
	}

	CommonTypes::UIntContainer GetSerializedVector(const CommonTypes::SerializedUIntVector& source)
	{
		CommonTypes::UIntContainer ret;

		if (!source.empty())
		{
			ret.assign(source.begin(), source.end());
		}

		return ret;
	}

	bool DPAPIEncrypt(const std::string& input, const std::string& entropy, std::string &output)
	{
		bool ret = false;

		if (!input.empty())
		{
			DATA_BLOB dataBlobInput = { 0 };
			DATA_BLOB dataBlobEntropy = { 0 };
			DATA_BLOB dataBlobOutput = { 0 };
			boost::shared_ptr<void> dataBlobOutputPtr(dataBlobOutput.pbData, ::LocalFree);

			// Encryption flags
			DWORD encryptionFlags = CRYPTPROTECT_UI_FORBIDDEN | CRYPTPROTECT_LOCAL_MACHINE;

			//preparing input
			dataBlobInput.pbData = reinterpret_cast<BYTE *>(const_cast<char*>(input.c_str()));
			dataBlobInput.cbData = (DWORD) (input.length() + 1);

			//preparing entropy
			dataBlobEntropy.pbData = reinterpret_cast<BYTE *>(const_cast<char*>(entropy.c_str()));
			dataBlobEntropy.cbData = (DWORD)(entropy.length() + 1);


			if (CryptProtectData(&dataBlobInput, NULL, &dataBlobEntropy, NULL, NULL, encryptionFlags, &dataBlobOutput) &&
				(dataBlobOutput.cbData > 0))
			{
				//Reserve array of dataBlobOutput and copy dataBlobOutput result to it
				CommonTypes::ByteContainer returnData(dataBlobOutput.cbData);
				memcpy_s(&*returnData.begin(), returnData.size(), dataBlobOutput.pbData, dataBlobOutput.cbData);

				output.assign(returnData.begin(), returnData.end());

				if (!output.empty())
				{
					ret = true;
				}
				else
				{
					output.clear();
				}
			}
		}

		return ret;
	}

	bool DPAPIDecrypt(const std::string& input, const std::string& entropy, std::string &output)
	{
		bool ret = false;

		if (!input.empty())
		{
			std::string workingDecodedInput(input);

			DATA_BLOB dataBlobInput = { 0 };
			DATA_BLOB dataBlobEntropy = { 0 };
			DATA_BLOB dataBlobOutput = { 0 };
			boost::shared_ptr<void> dataBlobOutputPtr(dataBlobOutput.pbData, ::LocalFree);

			//preparing input
			dataBlobInput.pbData = reinterpret_cast<BYTE *>(const_cast<char*>(workingDecodedInput.c_str()));
			dataBlobInput.cbData = (DWORD)(workingDecodedInput.length() + 1);

			// Decryption flags
			DWORD decryptionFlags = CRYPTPROTECT_UI_FORBIDDEN;

			//preparing entropy
			dataBlobEntropy.pbData = reinterpret_cast<BYTE *>(const_cast<char*>(entropy.c_str()));
			dataBlobEntropy.cbData = (DWORD)(entropy.length() + 1);

			if (CryptUnprotectData(&dataBlobInput, NULL, &dataBlobEntropy, NULL, NULL, decryptionFlags, &dataBlobOutput) &&
				(dataBlobOutput.cbData > 0))
			{
				//Reserve array of dataBlobOutput and copy dataBlobOutput result to it
				CommonTypes::ByteContainer returnData(dataBlobOutput.cbData);
				memcpy_s(&*returnData.begin(), returnData.size(), dataBlobOutput.pbData, dataBlobOutput.cbData);

				output.assign(returnData.begin(), returnData.end());

				if (!output.empty())
				{
					ret = true;
				}
				else
				{
					output.clear();
				}
			}
		}

		return ret;
	}

	bool DPAPIEncrypt(const CommonTypes::ByteContainer& input, const std::string& entropy, CommonTypes::ByteContainer &output)
	{
		bool ret = false;

		if (!input.empty())
		{
			DATA_BLOB dataBlobInput = { 0 };
			DATA_BLOB dataBlobEntropy = { 0 };
			DATA_BLOB dataBlobOutput = { 0 };
			boost::shared_ptr<void> dataBlobOutputPtr(dataBlobOutput.pbData, ::LocalFree);

			// Encryption flags
			DWORD encryptionFlags = CRYPTPROTECT_UI_FORBIDDEN | CRYPTPROTECT_LOCAL_MACHINE;
			
			//preparing input
			dataBlobInput.pbData = reinterpret_cast<BYTE *>(const_cast<BYTE*>(input.data()));
			dataBlobInput.cbData = (DWORD)(input.size());

			//preparing entropy
			dataBlobEntropy.pbData = reinterpret_cast<BYTE *>(const_cast<char*>(entropy.c_str()));
			dataBlobEntropy.cbData = (DWORD)(entropy.length() + 1);


			if (CryptProtectData(&dataBlobInput, NULL, &dataBlobEntropy, NULL, NULL, encryptionFlags, &dataBlobOutput) &&
				(dataBlobOutput.cbData > 0))
			{
				//Reserve array of dataBlobOutput and copy dataBlobOutput result to it
				CommonTypes::ByteContainer returnData(dataBlobOutput.cbData);
				memcpy_s(&*returnData.begin(), returnData.size(), dataBlobOutput.pbData, dataBlobOutput.cbData);
				output.assign(returnData.begin(), returnData.end());

				if (!output.empty())
				{
					ret = true;
				}
				else
				{
					output.clear();
				}
			}
		}

		return ret;
	}

	bool DPAPIDecrypt(const CommonTypes::ByteContainer& input, const std::string& entropy, CommonTypes::ByteContainer &output)
	{
		bool ret = false;

		if (!input.empty())
		{
			DATA_BLOB dataBlobInput = { 0 };
			DATA_BLOB dataBlobEntropy = { 0 };
			DATA_BLOB dataBlobOutput = { 0 };
			boost::shared_ptr<void> dataBlobOutputPtr(dataBlobOutput.pbData, ::LocalFree);

			//preparing input
			dataBlobInput.pbData = reinterpret_cast<BYTE *>(const_cast<BYTE*>(input.data()));
			dataBlobInput.cbData = (DWORD)(input.size());

			// Decryption flags
			DWORD decryptionFlags = CRYPTPROTECT_UI_FORBIDDEN;

			//preparing entropy
			dataBlobEntropy.pbData = reinterpret_cast<BYTE *>(const_cast<char*>(entropy.c_str()));
			dataBlobEntropy.cbData = (DWORD)(entropy.length() + 1);

			if (CryptUnprotectData(&dataBlobInput, NULL, &dataBlobEntropy, NULL, NULL, decryptionFlags, &dataBlobOutput) &&
				(dataBlobOutput.cbData > 0))
			{
				//Reserve array of dataBlobOutput and copy dataBlobOutput result to it
				CommonTypes::ByteContainer returnData(dataBlobOutput.cbData);
				memcpy_s(&*returnData.begin(), returnData.size(), dataBlobOutput.pbData, dataBlobOutput.cbData);

				output.assign(returnData.begin(), returnData.end());

				if (!output.empty())
				{
					ret = true;
				}
				else
				{
					output.clear();
				}
			}
		}

		return ret;
	}


	bool UnzipZIPFileToFolder(const std::wstring &zipFile, const std::wstring &targetFolder)
	{
		bool ret = false;

		try
		{
			if (!zipFile.empty() &&
				!targetFolder.empty() &&
				IsValidFile(zipFile) &&
				IsValidDirectory(targetFolder))
			{
				HRESULT hCOMInit = ::CoInitialize(NULL);
				if (SUCCEEDED(hCOMInit))
				{
					CComPtr<IShellDispatch> spISD = nullptr;
					HRESULT hCoCreateInstance = spISD.CoCreateInstance(CLSID_Shell);
					if (SUCCEEDED(hCoCreateInstance))
					{
						CComVariant vtZipFile(zipFile.c_str());
						CComPtr<Folder> spZipFile = nullptr;
						HRESULT hOriginatorNameSpace = spISD->NameSpace(vtZipFile, &spZipFile);
						if (SUCCEEDED(hOriginatorNameSpace))
						{
							CComVariant vtDestFolder(targetFolder.c_str());
							CComPtr<Folder> spDestination = nullptr;
							HRESULT hDestinationFolder = spISD->NameSpace(vtDestFolder, &spDestination);
							if (SUCCEEDED(hDestinationFolder) && spDestination)
							{
								CComPtr<FolderItems> spFilesInside = nullptr;
								HRESULT hITems = spZipFile->Items(&spFilesInside);
								if (SUCCEEDED(hITems))
								{
									CComPtr<IDispatch> spDispItem = nullptr;
									HRESULT hQueryInterface = spFilesInside.QueryInterface(&spDispItem);
									if (SUCCEEDED(hQueryInterface))
									{
										CComVariant vtItem(spDispItem);
										CComVariant vtOptions(FOF_NO_UI);

										HRESULT hCopyOperation = spDestination->CopyHere(vtItem, vtOptions);
										if (SUCCEEDED(hCopyOperation))
										{
											ret = true;
										}
									}
								}
							}
						}
					}

					CoUninitialize();
				}
			}

		}
		catch (...)
		{
			ret = false;
		}

		return ret;
	}

	//bool DownloadFile(const std::wstring &targetHost, const std::wstring &targetURLPath, const std::wstring &proxyToUse,
	//	const std::wstring &proxyUsername, const std::wstring &proxyPassword, const std::wstring &targetFile)
	bool DownloadFile(const std::wstring &targetHost, const std::wstring &targetURLPath, const CommonTypes::ProxyConfData &proxyData, const std::wstring &targetFile)
	{
		bool ret = false;
		static const std::wstring SYSMONX_UA = L"SysmonXUserAgent";
		static const std::wstring DEFAULT_HTTP_PROTOCOL = L"HTTP/1.1";
		static const std::wstring DEFAULT_HTTP_OPERATION = L"GET";
		HINTERNET hOpen = NULL;
		HINTERNET hConnect = NULL;
		HINTERNET hReq = NULL;

		try
		{
			if (!targetHost.empty() && !targetURLPath.empty() && !targetFile.empty())
			{
				std::ofstream outputFile(targetFile, std::ofstream::out | std::ofstream::binary);

				if (outputFile.is_open())
				{
					std::wstring workingProxy;

					//Configuring Proxy
					DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
					if (!proxyData.proxy.empty())
					{
						dwAccessType = INTERNET_OPEN_TYPE_PROXY;
						workingProxy.assign(proxyData.proxy);
						workingProxy.assign(L":");
						workingProxy.assign(proxyData.port);
					}

					//Initializing the connection
					hOpen = InternetOpenW(SYSMONX_UA.c_str(), dwAccessType, workingProxy.c_str(), NULL, 0);
					if (hOpen)
					{

						//Setting username and password in case proxy was requested
						if (dwAccessType == INTERNET_OPEN_TYPE_PROXY)
						{
							if (!proxyData.username.empty())
							{
								::InternetSetOptionExW(hOpen, INTERNET_OPTION_PROXY_USERNAME, (LPVOID)proxyData.username.c_str(), (DWORD)proxyData.username.length(), 0);
							}

							if (!proxyData.password.empty())
							{
								::InternetSetOptionExW(hOpen, INTERNET_OPTION_PROXY_PASSWORD, (LPVOID)proxyData.password.c_str(), (DWORD)proxyData.password.length(), 0);
							}
						}

						//Connecting to the host
						//TODO: mjo switch to INTERNET_DEFAULT_HTTPS_PORT
						hConnect = InternetConnectW(hOpen, targetHost.c_str(), INTERNET_DEFAULT_HTTP_PORT, NULL, DEFAULT_HTTP_PROTOCOL.c_str(), INTERNET_SERVICE_HTTP, 0, 0);
						//hConnect = InternetConnectW(hOpen, targetHost.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, DEFAULT_HTTP_PROTOCOL.c_str(), INTERNET_SERVICE_HTTP, 0, 0);
						if (hConnect)
						{

							//Sending the connection request to the host
							//hReq = HttpOpenRequestW(hConnect, DEFAULT_HTTP_OPERATION.c_str(), targetURLPath.c_str(), DEFAULT_HTTP_PROTOCOL.c_str(), NULL, NULL, INTERNET_FLAG_SECURE, 0);
							hReq = HttpOpenRequestW(hConnect, DEFAULT_HTTP_OPERATION.c_str(), targetURLPath.c_str(), DEFAULT_HTTP_PROTOCOL.c_str(), NULL, NULL, SECURITY_IGNORE_ERROR_MASK, 0);							
							if (hReq && HttpSendRequestW(hReq, NULL, 0, NULL, 0))
							{
								//TODO: Use HttpQueryInfoA(hInternet, HTTP_QUERY_CONTENT_LENGTH...) to get the target file size and provide download feedback
								//Getting target file size

								DWORD dwByteRead = 0;
								do
								{
									char szBuffer[1024] = { 0 };
									if (InternetReadFile(hReq, szBuffer, ARRAYSIZE(szBuffer), &dwByteRead))
									{
										outputFile.write(szBuffer, dwByteRead);
									}

								} while (dwByteRead);

								ret = true;
							}
						}

						if (hReq) InternetCloseHandle(hReq);
						if (hConnect) InternetCloseHandle(hConnect);
						if (hOpen) InternetCloseHandle(hOpen);
					}
				}

				outputFile.close();
			}
		}
		catch (...)
		{
			ret = false;
		}

		return ret;
	}

	bool RenameFile(const std::wstring &oldfile, const std::wstring &newfile)
	{
		bool ret = false;

		//Target file exists! trying deleting it first
		if ((oldfile.compare(newfile) != 0) && IsValidFile(newfile))
		{
			if (!RemoveFile(newfile))
			{
				return ret;
			}
		}

		if (!oldfile.empty() &&
			!newfile.empty() &&
			IsValidFile(oldfile) &&
			MoveFile(oldfile.c_str(), newfile.c_str()))
		{
			ret = true;
		}

		return ret;
	}

	bool RemoveDeletePending(const std::wstring &fileToRemove)
	{
		bool ret = false;

		if (!fileToRemove.empty() && IsValidFile(fileToRemove))
		{
			//Flagging the file as delete pending if we cannot remove it
			if (MoveFileEx(fileToRemove.c_str(), 0, MOVEFILE_DELAY_UNTIL_REBOOT))
			{
				ret = true;
			}
		}

		return ret;
	}

	bool RemoveFile(const std::wstring &fileToRemove)
	{
		bool ret = false;

		if (!fileToRemove.empty())
		{
			if (IsValidFile(fileToRemove))
			{
				if (DeleteFile(fileToRemove.c_str()))
				{
					ret = true;
				}
				else
				{
					if (RemoveDeletePending(fileToRemove))
					{
						ret = true;
					}
				}
			}
			else
			{
				//file does not exists already, returning true
				ret = true;
			}
		}

		return ret;
	}

	bool GetTargetFileSize(const std::wstring& targetFile, size_t &targetFileSize)
	{
		bool ret = false;

		if (!targetFile.empty() && IsValidFile(targetFile))
		{
			HANDLE hFile = CreateFileW(targetFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				targetFileSize = (size_t)GetFileSize(hFile, NULL);
				ret = true;

				CloseHandle(hFile);
			}
		}

		return ret;
	}

	bool GetTargetFileIntoBuffer(const std::wstring& targetFile, const size_t &targetFileSize, LPVOID lpBuffer, DWORD &bytesRead)
	{
		bool ret = false;

		if (!targetFile.empty() && 
			IsValidFile(targetFile) &&
			(lpBuffer != nullptr))
		{
			HANDLE hFile = CreateFileW(targetFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				if (ReadFile(hFile, lpBuffer, (DWORD)targetFileSize, &bytesRead, NULL) && (bytesRead > 0))
				{
					ret = true;
				}

				CloseHandle(hFile);
			}
		}

		return ret;
	}


	template <typename StringBuffer>
	bool InternalReadBuffer(const std::wstring& fullPathFile, StringBuffer& buffer)
	{
		bool ret = false;

		if (IsValidFile(fullPathFile))
		{
			std::basic_ifstream<typename StringBuffer::value_type> inputFile(fullPathFile, std::ios_base::binary);

			if (inputFile.good())
			{
				StringBuffer workingContent((std::istreambuf_iterator<typename StringBuffer::value_type>(inputFile)), {});

				if (!workingContent.empty())
				{
					buffer.assign(workingContent);
					ret = true;
				}
			}

			inputFile.close();
		}

		return ret;
	}

	bool GetTargetFileIntoString(const std::wstring& fullPathFile, std::wstring& buffer)
	{
		return InternalReadBuffer(fullPathFile, buffer);
	}

	bool GetTargetFileIntoString(const std::wstring& fullPathFile, std::string& buffer)
	{
		return InternalReadBuffer(fullPathFile, buffer);
	}

	bool GetUserProfileDirectory(std::wstring &userProfileDir)
	{
		bool ret = false;

		WCHAR workingPath[MAX_PATH + 1] = { 0 };

		if (SHGetSpecialFolderPathW(NULL, workingPath, CSIDL_APPDATA, FALSE))
		{
			userProfileDir.assign(workingPath);
			ret = true;
		}

		return ret;
	}

	bool GetRandomString(const unsigned int length, std::string &randomStr)
	{
		bool ret = false;
		const std::string DEFAULTCHARS = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

		if (length < MAX_PATH)
		{
			std::mt19937_64 gen{ std::random_device()() };
			std::uniform_int_distribution<size_t> dist{ 0, DEFAULTCHARS.length() - 1 };
			std::generate_n(std::back_inserter(randomStr), length, [&] { return DEFAULTCHARS[dist(gen)]; });

			if (!randomStr.empty())
			{
				ret = true;
			}
		}

		return ret;
	}

	bool CreateNewDirectory(const std::wstring& newDirectory, LPSECURITY_ATTRIBUTES &securityAttributes)
	{
		bool ret = false;

		if (!newDirectory.empty() &&
			CreateDirectoryW(newDirectory.c_str(), securityAttributes) &&
			IsValidDirectory(newDirectory.c_str()))
		{
			ret = true;
		}

		return ret;
	}

	bool RemoveWorkingDirectory(const std::wstring& newDirectory)
	{
		bool ret = false;

		if (!newDirectory.empty() &&
			IsValidDirectory(newDirectory.c_str()) &&
			RemoveDirectory(newDirectory.c_str()))
		{
			ret = true;
		}

		return ret;

	}

	bool GetTemporaryDirLocation(std::wstring &newPath, unsigned int lengthRandomDir)
	{
		bool ret = false;
		wchar_t workingTempPath[MAX_PATH] = { 0 };
		std::string randomString;
		std::wstring tempPath;

		//TODO: update security attributes to make the dir write/read only by running user
		LPSECURITY_ATTRIBUTES lpSecurityAttributes = { 0 };

		if ((lengthRandomDir > 0) &&
			GetTempPathW(MAX_PATH, workingTempPath))
		{
			tempPath.assign(workingTempPath);

			if (!tempPath.empty() && GetRandomString(lengthRandomDir, randomString))
			{
				tempPath.append(StrToWStr(randomString));

				if ((tempPath.length() < MAX_PATH) &&
					CreateNewDirectory(tempPath.c_str(), lpSecurityAttributes) &&
					IsValidDirectory(tempPath.c_str()))
				{
					GeneralHelpers::AddPathTrailCharsIfNeeded(tempPath);
					newPath.assign(tempPath);
					ret = true;
				}
			}
		}

		return ret;
	}
	
	bool GetAdminSecuredDirLocation(std::wstring &newPath, unsigned int lengthRandomDir)
	{
		bool ret = false;
		wchar_t workingTempPath[MAX_PATH] = { 0 };
		std::string randomString;
		std::wstring tempPath;

		std::wstring targetDACL = L"D:P(A;;GA;;;SY)(A;;GA;;;BA)";   //RWX access only to Built-in administrators and System
		//std::wstring targetDACL = L"D:P(A;;GA;;;SY)(A;;GA;;;BA)(A;;GA;;;LA)";   //RWX access only to Built-in administrators, Local Administrators and System

		if (GetTempPathW(MAX_PATH, workingTempPath))
		{
			tempPath.assign(workingTempPath);

			if (!tempPath.empty() && GetRandomString(lengthRandomDir, randomString))
			{
				tempPath.append(StrToWStr(randomString));

				SECURITY_ATTRIBUTES secureAttributes = { 0 };
				secureAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
				secureAttributes.bInheritHandle = FALSE;

				if (ConvertStringSecurityDescriptorToSecurityDescriptor(
					targetDACL.c_str(),
					SDDL_REVISION_1,
					&(secureAttributes.lpSecurityDescriptor),
					NULL))
				{
					if ((tempPath.length() < MAX_PATH) &&
						CreateDirectoryW(tempPath.c_str(), &secureAttributes) &&
						IsValidDirectory(tempPath.c_str()))
					{
						GeneralHelpers::AddPathTrailCharsIfNeeded(tempPath);
						newPath.assign(tempPath);
						ret = true;
					}

					// Free the memory allocated for the SECURITY_DESCRIPTOR.
					if (secureAttributes.lpSecurityDescriptor)
					{
						LocalFree(secureAttributes.lpSecurityDescriptor);
					}
				}
			}
		}

		return ret;
	}

	bool GetCommonAppDataPath(std::wstring &commonPath)
	{
		bool ret = false;
		wchar_t workingCommonAppData[MAX_PATH] = { 0 };

		if ((SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, workingCommonAppData) == S_OK) &&
			IsValidDirectory(workingCommonAppData))
		{
			commonPath.assign(workingCommonAppData);

			GeneralHelpers::AddPathTrailCharsIfNeeded(commonPath);

			ret = true;
		}

		return ret;
	}

	bool GetSysmonXCommonPath(std::wstring &sysmonXCommonPath)
	{
		bool ret = false;
		std::wstring workingPath;
		const std::wstring SYSMONX_FOLDER_NAME = L"SysmonX";
		if (GetCommonAppDataPath(workingPath) &&
			!workingPath.empty())
		{
			GeneralHelpers::AddPathTrailCharsIfNeeded(workingPath);
			workingPath.append(SYSMONX_FOLDER_NAME);

			SECURITY_ATTRIBUTES secureAttributes = { 0 };
			secureAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
			secureAttributes.bInheritHandle = FALSE;

			GeneralHelpers::AddPathTrailCharsIfNeeded(workingPath);

			if (IsValidDirectory(workingPath))
			{
				sysmonXCommonPath.assign(workingPath);
				ret = true;
			}
			else
			{
				if (CreateDirectoryW(workingPath.c_str(), &secureAttributes))
				{
					if (IsValidDirectory(workingPath))
					{
						sysmonXCommonPath.assign(workingPath);
						ret = true;
					}
				}
			}
		}

		return ret;
	}

	bool GetNewSysmonXRandomDirectoryName(const std::wstring &sysmonXCommonPath, std::wstring &newSysmonXDirectory)
	{
		bool ret = false;
		std::wstring workingPath;
		const unsigned int RANDOM_STRING_LENGTH = 12;
		std::string randomString;

		if (!sysmonXCommonPath.empty() &&
			IsValidDirectory(sysmonXCommonPath) &&
			GetRandomString(RANDOM_STRING_LENGTH, randomString) &&
			!randomString.empty())
		{
			workingPath.assign(sysmonXCommonPath);

			GeneralHelpers::AddPathTrailCharsIfNeeded(workingPath);

			workingPath.append(StrToWStr(randomString));

			GeneralHelpers::AddPathTrailCharsIfNeeded(workingPath);

			if (!workingPath.empty())
			{
				newSysmonXDirectory.assign(workingPath);
				ret = true;
			}
		}

		return ret;
	}

	//TODO: Review DACL below
	bool GetNewSecureSysmonXDirectory(std::wstring &newSecureDirectory)
	{
		bool ret = false;
		std::wstring commonSysmonXPath;
		std::wstring workingSysmonXPath;

		std::wstring targetDACL;
		//std::wstring targetDACL = L"D:P(A;;GA;;;SY)(A;;GA;;;BA)";   //RWX access only to Built-in administrators and System
		//std::wstring targetDACL = L"D:P(A;;GA;;;SY)(A;;GA;;;BA)(A;;GA;;;LA)";   //RWX access only to Built-in administrators, Local Administrators and System

		if (GetSysmonXCommonPath(commonSysmonXPath) &&
			IsValidDirectory(commonSysmonXPath) &&
			GetNewSysmonXRandomDirectoryName(commonSysmonXPath, workingSysmonXPath))
		{
			SECURITY_ATTRIBUTES secureAttributes = { 0 };
			secureAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
			secureAttributes.bInheritHandle = FALSE;

			if (ConvertStringSecurityDescriptorToSecurityDescriptor(
				targetDACL.c_str(),
				SDDL_REVISION_1,
				&(secureAttributes.lpSecurityDescriptor),
				NULL))
			{
				if ((workingSysmonXPath.length() < MAX_PATH) &&
					CreateDirectoryW(workingSysmonXPath.c_str(), &secureAttributes) &&
					IsValidDirectory(workingSysmonXPath))
				{
					GeneralHelpers::AddPathTrailCharsIfNeeded(workingSysmonXPath);

					if (IsValidDirectory(workingSysmonXPath))
					{
						newSecureDirectory.assign(workingSysmonXPath);
						ret = true;
					}
				}
				else
				{
					newSecureDirectory.clear();
				}

				// Free the memory allocated for the SECURITY_DESCRIPTOR.
				if (secureAttributes.lpSecurityDescriptor)
				{
					LocalFree(secureAttributes.lpSecurityDescriptor);
				}
			}
		}

		return ret;
	}

	bool GetTemporaryFileName(const std::wstring &tempDirectory, const std::wstring &tempFileExtension, std::wstring &tempFileName, unsigned int lengthRandomDir)
	{
		bool ret = false;
		wchar_t buffTempFile[MAX_PATH] = { 0 };
		std::wstring workingTempFile;
		std::string randomString;

		if (!tempDirectory.empty() &&
			!tempFileExtension.empty() &&
			lengthRandomDir > 0)
		{			
			workingTempFile.assign(tempDirectory);
			//checking if it is properly postpended
			GeneralHelpers::AddPathTrailCharsIfNeeded(workingTempFile);
			
			if (!workingTempFile.empty() && GetRandomString(lengthRandomDir, randomString))
			{
				workingTempFile.append(StrToWStr(randomString));
				workingTempFile.append(tempFileExtension);

				if (!workingTempFile.empty() &&
					(workingTempFile.length() < MAX_PATH))
				{
					tempFileName.assign(workingTempFile);
					ret = true;
				}
				else
				{
					workingTempFile.clear();
				}
			}
		}

		return ret;
	}

	bool ConvertStringToHexRepresentation(const std::string &input, std::string &output)
	{
		bool ret = false;
		std::ostringstream convertedOS;
		std::string workingStr;
		for (size_t it = 0; it < input.length(); it++)
		{
			convertedOS << std::hex << std::uppercase << (int)input[it];
		}

		workingStr = convertedOS.str();

		if (!workingStr.empty())
		{
			output.assign(workingStr);
			ret = true;
		}

		return ret;
	}

	bool RunProcess(const std::wstring &process, const std::wstring &cmdlineArguments, CommonTypes::StringsContainer &processOutput, DWORD &exitCode, const bool createWindowMode)
	{
		bool ret = false;
		DWORD flags = 0;
		constexpr std::size_t BUFFERSIZE = 2048;
		constexpr std::size_t MAX_ITERATIONS = 2048;
		HANDLE stdOutRead = INVALID_HANDLE_VALUE;
		HANDLE stdOutWrite = INVALID_HANDLE_VALUE;
		SECURITY_ATTRIBUTES securityAttributes = { 0 };
		securityAttributes.nLength = sizeof(securityAttributes);
		securityAttributes.bInheritHandle = TRUE;
		securityAttributes.lpSecurityDescriptor = NULL;
		STARTUPINFO startupInfo = { 0 };
		PROCESS_INFORMATION processInfo = { 0 };
		DWORD procesCreateFlags = 0;

		if (CreatePipe(&stdOutRead, &stdOutWrite, &securityAttributes, BUFFERSIZE))
		{
			if (!SetHandleInformation(stdOutRead, HANDLE_FLAG_INHERIT, flags))
			{
				//"SetHandleInformation error 0x" << std::hex << GetLastError()
				return ret;
			}
		}
		else
		{
			//"CreatePipe error 0x" << std::hex << GetLastError()
			return ret;
		}

		startupInfo.cb = sizeof(startupInfo);
		startupInfo.hStdOutput = stdOutWrite;
		startupInfo.wShowWindow = SW_HIDE;
		if (!createWindowMode)
		{			
			startupInfo.dwFlags |= STARTF_USESTDHANDLES;			
			startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
			procesCreateFlags |= CREATE_NO_WINDOW;
		}
			
		std::wstring workingCMDLine;
		workingCMDLine.append(process);
		workingCMDLine.append(L" ");
		workingCMDLine.append(cmdlineArguments);

		//Forcing some idle time before using pipes
		Sleep(500);

		if (CreateProcessW(process.c_str(),
			((wchar_t *)workingCMDLine.c_str()),
			NULL,
			NULL,
			TRUE,
			procesCreateFlags,
			NULL,
			NULL,
			&startupInfo,
			&processInfo))
		{
			// Successfully created the process.  Waiting for it to finish.
			WaitForSingleObject(processInfo.hProcess, INFINITE);

			//Forcing some idle time before peeking into pipes
			Sleep(500);
			char buffer[BUFFERSIZE] = { 0 };
			std::vector<std::string> workContainer;
			DWORD bytesRead = 0;
			DWORD workingExitCode = STILL_ACTIVE;
			size_t iterations = 0;
			while ((workingExitCode == STILL_ACTIVE) && (iterations < MAX_ITERATIONS))
			{
				DWORD bytesOnPipe = 0;

				GetExitCodeProcess(processInfo.hProcess, &workingExitCode);
				
				if (PeekNamedPipe(stdOutRead, nullptr, 0, nullptr, &bytesOnPipe, nullptr))
				{
					if (bytesOnPipe > 0)
					{
						auto readStatus = ReadFile(stdOutRead, buffer, BUFFERSIZE - 1, &bytesRead, nullptr);
						if (readStatus && bytesRead > 0)
						{
							std::string workStr(buffer);
							std::vector<std::string> splittedStr;
							if (!workStr.empty())
							{							
								if (GetVectorByWindowsCRToken(workStr, splittedStr))
								{
									for (auto itStr = splittedStr.begin(); itStr != splittedStr.end(); itStr++)
									{
										std::string workITstr(*itStr);
										if (!workITstr.empty())
										{
											processOutput.push_back(workITstr);
										}									
									}

									ret = true;
								}
								else
								{
									processOutput.push_back(workStr);
								}								
							}
						}
					}
					else
					{
						break;
					}
				}
				else
				{					
					break;
				}

				iterations++;
			}

			exitCode = workingExitCode;			
		}
		else
		{
			//"CreateProcess error 0x" << std::hex << GetLastError()
			return ret;
		}

		if (stdOutRead != INVALID_HANDLE_VALUE)	CloseHandle(stdOutRead);
		if (stdOutWrite != INVALID_HANDLE_VALUE) CloseHandle(stdOutWrite);
		if (processInfo.hProcess != INVALID_HANDLE_VALUE) CloseHandle(processInfo.hProcess);
		if (processInfo.hThread != INVALID_HANDLE_VALUE) CloseHandle(processInfo.hThread);

		return ret;
	}

	bool GetCurrentProcessFullPath(std::wstring &currentProcesFullPath)
	{
		bool ret = false;
		wchar_t workFullPath[MAX_PATH + 1] = { 0 };

		if (GetModuleFileNameW(NULL, workFullPath, MAX_PATH) > 0)
		{
			currentProcesFullPath.assign(workFullPath);
			ret = true;
		}

		return ret;
	}

	bool FileCopy(const std::wstring &originFile, const std::wstring &destFile, bool overwrite)
	{
		bool ret = false;

		if (!originFile.empty() &&
			!destFile.empty() &&
			IsValidFile(originFile) &&
			CopyFileW(originFile.c_str(), destFile.c_str(), !overwrite))
		{
			ret = true;
		}

		return ret;
	}

	bool GetFileNameWithoutExtension(const std::wstring &filename, std::wstring &baseFileNameWoExtension)
	{
		bool ret = false;

		if (!filename.empty())
		{
			size_t lastindex = filename.find_last_of(L".");
			std::wstring workingBaseFileNameWoExtension = filename.substr(0, lastindex);

			if (!workingBaseFileNameWoExtension.empty())
			{
				baseFileNameWoExtension.assign(workingBaseFileNameWoExtension);
				ret = true;
			}
		}

return ret;
	}

	bool ComputeMD5FileHash(const std::wstring &filename, std::string &hash)
	{
		bool ret = false;
		static CHAR rgbDigits[] = "0123456789abcdef";
		constexpr DWORD BUFSIZE = 16;

		DWORD readCounter = 0;
		DWORD hashCounter = 0;
		BYTE rgbHash[BUFSIZE] = { 0 };
		BYTE workHash[BUFSIZE] = { 0 };
		DWORD status = 0;
		BOOL result = FALSE;
		HANDLE hFile = INVALID_HANDLE_VALUE;
		HCRYPTHASH hHash = NULL;
		HCRYPTPROV hProv = NULL;

		if (IsValidFile(filename))
		{
			hash.clear();

			hFile = CreateFile(filename.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_SEQUENTIAL_SCAN,
				NULL);

			if (INVALID_HANDLE_VALUE != hFile)
			{
				// Get handle to the crypto provider
				if (CryptAcquireContext(&hProv,
					NULL,
					NULL,
					PROV_RSA_FULL,
					CRYPT_VERIFYCONTEXT))
				{
					if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
					{
						while (result = ReadFile(hFile, workHash, BUFSIZE, &readCounter, NULL))
						{
							if (0 == readCounter)
							{
								break;
							}

							CryptHashData(hHash, workHash, readCounter, 0);
						}

						if (result)
						{
							hashCounter = BUFSIZE;

							if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &hashCounter, 0))
							{
								std::string workingHash;
								for (DWORD i = 0; i < hashCounter; i++)
								{
									workingHash.push_back(rgbDigits[rgbHash[i] >> 4]);
									workingHash.push_back(rgbDigits[rgbHash[i] & 0xf]);
								}

								if (!workingHash.empty())
								{
									hash.assign(workingHash);
									ret = true;
								}
							}

						}
					}
				}
			}
		}

		if (hHash) CryptDestroyHash(hHash);
		if (hProv) CryptReleaseContext(hProv, 0);
		if (hFile) CloseHandle(hFile);

		return ret;
	}

	bool AreFilesEqual(const std::wstring &file1, const std::wstring &file2)
	{
		bool ret = false;
		std::string hashFile1;
		std::string hashFile2;

		if (!file1.empty() &&
			!file2.empty() &&
			IsValidFile(file1) &&
			IsValidFile(file2) &&
			ComputeMD5FileHash(file1, hashFile1) &&
			ComputeMD5FileHash(file2, hashFile2) &&
			!hashFile1.empty() &&
			!hashFile2.empty())
		{
			if (hashFile1.compare(hashFile2) == 0)
			{
				ret = true;
			}			
		}

		return ret;
	}


	std::wstring GetCurrentProcessFullPath()
	{
		wchar_t workFullPath[MAX_PATH + 1] = { 0 };
		std::wstring ret;

		if (GetModuleFileNameW(NULL, workFullPath, MAX_PATH) > 0)
		{
			ret.assign(workFullPath);
		}

		return ret;
	}

	std::wstring GetBaseFileName(const std::wstring &fullPath)
	{
		std::wstring ret;

		if (!GetBaseFileName(fullPath, ret))
		{
			ret.clear();
		}
		return ret;
	}

	std::wstring ToWstring(const unsigned int &value)
	{
		std::wostringstream ss;
		ss << value;
		return ss.str();
	}

	std::wstring ReplaceTokensInStr(const std::wstring &str, const std::wstring &from, const std::wstring &to)
	{
		std::wstring ret(str);

		size_t start_pos = 0;
		while ((start_pos = ret.find(from, start_pos)) != std::wstring::npos)
		{
			ret.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}

		return ret;
	}

	std::wstring SanitizeStr(const std::wstring &str)
	{
		std::wstring ret;

		ret = ReplaceTokensInStr(str, L"\\", L"\\\\");

		return ret;
	}

	std::wstring StrToWStr(const std::string& str)
	{
		size_t strCount = str.length();
		int bytesToAllocate = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)strCount, NULL, 0);

		std::wstring ret(bytesToAllocate, 0);
		int wideCharsCount = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)strCount, &ret[0], bytesToAllocate);

		//TODO: add check for wideCharsCount == strCount
		return ret;
	}

	std::wstring GetHexString(PVOID addr)
	{
		std::wstring ret;
		std::wstringstream formatter;
		if (addr != NULL)
		{
			formatter << L"0x";
			formatter << std::hex << addr;
			ret.assign(formatter.str());
		}

		return ret;
	}

	std::wstring GetMemoryRegionType(DWORD id)
	{
		std::wstring ret;

		switch (id)
		{
		case SEC_COMMIT:
			ret.assign(L"SEC_COMMIT");
			break;
		case MEM_MAPPED:
			ret.assign(L"MEM_MAPPED");
			break;
		case SEC_FILE:
			ret.assign(L"SEC_FILE");
			break;
		case MEM_IMAGE:
			ret.assign(L"MEM_IMAGE");
			break;
		case MEM_PRIVATE:
			ret.assign(L"MEM_PRIVATE");
			break;
		default:
			break;
		}

		return ret;
	}

	std::wstring GetMemoryRegionState(DWORD id)
	{
		std::wstring ret;

		switch (id)
		{
		case MEM_COMMIT:
			ret.assign(L"MEM_COMMIT");
			break;
		case MEM_FREE:
			ret.assign(L"MEM_FREE");
			break;
		case MEM_RESERVE:
			ret.assign(L"MEM_RESERVE");
			break;
		default:
			break;
		}

		return ret;
	}

	std::wstring GetMemoryRegionProtection(DWORD id)
	{
		std::wstring ret;

		switch (id)
		{
		case PAGE_NOACCESS:
			ret.assign(L"PAGE_NOACCESS");
			break;
		case PAGE_READONLY:
			ret.assign(L"PAGE_READONLY");
			break;
		case PAGE_READWRITE:
			ret.assign(L"PAGE_READWRITE");
			break;
		case PAGE_WRITECOPY:
			ret.assign(L"PAGE_WRITECOPY");
			break;
		case PAGE_EXECUTE:
			ret.assign(L"PAGE_EXECUTE");
			break;
		case PAGE_EXECUTE_READ:
			ret.assign(L"PAGE_EXECUTE_READ");
			break;
		case PAGE_EXECUTE_READWRITE:
			ret.assign(L"PAGE_EXECUTE_READWRITE");
			break;
		case PAGE_EXECUTE_WRITECOPY:
			ret.assign(L"PAGE_EXECUTE_WRITECOPY");
			break;
		case PAGE_GUARD:
			ret.assign(L"PAGE_GUARD");
			break;
		case PAGE_NOCACHE:
			ret.assign(L"PAGE_NOCACHE");
			break;
		case PAGE_WRITECOMBINE:
			ret.assign(L"PAGE_WRITECOMBINE");
			break;
		default:
			break;
		}

		return ret;
	}

	std::wstring GetWstrFromInteger(UINT32 &value)
	{
		return std::to_wstring(value);
	}

	std::wstring GetFileNameWithoutExtension(const std::wstring &filename)
	{
		std::wstring ret;
		std::wstring workingStr;
		if (!filename.empty() && 
			GetFileNameWithoutExtension(filename, workingStr) &&
			!workingStr.empty())
		{
			ret.assign(workingStr);
		}

		return ret;
	}

	std::string WStrToStr(const std::wstring& wstr)
	{
		size_t strCount = wstr.length();
		int bytesToAllocate = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)strCount, NULL, 0, NULL, NULL);

		std::string ret(bytesToAllocate, 0);
		int w = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)strCount, &ret[0], bytesToAllocate, NULL, NULL);

		//TODO: add check for wideCharsCount == strCount
		return ret;
	}


	std::string GetErrorText(NTSTATUS code)
	{
		std::string ret;
		LPSTR lpStr = nullptr;

		if (FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_FROM_HMODULE |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			GetModuleHandleA("ntdll.dll"),
			code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpStr, 0, NULL) != 0)
		{
			ret.assign(lpStr);
			LocalFree(lpStr);
		}

		return ret;
	}

	void StrAddDelimitator(const wchar_t delim, std::wstring &str)
	{
		if (str.length() > 0)
		{
			wchar_t lastCharacter = *(str.rbegin());
			if (delim != lastCharacter)
			{
				str.push_back(delim);
			}
		}
	}

	void StrTrim(std::wstring &str)
	{
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	}

	void StrToUppercase(std::wstring &str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::towupper);
	}

	void StrToUppercase(std::string &str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
	}

	void StrToLowercase(std::wstring &str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::towlower);
	}

	void StrToLowercase(std::string &str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
	}

	void AddPathTrailCharsIfNeeded(std::wstring &path)
	{
		if (!path.empty() && path.back() != '\\')
		{
			path.push_back('\\');
		}
	}

	const std::time_t GetEpochTimestamp()
	{
		std::time_t result = std::time(nullptr);
		return result;
	}

	bool GetWStringFromGUID(const GUID &guidValue, std::wstring &value)
	{
		bool ret = false;
		value.clear();
		std::wstringstream newGuidStr;

		newGuidStr << std::uppercase;
		newGuidStr.width(8);
		newGuidStr << std::hex << guidValue.Data1 << L'-';
		newGuidStr.width(4);
		newGuidStr << std::hex << guidValue.Data2 << L'-';
		newGuidStr.width(4);
		newGuidStr << std::hex << guidValue.Data3 << L'-';
		newGuidStr.width(2);
		newGuidStr << std::hex
			<< static_cast<short>(guidValue.Data4[0])
			<< static_cast<short>(guidValue.Data4[1])
			<< L'-'
			<< static_cast<short>(guidValue.Data4[2])
			<< static_cast<short>(guidValue.Data4[3])
			<< static_cast<short>(guidValue.Data4[4])
			<< static_cast<short>(guidValue.Data4[5])
			<< static_cast<short>(guidValue.Data4[6])
			<< static_cast<short>(guidValue.Data4[7]);
		newGuidStr << std::nouppercase;

		if (newGuidStr.rdbuf()->in_avail())
		{
			value.assign(newGuidStr.str());
			ret = true;
		}

		return ret;
	}

	bool GetStringFromGUID(const GUID &guidValue, std::string &value)
	{
		bool ret = false;
		value.clear();
		std::stringstream newGuidStr;

		newGuidStr << std::uppercase;
		newGuidStr.width(8);
		newGuidStr << std::hex << guidValue.Data1 << '-';
		newGuidStr.width(4);
		newGuidStr << std::hex << guidValue.Data2 << '-';
		newGuidStr.width(4);
		newGuidStr << std::hex << guidValue.Data3 << '-';
		newGuidStr.width(2);
		newGuidStr << std::hex
			<< static_cast<short>(guidValue.Data4[0])
			<< static_cast<short>(guidValue.Data4[1])
			<< '-'
			<< static_cast<short>(guidValue.Data4[2])
			<< static_cast<short>(guidValue.Data4[3])
			<< static_cast<short>(guidValue.Data4[4])
			<< static_cast<short>(guidValue.Data4[5])
			<< static_cast<short>(guidValue.Data4[6])
			<< static_cast<short>(guidValue.Data4[7]);
		newGuidStr << std::nouppercase;

		if (newGuidStr.rdbuf()->in_avail())
		{
			value.assign(newGuidStr.str());
			ret = true;
		}

		return ret;
	}

	bool GetStringFromArray(const CommonTypes::ArrayBoolT &valueArray, std::wstring &stringArray)
	{
		bool ret = false;

		for (const bool& value : valueArray)
		{
			if (value)
			{
				stringArray.append(L"0");
			}
			else
			{
				stringArray.append(L"1");
			}
		}

		if (!stringArray.empty())
		{
			ret = true;
		}

		return ret;
	}

	bool GetStringFromArray(const CommonTypes::Array16T &valueArray, std::wstring &stringArray)
	{
		bool ret = false;

		for (const UINT16& value : valueArray)
		{
			std::wstring workValue;
			if (boost::conversion::try_lexical_convert(value, workValue))
			{
				stringArray.append(workValue);
			}
		}

		if (!stringArray.empty())
		{
			ret = true;
		}

		return ret;
	}

	bool GetStringFromArray(const CommonTypes::Array32T &valueArray, std::wstring &stringArray)
	{
		bool ret = false;

		for (const UINT32& value : valueArray)
		{
			std::wstring workValue;
			if (boost::conversion::try_lexical_convert(value, workValue))
			{
				stringArray.append(workValue);
			}
		}

		if (!stringArray.empty())
		{
			ret = true;
		}

		return ret;
	}

	bool GetStringFromArray(const CommonTypes::Array64T &valueArray, std::wstring &stringArray)
	{
		bool ret = false;

		for (const UINT64& value : valueArray)
		{
			std::wstring workValue;
			if (boost::conversion::try_lexical_convert(value, workValue))
			{
				stringArray.append(workValue);
			}
		}

		if (!stringArray.empty())
		{
			ret = true;
		}

		return ret;
	}
}

namespace RegistryHelpers
{
	bool OpenKey(const HKEY &hRootKey, const std::wstring &regSubKey, HKEY &hKey)
	{
		bool ret = false;

		if ((!regSubKey.empty()) && (hRootKey != NULL))
		{
			LSTATUS retCode = RegOpenKeyEx(hRootKey, regSubKey.c_str(), NULL, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
			if (retCode == ERROR_SUCCESS)
			{
				ret = true;
			}
			else if ((retCode == ERROR_FILE_NOT_FOUND) || (retCode == ERROR_PATH_NOT_FOUND))
			{
				if ((RegCreateKeyEx(hRootKey, regSubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY, NULL, &hKey, NULL)) == ERROR_SUCCESS)
				{
					ret = true;
				}
			}
		}

		return ret;
	}

	bool CloseKey(const HKEY &hKey)
	{
		bool ret = false;

		LSTATUS retCode = RegCloseKey(hKey);
		if (retCode == ERROR_SUCCESS)
		{
			ret = true;
		}

		return ret;
	}

	bool DeleteKey(const HKEY &hRootKey, const std::wstring &regSubKey)
	{
		bool ret = false;
		HKEY hKey = NULL;

		if ((!regSubKey.empty()) && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			LSTATUS retCode = RegDeleteKey(hKey, regSubKey.c_str());
			if ((retCode == ERROR_SUCCESS) && (CloseKey(hKey)))
			{
				ret = true;
			}

		}

		return ret;
	}

	bool CreateKey(const HKEY &hRootKey, const std::wstring &regSubKey)
	{
		bool ret = false;
		HKEY hKey = NULL;

		if ((!regSubKey.empty()) && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			ret = true;

			CloseKey(hKey);
		}

		return ret;
	}

	bool DeleteValue(const HKEY &hRootKey, const std::wstring &regSubKey, const std::wstring &regValue)
	{
		bool ret = false;
		HKEY hKey = NULL;

		if ((!regSubKey.empty()) && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			LSTATUS retCode = RegDeleteValueW(hKey, regValue.c_str());
			if ((retCode == ERROR_SUCCESS) && (CloseKey(hKey)))
			{
				ret = true;
			}

		}

		return ret;
	}

	bool RegistryKeyExists(const HKEY &hRootKey, const std::wstring &regSubKey)
	{
		bool ret = false;

		if ((!regSubKey.empty()) && (hRootKey != NULL))
		{
			HKEY hKey = NULL;
			LSTATUS retCode = RegOpenKeyEx(hRootKey, regSubKey.c_str(), NULL, KEY_READ | KEY_WOW64_64KEY, &hKey);
			if ((retCode == ERROR_SUCCESS) && (CloseKey(hKey)))
			{
				ret = true;
			}
		}

		return ret;
	}

	bool RegistryValueExists(const HKEY &hRootKey, const std::wstring &regSubKey, const std::wstring& regValue)
	{
		bool ret = false;

		if ((!regSubKey.empty()) && (hRootKey != NULL))
		{
			HKEY hKey = NULL;
			LSTATUS retCodeKey = RegOpenKeyEx(hRootKey, regSubKey.c_str(), NULL, KEY_READ | KEY_WOW64_64KEY, &hKey);
			if (retCodeKey == ERROR_SUCCESS)
			{
				DWORD dwType;
				LSTATUS retCodeValue = RegQueryValueEx(hKey, regValue.c_str(), NULL, &dwType, NULL, NULL);
				if ((retCodeValue == ERROR_SUCCESS) && (CloseKey(hKey)))
				{
					ret = true;
				}
			}
		}

		return ret;
	}

	bool GetRegStringValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, std::wstring &regContent)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			regContent.clear();
			WCHAR szBuffer[MAX_PATH] = { 0 };
			DWORD dwBufferSize = sizeof(szBuffer);

			if (RegGetValueW(
				hKey,
				NULL,
				regValue.c_str(),
				RRF_RT_REG_SZ,
				NULL,
				szBuffer,
				&dwBufferSize) == ERROR_SUCCESS)
			{
				regContent.assign(szBuffer);
				ret = true;
			}

			RegCloseKey(hKey);
		}

		return ret;
	}


	bool SetRegStringValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const std::wstring &regContent)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			if (RegSetValueExW(hKey, regValue.c_str(), 0, REG_SZ, ((const BYTE*)regContent.c_str()), (DWORD)((regContent.length() * sizeof(TCHAR)) + 1)) == ERROR_SUCCESS)
			{
				ret = true;
			}

			RegCloseKey(hKey);
		}

		return ret;
	}

	bool GetRegDWORDValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, DWORD &nValue)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			nValue = 0;
			DWORD dwBufferSize = sizeof(DWORD);
			DWORD nResult = 0;
			LONG nError = ::RegQueryValueExW(hKey,
				regValue.c_str(),
				0,
				NULL,
				reinterpret_cast<LPBYTE>(&nResult),
				&dwBufferSize);

			if (ERROR_SUCCESS == nError)
			{
				nValue = nResult;
				ret = true;
			}

			RegCloseKey(hKey);
		}

		return ret;
	}


	bool SetRegDWORDValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const DWORD &nValue)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			if (RegSetValueExW(hKey, regValue.c_str(), 0, REG_DWORD, ((const BYTE*)&nValue), sizeof(nValue)) == ERROR_SUCCESS)
			{
				ret = true;
			}

			RegCloseKey(hKey);
		}

		return ret;
	}

	bool GetRegBoolValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, bool &nValue)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			nValue = false;
			DWORD dwBufferSize = sizeof(DWORD);
			DWORD nResult = 0;
			LONG nError = ::RegQueryValueExW(hKey,
				regValue.c_str(),
				0,
				NULL,
				reinterpret_cast<LPBYTE>(&nResult),
				&dwBufferSize);

			if (ERROR_SUCCESS == nError)
			{
				nValue = nResult;
				ret = true;
			}
		}

		return ret;
	}


	bool SetRegBoolValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const bool &nValue)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			DWORD workingValue = 0;

			if (nValue) workingValue = 1;

			if (RegSetValueExW(hKey, regValue.c_str(), 0, REG_DWORD, ((const BYTE*)&workingValue), sizeof(workingValue)) == ERROR_SUCCESS)
			{
				ret = true;
			}

			RegCloseKey(hKey);
		}

		return ret;
	}


	bool GetRegBinaryValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, CommonTypes::ByteContainer& data)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			DWORD dwBufferSize = 0;
			LONG nError = ::RegGetValue(hKey,
				NULL,
				regValue.c_str(),
				RRF_RT_REG_BINARY,
				NULL,
				NULL,
				&dwBufferSize);

			if ((ERROR_SUCCESS == nError) && (dwBufferSize > 0))
			{				
				CommonTypes::ByteContainer workingData(dwBufferSize);

				LONG nErrorInternal = ::RegGetValue(hKey,
					NULL,
					regValue.c_str(),
					RRF_RT_REG_BINARY,
					NULL,
					&workingData[0],
					&dwBufferSize);

				if ((ERROR_SUCCESS == nError) && (!workingData.empty()))
				{
					data.swap(workingData);
					ret = true;
				}				
			}
		}

		return ret;
	}

	bool SetRegBinaryValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const CommonTypes::ByteContainer& data)
	{
		bool ret = false;

		HKEY hKey = NULL;
		if (!regSubKey.empty() && !regValue.empty() && !data.empty() && (hRootKey != NULL) && OpenKey(hRootKey, regSubKey, hKey))
		{
			const DWORD dataSize = static_cast<DWORD>(data.size());

			if (RegSetValueExW(hKey, regValue.c_str(), 0, REG_BINARY, ((const BYTE*)&data[0]), dataSize) == ERROR_SUCCESS)
			{
				ret = true;
			}

			RegCloseKey(hKey);
		}

		return ret;
	}

	bool PlaceRegMonitoringEvent(const HKEY& hKey, HANDLE& hEvent, DWORD flags)
	{
		bool ret = false;

		if ((hKey != INVALID_HANDLE_VALUE) &&			
			(RegNotifyChangeKeyValue(hKey, true, flags, hEvent, true) == ERROR_SUCCESS))
		{
			ret = true;
		}

		return ret;
	}
}

namespace ServiceHelpers
{
	bool WaitForState(const SC_HANDLE& hService, const DWORD finalState, const DWORD pendingState)
	{
		bool ret = false;
		SERVICE_STATUS_PROCESS status = { 0 };
		DWORD readBytes;

		if (QueryServiceStatusEx(
			hService,
			SC_STATUS_PROCESS_INFO,
			(LPBYTE)&status,
			sizeof(SERVICE_STATUS_PROCESS),
			&readBytes))
		{
			if (status.dwCurrentState != finalState)
			{
				DWORD startTime = GetTickCount();
				DWORD waitTime = status.dwWaitHint / 10;

				//Waiting state recommended by MS
				if (waitTime < 1000)
				{
					waitTime = 1000;
				}
				else if (waitTime > 5000)
				{
					waitTime = 5000;
				}

				while (status.dwCurrentState == pendingState)
				{
					Sleep(waitTime);

					if (QueryServiceStatusEx(
						hService,
						SC_STATUS_PROCESS_INFO,
						(LPBYTE)&status,
						sizeof(SERVICE_STATUS_PROCESS),
						&readBytes))
					{
						if (status.dwCurrentState == finalState)
						{
							ret = true;
						}
					}
				}
			}
		}

		return ret;
	}

	bool RegisterService(const std::wstring &serviceExecutable, const std::wstring &serviceCmdArgs, const std::wstring &serviceName, const std::wstring &serviceDisplay)
	{
		bool ret = false;

		SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM)
		{
			std::wstring fullExecutableLocationPlusArgs;
			//fullExecutableLocationPlusArgs.assign(L"\"");
			fullExecutableLocationPlusArgs.assign(serviceExecutable);
			fullExecutableLocationPlusArgs.append(L" ");
			fullExecutableLocationPlusArgs.append(serviceCmdArgs);
			//fullExecutableLocationPlusArgs.append(L"\"");

			SC_HANDLE hService = CreateService(hSCM,
				serviceName.c_str(),					// Name of service
				serviceDisplay.c_str(),					// Name to display
				SERVICE_ALL_ACCESS,						// Desired access
				SERVICE_WIN32_OWN_PROCESS,				// Service type
				SERVICE_AUTO_START,						// Service start type
				SERVICE_ERROR_NORMAL,					// Error control type
				fullExecutableLocationPlusArgs.c_str(),	// Service's binary location
				NULL,                           // No load ordering group
				NULL,                           // No tag identifier
				NULL,							// Dependencies
				NULL,							// Service running account
				NULL							// Password of the account
			);

			if (hService != NULL)
			{
				SERVICE_DESCRIPTION sd = { 0 };
				sd.lpDescription = (wchar_t *)serviceDisplay.c_str();
				if (ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd))
				{
					ret = true;
				}

				CloseServiceHandle(hService);
			}

			CloseServiceHandle(hSCM);
			hSCM = NULL;
		}

		return ret;
	}

	bool IsSameServiceExecutablePath(const std::wstring &serviceName, const std::wstring &fullServiceExecPath)
	{
		bool ret = false;
		DWORD bytesNeeded = 0;
		DWORD buffSize = 0;
		LPQUERY_SERVICE_CONFIG lpsc = { 0 };

		SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM)
		{
			SC_HANDLE hService = OpenService(hSCM, serviceName.c_str(), SERVICE_QUERY_CONFIG);
			if (hService)
			{
				// Get the configuration information and prepare buffer first
				if (!QueryServiceConfig(hService, NULL, 0, &bytesNeeded))
				{
					DWORD lastRrror = GetLastError();
					if (ERROR_INSUFFICIENT_BUFFER == lastRrror)
					{
						buffSize = bytesNeeded;
						lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, buffSize);

						if (QueryServiceConfig(hService, lpsc, buffSize, &bytesNeeded))
						{
							std::wstring registeredServiceFullPath(lpsc->lpBinaryPathName);

							//TODO: This can be improved to perform absolut match
							if (GeneralHelpers::StrContainsPattern(registeredServiceFullPath, fullServiceExecPath))
							{
								ret = true;
							}

						}

						if (lpsc)
						{
							LocalFree(lpsc);
						}
					}
				}

				CloseServiceHandle(hService);
				hService = NULL;
			}

			CloseServiceHandle(hSCM);
			hSCM = NULL;
		}

		return ret;
	}

	bool DeleteService(const std::wstring &serviceName)
	{
		bool ret = false;

		StopTargetService(serviceName);

		//forcing some sleep time to avoid rapid SCM operations
		Sleep(500);

		SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM)
		{
			SC_HANDLE hService = OpenService(hSCM, serviceName.c_str(), DELETE);
			if (hService)
			{
				if (DeleteService(hService))
				{
					ret = true;
				}

				CloseServiceHandle(hService);
				hService = NULL;
			}

			CloseServiceHandle(hSCM);
			hSCM = NULL;
		}

		return ret;
	}

	bool IsServiceCreated(const std::wstring& serviceName)
	{
		bool ret = false;

		SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM)
		{
			SC_HANDLE hService = OpenService(hSCM, serviceName.c_str(), (SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS));
			if (hService)
			{
				CloseServiceHandle(hService);
				hService = NULL;
				ret = true;
			}

			CloseServiceHandle(hSCM);
			hSCM = NULL;
		}

		//forcing some sleep time to avoid rapid SCM operations
		Sleep(500);

		return ret;
	}


	bool IsServiceStopped(const std::wstring& serviceName)
	{
		bool ret = false;
		SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM)
		{
			SC_HANDLE hService = OpenService(hSCM, serviceName.c_str(), (SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS));
			if (hService)
			{
				SERVICE_STATUS status = { 0 };
				if (QueryServiceStatus(hService, &status))
				{
					if ((status.dwCurrentState == SERVICE_STOP_PENDING) || (status.dwCurrentState == SERVICE_STOPPED))
					{
						if (SERVICE_STOP_PENDING == status.dwCurrentState)
						{
							WaitForState(hService, SERVICE_STOPPED, SERVICE_STOP_PENDING);
						}
						ret = true;
					}
				}

				CloseServiceHandle(hService);
				hService = NULL;
			}

			CloseServiceHandle(hSCM);
			hSCM = NULL;
		}

		return ret;
	}


	bool IsServiceStarted(const std::wstring& serviceName)
	{
		bool ret = false;
		SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (hSCM)
		{
			SC_HANDLE hService = OpenService(hSCM, serviceName.c_str(), (SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS));
			if (hService)
			{
				SERVICE_STATUS status = { 0 };
				if (QueryServiceStatus(hService, &status))
				{
					if ((status.dwCurrentState == SERVICE_START_PENDING) || (status.dwCurrentState == SERVICE_RUNNING))
					{
						if (SERVICE_START_PENDING == status.dwCurrentState)
						{
							WaitForState(hService, SERVICE_RUNNING, SERVICE_START_PENDING);
						}
						ret = true;
					}
				}

				CloseServiceHandle(hService);
				hService = NULL;
			}

			CloseServiceHandle(hSCM);
			hSCM = NULL;
		}

		return ret;
	}


	bool StartTargetService(const std::wstring& serviceName)
	{
		bool ret = false;

		if (IsServiceStarted(serviceName))
		{
			ret = true;
		}
		else
		{
			//waiting for any pending operation
			IsServiceStopped(serviceName);

			SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hSCM)
			{
				SC_HANDLE hService = OpenService(hSCM, serviceName.c_str(), (SERVICE_START | SERVICE_ENUMERATE_DEPENDENTS));
				if (hService)
				{
					// Attempt to start the service
					if (::StartService(hService, 0, NULL))
					{
						WaitForState(hService, SERVICE_RUNNING, SERVICE_START_PENDING);
						ret = true;
					}

					CloseServiceHandle(hService);
					hService = NULL;
				}

				CloseServiceHandle(hSCM);
				hSCM = NULL;
			}
		}

		return ret;
	}


	bool StopTargetService(const std::wstring& serviceName)
	{
		bool ret = false;

		if (IsServiceStopped(serviceName))
		{
			ret = true;
		}
		else
		{
			//waiting for any pending operation
			IsServiceStarted(serviceName);

			SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hSCM)
			{
				SC_HANDLE hService = OpenService(hSCM, serviceName.c_str(), (SERVICE_STOP | SERVICE_ENUMERATE_DEPENDENTS));
				if (hService)
				{
					SERVICE_STATUS_PROCESS status;
					if (ControlService(hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&status))
					{
						WaitForState(hService, SERVICE_STOPPED, SERVICE_STOP_PENDING);
						ret = true;
					}

					CloseServiceHandle(hService);
					hService = NULL;
				}

				CloseServiceHandle(hSCM);
				hSCM = NULL;
			}
		}

		//forcing some sleep time to avoid rapid SCM operations
		Sleep(500);

		return ret;
	}


	bool RestartTargetService(const std::wstring& serviceName)
	{
		bool ret = false;

		if (!serviceName.empty() && 
			StopTargetService(serviceName) &&
			StartTargetService(serviceName))
		{
			ret = true;
		}


		return ret;
	}
}

namespace KrabsETWHelper
{
	bool GetGUIDValue(const krabs::binary& blob, GUID &guidData)
	{
		bool ret = false;
		static size_t MAGIC_ARRAY_SIZE_FOR_GUID = 16;
		const std::vector<BYTE>& bytes = blob.bytes();

		if (!bytes.empty() && (bytes.size() == MAGIC_ARRAY_SIZE_FOR_GUID))
		{
			guidData.Data1 = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
			guidData.Data2 = bytes[4] | (bytes[5] << 8);
			guidData.Data3 = bytes[6] | (bytes[7] << 8);
			guidData.Data4[0] = bytes[8];
			guidData.Data4[1] = bytes[9];
			guidData.Data4[2] = bytes[10];
			guidData.Data4[3] = bytes[11];
			guidData.Data4[4] = bytes[12];
			guidData.Data4[5] = bytes[13];
			guidData.Data4[6] = bytes[14];
			guidData.Data4[7] = bytes[15];

			ret = true;
		}

		return ret;
	}

	bool GetHEXINT64Value(const krabs::binary& blob, UINT64 &value)
	{
		bool ret = false;
		static size_t MAGIC_ARRAY_SIZE = 8;
		const std::vector<BYTE>& bytes = blob.bytes();

		if (!bytes.empty() && (bytes.size() == MAGIC_ARRAY_SIZE))
		{
			value = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24) | (bytes[4] << 32) | (bytes[5] << 40) | (bytes[6] << 48) | (bytes[7] << 56);

			ret = true;
		}

		return ret;
	}

	bool GetHEXINT32Value(const krabs::binary& blob, UINT32 &value)
	{
		bool ret = false;
		static size_t MAGIC_ARRAY_SIZE = 4;
		const std::vector<BYTE>& bytes = blob.bytes();

		if (!bytes.empty() && (bytes.size() == MAGIC_ARRAY_SIZE))
		{
			value = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);

			ret = true;
		}

		return ret;
	}

	bool GetHEXINT16Value(const krabs::binary& blob, UINT16 &value)
	{
		bool ret = false;
		static size_t MAGIC_ARRAY_SIZE = 2;
		const std::vector<BYTE>& bytes = blob.bytes();

		if (!bytes.empty() && (bytes.size() == MAGIC_ARRAY_SIZE))
		{
			value = bytes[0] | (bytes[1] << 8);

			ret = true;
		}

		return ret;
	}

	bool GetHEXINT8Value(const krabs::binary& blob, UINT8 &value)
	{
		bool ret = false;
		static size_t MAGIC_ARRAY_SIZE = 1;
		const std::vector<BYTE>& bytes = blob.bytes();

		if (!bytes.empty() && (bytes.size() == MAGIC_ARRAY_SIZE))
		{
			value = bytes[0];

			ret = true;
		}

		return ret;
	}

}