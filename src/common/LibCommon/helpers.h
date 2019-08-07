#pragma once

#include "commondefs.h"
#include "commontypes.h"
#include "external.h"

namespace GeneralHelpers
{
	bool StartProcess(const std::wstring &process, const std::wstring &args);
	int  ToInteger(const std::wstring &st);
	bool IsNumber(const std::wstring& str);
	bool IsValidFile(const std::wstring &fileName);
	bool IsValidDirectory(const std::wstring &directory);
	bool GetFullPathToFile(const std::wstring &fileName, std::wstring &fullPathFile);
	bool GetBaseFileName(const std::wstring &fullPath, std::wstring &baseName);
	bool GetCurrentProcessModuleFullPath(std::wstring &fullPath);
	bool GetCurrentProcessModuleDirectory(std::wstring &fullDirectoryPath);
	bool StrCompare(const std::wstring& str1, const std::wstring& str2);
	bool StrCompare(const std::string& str1, const std::string& str2);
	bool StrCompareInsensitive(const std::string& str1, const std::string& str2);
	bool StrCompareInsensitive(const std::wstring& str1, const std::wstring& str2);
	bool StrContainsPattern(const std::wstring& str, const std::wstring& pattern);
	bool StrContainsPatternInsensitive(const std::wstring& str, const std::wstring& pattern);
	bool StrContainsPattern(const std::string& str, const std::string& pattern);
	bool StrContainsPatternInsensitive(const std::string& str, const std::string& pattern);
	bool IsRunningAs64BitProcess();
	bool IsRunningAsAdmin();
	bool TerminateProcess(const DWORD &processID, const uint32_t &exitCode);
	bool GetSystemArchitecture(DWORD &arch);
	bool GetProcessBitness(const HANDLE &hProcess, CommonTypes::ProcessType &processType);
	bool EnableTokenPrivilege(LPTSTR pPrivilege);
	bool IsProcessStillRunning(const HANDLE &hProcess);
	bool IsThreadStillRunning(const HANDLE &hThread);
	bool IsDotNETLib(const std::wstring &name);
	bool GetWindowsSystemDirectory(std::wstring &system32Directory);
	bool GetWindowsSystemRootDirectory(std::wstring &systemRootDirectory);
	bool GetVectorByToken(const std::string& input, const char token, std::vector<std::string> &vector);
	bool GetVectorByWindowsCRToken(const std::string& input, std::vector<std::string> &vector);
	bool GetVectorByToken(const std::wstring& input, const wchar_t token, std::vector<std::wstring> &vector);
	bool IsElementPresentOnList(const std::vector<std::wstring> &listToCheck, const std::wstring &element);
	bool TrimSpaces(std::string &str);
	bool TrimSpaces(std::wstring &str);
	bool GetProcessnameByPID(DWORD pid, std::wstring &processName);
	bool WStringFromVector(const CommonTypes::WCharContainer& source, std::wstring& target);
	bool WStringToVector(const std::wstring& source, CommonTypes::WCharContainer& target);
	bool DPAPIEncrypt(const std::string& input, const std::string& entropy, std::string &output);
	bool DPAPIDecrypt(const std::string& input, const std::string& entropy, std::string &output);
	bool DPAPIEncrypt(const CommonTypes::ByteContainer& input, const std::string& entropy, CommonTypes::ByteContainer &output);
	bool DPAPIDecrypt(const CommonTypes::ByteContainer& input, const std::string& entropy, CommonTypes::ByteContainer &output);
	bool UnzipZIPFileToFolder(const std::wstring &zipFile, const std::wstring &targetFolder);
	bool DownloadFile(const std::wstring &targetHost, const std::wstring &targetURLPath, const CommonTypes::ProxyConfData &proxyData, const std::wstring &targetFile);
	bool RenameFile(const std::wstring &oldfile, const std::wstring &newfile);
	bool RemoveDeletePending(const std::wstring &fileToRemove);
	bool RemoveFile(const std::wstring &fileToRemove);
	bool GetTargetFileSize(const std::wstring& file, size_t &size);
	bool GetTargetFileIntoBuffer(const std::wstring& targetFile, const size_t &targetFileSize, LPVOID lpBuffer, DWORD &bytesRead);
	bool GetTargetFileIntoString(const std::wstring& fullPathFile, std::wstring& buffer);
	bool GetTargetFileIntoString(const std::wstring& fullPathFile, std::string& buffer);
	bool GetUserProfileDirectory(std::wstring &userProfileDir);
	bool GetRandomString(const unsigned int length, std::string &randomStr);
	bool CreateNewDirectory(const std::wstring& newDirectory, LPSECURITY_ATTRIBUTES &securityAttributes);
	bool RemoveWorkingDirectory(const std::wstring& newDirectory);
	bool GetTemporaryDirLocation(std::wstring &tempPath, unsigned int lengthRandomDir = CommonDefs::RANDOM_STRING_LENGTH);
	bool GetAdminSecuredDirLocation(std::wstring &newPath, unsigned int lengthRandomDir = CommonDefs::RANDOM_STRING_LENGTH);
	bool GetCommonAppDataPath(std::wstring &commonPath);
	bool GetSysmonXCommonPath(std::wstring &sysmonXCommonPath);
	bool GetNewSysmonXRandomDirectoryName(const std::wstring &sysmonXCommonPath, std::wstring &newSysmonXDirectory);
	bool GetNewSecureSysmonXDirectory(std::wstring &newSecureDirectory);
	bool GetTemporaryFileName(const std::wstring &tempDirectory, const std::wstring &tempFileExtension, std::wstring &tempFileName, unsigned int lengthRandomDir = CommonDefs::RANDOM_STRING_LENGTH);
	bool ConvertStringToHexRepresentation(const std::string &input, std::string &output);
	bool RunProcess(const std::wstring &process, const std::wstring &cmdlineArguments, CommonTypes::StringsContainer &processOutput, DWORD &exitCode);
	bool GetCurrentProcessFullPath(std::wstring &currentProcesFullPath);
	bool FileCopy(const std::wstring &originFile, const std::wstring &destFile, bool overwrite = true);
	bool GetFileNameWithoutExtension(const std::wstring &filename, std::wstring &baseFileNameWoExtension);
	bool ComputeMD5FileHash(const std::wstring &filename, std::string &hash);
	bool AreFilesEqual(const std::wstring &file1, const std::wstring &file2);

	std::string  WStrToStr(const std::wstring& wstr);
	std::wstring GetCurrentProcessFullPath();
	std::wstring StrToWStr(const std::string& str);
	std::wstring GetBaseFileName(const std::wstring &fullPath);
	std::wstring ToWstring(const unsigned int &value);
	std::wstring ReplaceTokensInStr(const std::wstring &str, const std::wstring &from, const std::wstring &to);
	std::wstring SanitizeStr(const std::wstring &str);
	std::wstring GetHexString(PVOID addr);
	std::wstring GetMemoryRegionType(DWORD id);
	std::wstring GetMemoryRegionState(DWORD id);
	std::wstring GetMemoryRegionProtection(DWORD id);
	std::wstring GetWstrFromInteger(UINT32 &value);
	std::wstring GetFileNameWithoutExtension(const std::wstring &filename);
	std::wstring WStringFromVector(const CommonTypes::WCharContainer& source);
	std::wstring GetSerializedWString(const CommonTypes::SerializedWCharVector& source);
	std::string GetSerializedString(const CommonTypes::SerializedCharVector& source);
	std::string GetSerializedString(const CommonTypes::SerializedWCharVector& source);
	std::string GetRandomString(const unsigned int length);
	std::string GetErrorText(NTSTATUS code);


	//Abusing lexical cast from boost here
	template <typename Source>
	bool GetWstringFromValue(const Source &argValue, const std::wstring &string)
	{
		bool ret = false;

		if (boost::conversion::try_lexical_convert(argValue, string))
		{
			ret = true;
		}

		return ret;
	}

	//TODO: Templatize this
	CommonTypes::WCharContainer WStringToVector(const std::wstring& source);
	CommonTypes::CharContainer GetSerializedVector(const CommonTypes::SerializedCharVector& source);
	CommonTypes::WCharContainer GetSerializedVector(const CommonTypes::SerializedWCharVector& source);
	CommonTypes::SerializedWCharVector GetSerializedVector(const std::wstring& source);
	CommonTypes::SerializedCharVector GetSerializedVector(const std::string& source);
	CommonTypes::StringsContainer GetSerializedStringVector(const CommonTypes::SerializedCistaStringVector& source);
	CommonTypes::SerializedCistaStringVector GetSerializedStringVector(const CommonTypes::StringsContainer& source);
	CommonTypes::UIntContainer GetSerializedVector(const CommonTypes::SerializedUIntVector& source);
	CommonTypes::SerializedUIntVector GetSerializedVector(const CommonTypes::UIntContainer& source);

	void StrAddDelimitator(const wchar_t delim, std::wstring &str);
	void StrTrim(std::wstring &str);
	void StrToUppercase(std::wstring &str);
	void StrToUppercase(std::string &str);
	void StrToLowercase(std::wstring &str);
	void StrToLowercase(std::string &str);
	const time_t GetEpochTimestamp();
}

namespace RegistryHelpers
{
	bool DeleteKey(const HKEY &hRootKey, const std::wstring &regSubKey);
	bool CreateKey(const HKEY &hRootKey, const std::wstring &regSubKey);
	bool DeleteValue(const HKEY &hRootKey, const std::wstring &regSubKey, const std::wstring &regValue);
	bool RegistryKeyExists(const HKEY &hRootKey, const std::wstring &regSubKey);
	bool RegistryValueExists(const HKEY &hRootKey, const std::wstring &regSubKey, const std::wstring& regValue);
	bool GetRegStringValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, std::wstring &regContent);
	bool SetRegStringValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const std::wstring &regContent);
	bool GetRegDWORDValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, DWORD &nValue);
	bool SetRegDWORDValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const DWORD &nValue);
	bool GetRegBoolValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, bool &nValue);
	bool SetRegBoolValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const bool &nValue);
	bool GetRegBinaryValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, CommonTypes::ByteContainer& data);
	bool SetRegBinaryValue(const HKEY hRootKey, const std::wstring& regSubKey, const std::wstring& regValue, const CommonTypes::ByteContainer& data);
}

namespace ServiceHelpers
{
	bool RegisterService(const std::wstring &serviceExecutable, const std::wstring &serviceCmdArgs, const std::wstring &serviceName, const std::wstring &serviceDisplay);
	bool IsSameServiceExecutablePath(const std::wstring &serviceName, const std::wstring &fullServiceExecPath);
	bool DeleteService(const std::wstring &serviceName);
	bool IsServiceCreated(const std::wstring& serviceName);
	bool IsServiceStopped(const std::wstring& serviceName);
	bool IsServiceStarted(const std::wstring& serviceName);
	bool StartTargetService(const std::wstring& serviceName);
	bool StopTargetService(const std::wstring& serviceName);
	bool RestartTargetService(const std::wstring& serviceName);
}

namespace KrabsETWHelper
{
	bool GetGUIDValue(const krabs::binary& blob, GUID &guidData);
	bool GetHEXINT64Value(const krabs::binary& blob, UINT64 &value);
	bool GetHEXINT32Value(const krabs::binary& blob, UINT32 &value);
	bool GetHEXINT16Value(const krabs::binary& blob, UINT16 &value);
	bool GetHEXINT8Value(const krabs::binary& blob, UINT8 &value);
}


namespace TraceHelpers
{
	//Custom SPD wrapper logger
	class Logger
	{
	private:

		Logger() : m_initialized(false), m_logger(nullptr) { }

		~Logger() {	}

		bool m_initialized;
		spdlog::logger *m_logger;

	public:

		//Forcing singleton here
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger& operator=(Logger&&) = delete;

		static Logger& Instance()
		{
			static Logger INSTANCE;
			return INSTANCE;
		}

		bool Initialize(const CommonTypes::LoggerContainer &modes, std::string logfile);

		bool IsInitialized() { return m_initialized; }

		void SetLevel(CommonTypes::LoggerVerbose level);

		template<typename... Args>
		inline void Error(const std::string &fmt, const Args &... args)
		{
			if (m_initialized && m_logger && !fmt.empty())
			{
				m_logger->error(fmt.c_str(), args...);
			}
		}

		template<typename... Args>
		inline void Warn(const std::string &fmt, const Args &... args)
		{
			if (m_initialized && m_logger && !fmt.empty())
			{
				m_logger->warn(fmt.c_str(), args...);
			}
		}

		template<typename... Args>
		inline void Info(const std::string &fmt, const Args &... args)
		{
			if (m_initialized && m_logger && !fmt.empty())
			{
				m_logger->info(fmt.c_str(), args...);
			}
		}

		template<typename... Args>
		inline void Debug(const std::string &fmt, const Args &... args)
		{
			if (m_initialized && m_logger && !fmt.empty())
			{
				m_logger->debug(fmt.c_str(), args...);
			}
		}

		template<typename... Args>
		inline void Trace(const std::string &fmt, const Args &... args)
		{
			if (m_initialized && m_logger && !fmt.empty())
			{
				m_logger->trace(fmt.c_str(), args...);
			}
		}

		template<typename... Args>
		inline void Critical(const std::string &fmt, const Args &... args)
		{
			if (m_initialized && m_logger && !fmt.empty())
			{
				m_logger->critical(fmt.c_str(), args...);
			}
		}

		//Standalone console trace
		void TraceUp(const char *buffer, ...);
		void TraceDown(const char *buffer, ...);
		void TraceConsoleUp(const char *buffer, ...);
		void TraceConsoleDown(const char *buffer, ...);
		void TraceConsole(const char *buffer, ...);
	};
}
