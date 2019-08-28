#include "common.h"

namespace SysmonXHelpers
{
	bool IsPIDInCollection(SysmonXCommon::ProcessCollection &sysProcessesData, const DWORD &pid, SysmonXCommon::ProcessCollection::iterator &it)
	{
		bool ret = false;
		it = sysProcessesData.find(pid);
		if (it != sysProcessesData.end())
		{
			ret = true;
		}
		return ret;
	}

	bool GetSystemProcessesData(SysmonXCommon::ProcessCollection &sysProcessesData)
	{
		bool ret = false;
		HANDLE hSystemSnap = INVALID_HANDLE_VALUE;
		DWORD currentProcessID = GetCurrentProcessId();

		//Clearing container first
		CleanupSystemProcessData(sysProcessesData);

		// Take a snapshot of entire system
		hSystemSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hSystemSnap != INVALID_HANDLE_VALUE)
		{
			THREADENTRY32 te32 = { 0 };
			te32.dwSize = sizeof(THREADENTRY32);
			for (BOOL threadSuccess = Thread32First(hSystemSnap, &te32);
				threadSuccess != FALSE;
				threadSuccess = Thread32Next(hSystemSnap, &te32))
			{
				//Skipping critical and current processes
				if ((te32.th32OwnerProcessID > 4) &&
					(te32.th32OwnerProcessID != currentProcessID))
				{
					SysmonXCommon::ProcessCollection::iterator it;
					if (IsPIDInCollection(sysProcessesData, te32.th32OwnerProcessID, it))
					{
						it->second->threads.push_back(te32);
					}
					else
					{
						//Only initializing process once
						HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, te32.th32OwnerProcessID);
						if (hProcess != NULL)
						{
							wchar_t processNameBuff[MAX_PATH] = { 0 };
							std::wstring processFullPathName;

							//if (GetModuleBaseName( hProcess, 0, processNameBuff, MAX_PATH))
							if (GetModuleFileNameEx(hProcess, 0, processNameBuff, MAX_PATH))
							{
								processFullPathName.assign(processNameBuff);

								std::wstring baseProcessName;
								if (GeneralHelpers::GetBaseFileName(processFullPathName, baseProcessName))
								{
									//Should skip this entry has it has been flagged for exclusion
									continue;
								}
							}
	
							CommonTypes::ProcessType type = CommonTypes::ProcessType::PROCESS_UNKNOWN;
							GeneralHelpers::GetProcessBitness(hProcess, type);

							auto processData = std::make_shared<SysmonXCommon::ProcessData>(hProcess, te32.th32OwnerProcessID, processFullPathName, type);

							processData->threads.push_back(te32);

							//Getting modules for the process							
							HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, te32.th32OwnerProcessID);
							if (hModuleSnap != INVALID_HANDLE_VALUE)
							{
								MODULEENTRY32 me32 = { 0 };
								//  Set the size of the structure before using it. 
								me32.dwSize = sizeof(MODULEENTRY32);
								for (BOOL moduleSuccess = Module32First(hModuleSnap, &me32);
									moduleSuccess != FALSE;
									moduleSuccess = Module32Next(hModuleSnap, &me32))
								{
									processData->modules.push_back(me32);

									/*
									if (!processData->isManaged && GeneralHelpers::IsDotNETLib(me32.szModule))
									{
										processData->isManaged = true;
									}
									*/
								}

								CloseHandle(hModuleSnap);
							}

							//Initializing blackbone over process
							if ((!processData->bbProcess.valid()) && (NT_SUCCESS(processData->bbProcess.Attach(te32.th32OwnerProcessID))))
							{
								processData->bbProcessReady = true;

								//if (!processData->bbProcess.barrier().mismatch)
								if (processData->bbProcess.valid())
								{
									processData->bbMainModule = processData->bbProcess.modules().GetMainModule();
									if ((processData->bbMainModule != nullptr) &&
										(!processData->bbMainModule->fullPath.empty()) &&
										(NT_SUCCESS(processData->bbPE.Load(processData->bbMainModule->fullPath))))
									{
										processData->isManaged = processData->bbPE.pureIL();
										processData->bbPEReady = true;
									}

									processData->bbModulesManuallyMapped = processData->bbProcess.modules().GetManualModules();
									processData->bbModulesLdrList = processData->bbProcess.modules().GetAllModules(blackbone::LdrList);
									//processData->bbModulesPEHeaders = processData->bbProcess.modules().GetAllModules(blackbone::PEHeaders);
									//processData->bbModulesSections = processData->bbProcess.modules().GetAllModules(blackbone::Sections);
								}
							}

							sysProcessesData.insert(std::pair<DWORD, std::shared_ptr<SysmonXCommon::ProcessData>>(te32.th32OwnerProcessID, processData));
						}
					}
				}
			}

			ret = true;
			CloseHandle(hSystemSnap);
		}

		return ret;
	}


	bool CleanupSystemProcessData(SysmonXCommon::ProcessCollection &sysProcessesData)
	{
		bool ret = true;

		for (SysmonXCommon::ProcessCollection::iterator handleIt = sysProcessesData.begin();
			handleIt != sysProcessesData.end();
			++handleIt)
		{
			if (handleIt->first > 0)
			{
				std::shared_ptr<SysmonXCommon::ProcessData> procInfo = handleIt->second;
				CloseHandle(procInfo->handle);
			}
		}

		sysProcessesData.clear();

		return ret;
	}

	bool GetMemoryRegionInfo(HANDLE &hProcess, size_t &dwStartAddress, MEMORY_BASIC_INFORMATION &info)
	{
		bool ret = false;

		memset(&info, 0, sizeof(info));
		if (VirtualQueryEx(hProcess, (LPVOID)dwStartAddress, &info, sizeof(MEMORY_BASIC_INFORMATION)) != 0)
		{
			ret = true;
		}

		return ret;
	}

	bool GetThreadBasicInfo(HANDLE &thread, CustomWinTypes::THREAD_BASIC_INFORMATION &threadInfo)
	{
		bool ret = false;
		HMODULE ntdll = NULL;
		static CustomWinTypes::PNtQueryInformationThread NtQueryInformationThread = NULL;

		//lazy siglenton
		if (NtQueryInformationThread == NULL)
		{
			if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"ntdll.dll", &ntdll))
			{
				NtQueryInformationThread = (CustomWinTypes::PNtQueryInformationThread)GetProcAddress(ntdll, "NtQueryInformationThread");
			}
		}

		if (NtQueryInformationThread != NULL)
		{
			if (NT_SUCCESS(NtQueryInformationThread(thread, CustomWinTypes::ThreadBasicInformation, &threadInfo, sizeof(threadInfo), NULL)))
			{
				ret = true;
			}
		}

		return ret;
	}


	bool GetThreadStartAddress(HANDLE &thread, PVOID &threadStartAddress)
	{
		bool ret = false;
		HMODULE ntdll = NULL;
		static CustomWinTypes::PNtQueryInformationThread NtQueryInformationThread = NULL;
		DWORD dwLen = 0;

		//lazy siglenton
		if (NtQueryInformationThread == NULL)
		{
			if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"ntdll.dll", &ntdll))
			{
				NtQueryInformationThread = (CustomWinTypes::PNtQueryInformationThread)GetProcAddress(ntdll, "NtQueryInformationThread");
			}
		}

		if (NtQueryInformationThread != NULL)
		{
			if (NT_SUCCESS(NtQueryInformationThread(thread, CustomWinTypes::ThreadQuerySetWin32StartAddress, &threadStartAddress, sizeof(PVOID), &dwLen)))
			{
				ret = true;
			}
		}

		return ret;
	}


	bool IsFuzzyPEHeaderPresent(HANDLE &hProcess, size_t &addressToCheck, size_t &regionSize)
	{
		bool ret = false;
		const char *header = "MZ";
		char *buffer = nullptr;

		buffer = (char*)malloc(regionSize);
		if ((ReadProcessMemory(hProcess, (LPVOID)addressToCheck, buffer, regionSize, NULL)) &&
			(strncmp(buffer, header, 2) == 0))
		{
			ret = true;
		}
		free(buffer);

		return ret;
	}

	bool PopulateModulesIfNeededByWalkingPEHeaders(SysmonXCommon::ProcessDataPtr &processData)
	{
		bool ret = false;

		if (processData)
		{
			if (processData->bbModulesPEHeaders.size() > 0)
			{
				ret = true;
			}
			else
			{
				processData->bbModulesPEHeaders = processData->bbProcess.modules().GetAllModules(blackbone::PEHeaders);
				ret = true;
			}

		}

		return ret;
	}

	bool PopulateModulesIfNeededByMemorySections(SysmonXCommon::ProcessDataPtr &processData)
	{
		bool ret = false;

		if (processData)
		{
			if (processData->bbModulesSections.size() > 0)
			{
				ret = true;
			}
			else
			{
				processData->bbModulesSections = processData->bbProcess.modules().GetAllModules(blackbone::Sections);
				ret = true;
			}

		}

		return ret;
	}

	bool GetParentPid(const DWORD &pid, DWORD &parentPID)
	{
		bool ret = false;

		if (pid >= 0)
		{
			auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcSnap)
			{
				PROCESSENTRY32W pEntry = { 0 };
				pEntry.dwSize = sizeof(PROCESSENTRY32W);

				// Iterate processes
				for (BOOL success = Process32FirstW(hProcSnap, &pEntry);
					success != FALSE;
					success = Process32NextW(hProcSnap, &pEntry))
				{
					if (pEntry.th32ProcessID == pid)
					{
						parentPID = pEntry.th32ParentProcessID;
						ret = true;
					}
				}
			}
		}

		return ret;
	}

	bool GetPEBAddress(const HANDLE &processHandle, PVOID &baseaddr)
	{
		bool ret = false;
		HMODULE ntdll = NULL;
		PROCESS_BASIC_INFORMATION processPEBAddress = { 0 };
		static CustomWinTypes::pNtQueryInformationProcess NtQueryInformationProcess = { 0 };
		DWORD dwLen = 0;

		//lazy siglenton
		if (NtQueryInformationProcess == NULL)
		{
			if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"ntdll.dll", &ntdll))
			{
				NtQueryInformationProcess = (CustomWinTypes::pNtQueryInformationProcess)GetProcAddress(ntdll, "NtQueryInformationProcess");
			}
		}

		if (NtQueryInformationProcess != NULL)
		{
			if (NT_SUCCESS(NtQueryInformationProcess(processHandle, 0, &processPEBAddress, sizeof(processPEBAddress), &dwLen)))
			{
				baseaddr = processPEBAddress.PebBaseAddress;
				ret = true;
			}
		}

		return ret;
	}


	bool GetProcessCommandLine(blackbone::Process &targetProc, std::wstring &cmdline)
	{
		bool ret = false;
		PVOID pebAddress = nullptr;
		PVOID rtlUserProcParamsAddress = nullptr;
		UNICODE_STRING commandLine = { 0 };
		WCHAR *commandLineContents = nullptr;

		if (targetProc.valid())
		{
			cmdline.clear();

			HANDLE procHandle = targetProc.core().handle();
			
			if (GetPEBAddress(targetProc.core().handle(), pebAddress) &&
				ReadProcessMemory(targetProc.core().handle(),
				&(((_PEB*)pebAddress)->ProcessParameters),
				&rtlUserProcParamsAddress,
				sizeof(PVOID), NULL))
			{
				/* read the CommandLine UNICODE_STRING structure */
				if (ReadProcessMemory(targetProc.core().handle(),
					&(((_RTL_USER_PROCESS_PARAMETERS*)rtlUserProcParamsAddress)->CommandLine),
					&commandLine, sizeof(commandLine), NULL))
				{
					/* allocate memory to hold the command line */
					commandLineContents = (WCHAR *)malloc((commandLine.Length * sizeof(WCHAR)) + sizeof(WCHAR));
					memset(commandLineContents, 0, commandLine.Length);
					/* read the command line */
					if (ReadProcessMemory(targetProc.core().handle(), commandLine.Buffer,
						commandLineContents, commandLine.Length + sizeof(WCHAR), NULL))
					{
						/* print it */
						/* the length specifier is in characters, but commandLine.Length is in bytes */
						/* a WCHAR is 2 bytes */

						cmdline.assign(commandLineContents);

						if (!cmdline.empty())
						{
							ret = true;
						}
					}
				}
			}

			if (commandLineContents) free(commandLineContents);
		}

		return ret;
	}

	bool VerifyEmbeddedSignature(LPCWSTR pwszSourceFile)
	{
		LONG lStatus;
		DWORD dwLastError;
		bool retValue = false;

		// Initialize the WINTRUST_FILE_INFO structure.
		WINTRUST_FILE_INFO FileData;
		memset(&FileData, 0, sizeof(FileData));
		FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
		FileData.pcwszFilePath = pwszSourceFile;
		FileData.hFile = NULL;
		FileData.pgKnownSubject = NULL;

		/*
		WVTPolicyGUID specifies the policy to apply on the file
		WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:

		1) The certificate used to sign the file chains up to a root
		certificate located in the trusted root certificate store. This
		implies that the identity of the publisher has been verified by
		a certification authority.

		2) In cases where user interface is displayed (which this example
		does not do), WinVerifyTrust will check for whether the
		end entity certificate is stored in the trusted publisher store,
		implying that the user trusts content from this publisher.

		3) The end entity certificate has sufficient permission to sign
		code, as indicated by the presence of a code signing EKU or no
		EKU.
		*/

		GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
		WINTRUST_DATA WinTrustData;

		// Initialize the WinVerifyTrust input data structure.

		// Default all fields to 0.
		memset(&WinTrustData, 0, sizeof(WinTrustData));

		WinTrustData.cbStruct = sizeof(WinTrustData);

		// Use default code signing EKU.
		WinTrustData.pPolicyCallbackData = NULL;

		// No data to pass to SIP.
		WinTrustData.pSIPClientData = NULL;

		// Disable WVT UI.
		WinTrustData.dwUIChoice = WTD_UI_NONE;

		// No revocation checking.
		WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;

		// Verify an embedded signature on a file.
		WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;

		// Verify action.
		WinTrustData.dwStateAction = WTD_STATEACTION_VERIFY;

		// Verification sets this value.
		WinTrustData.hWVTStateData = NULL;

		// Not used.
		WinTrustData.pwszURLReference = NULL;

		// This is not applicable if there is no UI because it changes 
		// the UI to accommodate running applications instead of 
		// installing applications.
		WinTrustData.dwUIContext = 0;

		// Set pFile.
		WinTrustData.pFile = &FileData;

		// Prevent network calls during the validation process
		WinTrustData.dwProvFlags = WTD_CACHE_ONLY_URL_RETRIEVAL;

		// WinVerifyTrust verifies signatures as specified by the GUID 
		// and Wintrust_Data.
		lStatus = WinVerifyTrust(
			NULL,
			&WVTPolicyGUID,
			&WinTrustData);

		switch (lStatus)
		{
		case ERROR_SUCCESS:
			/*
			Signed file:
			- Hash that represents the subject is trusted.

			- Trusted publisher without any verification errors.

			- UI was disabled in dwUIChoice. No publisher or
			time stamp chain errors.

			- UI was enabled in dwUIChoice and the user clicked
			"Yes" when asked to install and run the signed
			subject.
			*/
			//TraceHelpers::TraceConsoleDown("The file \"%s\" is signed and the signature "
			//	L"was verified.\n",
			//	pwszSourceFile);
			retValue = true;
			break;

		case TRUST_E_NOSIGNATURE:
			// The file was not signed or had a signature 
			// that was not valid.

			// Get the reason for no signature.
			dwLastError = GetLastError();
			if (TRUST_E_NOSIGNATURE == dwLastError ||
				TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
				TRUST_E_PROVIDER_UNKNOWN == dwLastError)
			{
				// The file was not signed.
				//TraceHelpers::TraceConsoleDown("The file \"%s\" is not signed.\n",
				//	pwszSourceFile);
			}
			else
			{
				// The signature was not valid or there was an error 
				// opening the file.
				//TraceHelpers::TraceConsoleDown(L"An unknown error occurred trying to "
				//	L"verify the signature of the \"%s\" file.\n",
				//	pwszSourceFile);
			}

			break;

		case TRUST_E_EXPLICIT_DISTRUST:
			// The hash that represents the subject or the publisher 
			// is not allowed by the admin or user.
			//TraceHelpers::TraceConsoleDown("The signature is present, but specifically "
			//	L"disallowed.\n");
			break;

		case TRUST_E_SUBJECT_NOT_TRUSTED:
			// The user clicked "No" when asked to install and run.
			//TraceHelpers::TraceConsoleDown("The signature is present, but not "
			//	L"trusted.\n");
			break;

		case CRYPT_E_SECURITY_SETTINGS:
			/*
			The hash that represents the subject or the publisher
			was not explicitly trusted by the admin and the
			admin policy has disabled user trust. No signature,
			publisher or time stamp errors.
			*/
			//TraceHelpers::TraceConsoleDown("CRYPT_E_SECURITY_SETTINGS - The hash "
			//	L"representing the subject or the publisher wasn't "
			//	L"explicitly trusted by the admin and admin policy "
			//	L"has disabled user trust. No signature, publisher "
			//	L"or timestamp errors.\n");
			break;

		default:
			// The UI was disabled in dwUIChoice or the admin policy 
			// has disabled user trust. lStatus contains the 
			// publisher or time stamp chain error.
			//TraceHelpers::TraceConsoleDown("Error is: 0x%x.\n",
			//	lStatus);
			break;
		}

		// Any hWVTStateData must be released by a call with close.
		WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;

		lStatus = WinVerifyTrust(
			NULL,
			&WVTPolicyGUID,
			&WinTrustData);

		return (retValue);
	}


	const std::wstring GetCertsSigner(const std::wstring &fileName, PCCERT_CONTEXT &pCertContext)
	{
		int                nRetCode = 0;
		HCERTSTORE         hCertStore = NULL;
		HCRYPTMSG          hCryptMsg = NULL;
		DWORD              dwContentType = 0;
		DWORD              dwExpectedType =
			CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED;
		DWORD              cbData = 0;
		CMSG_SIGNER_INFO * pSignerInfo = NULL;
		CERT_INFO          CertInfo = { 0 };

		const int SUBJECT_NAME_BUFFER_SIZE = 512;
		WCHAR             pwszSubjectName[SUBJECT_NAME_BUFFER_SIZE];
		std::wstring	   retValue;


		// Retrieve the signed executable HCRYPTMSG and HCERTSTORE.
		if (!CryptQueryObject(CERT_QUERY_OBJECT_FILE,
			(LPCVOID)fileName.c_str(),
			dwExpectedType,
			CERT_QUERY_FORMAT_FLAG_BINARY,
			0,
			NULL,
			&dwContentType,
			NULL,
			&hCertStore,
			&hCryptMsg,
			NULL))
		{
			//nRetCode = GetLastError();
			//TraceHelpers::TraceConsoleDown("Error [%#x]: CryptQueryObject() failed.\n", nRetCode);
			return(retValue);
		}

		// Sanity check.
		_ASSERT(CERT_QUERY_CONTENT_PKCS7_SIGNED_EMBED == dwContentType);

		// Use low level messaging API to retrieve signer's info.
		if (!CryptMsgGetParam(hCryptMsg,
			CMSG_SIGNER_INFO_PARAM,
			0,
			NULL,
			&cbData))
		{
			//nRetCode = GetLastError();
			//TraceHelpers::TraceConsoleDown("Error [%#x]: CryptMsgGetParam() failed.\n", nRetCode);
			return(retValue);
		}

		if (!(pSignerInfo = (CMSG_SIGNER_INFO *)malloc(cbData)))
		{
			//nRetCode = E_OUTOFMEMORY;
			//TraceHelpers::TraceConsoleDown("Error [%#x]: malloc() failed.\n", nRetCode);
			return(retValue);
		}

		if (!CryptMsgGetParam(hCryptMsg,
			CMSG_SIGNER_INFO_PARAM,
			0,
			pSignerInfo,
			&cbData))
		{
			//nRetCode = GetLastError();
			//TraceHelpers::TraceConsoleDown("Error [%#x]: CryptMsgGetParam() failed.\n", nRetCode);
			return(retValue);
		}

		// Find signer's cert in store.
		CertInfo.Issuer = pSignerInfo->Issuer;
		CertInfo.SerialNumber = pSignerInfo->SerialNumber;

		if (!(pCertContext = CertFindCertificateInStore(hCertStore,
			X509_ASN_ENCODING |
			PKCS_7_ASN_ENCODING,
			0,

			CERT_FIND_SUBJECT_CERT,
			(LPVOID)&CertInfo,
			NULL)))
		{
			free(pSignerInfo);
			//nRetCode = GetLastError();
			//TraceHelpers::TraceConsoleDown("Error [%#x]: CryptMsgGetParam() failed.\n", nRetCode);
			return(retValue);
		}

		if (!(cbData = CertGetNameStringW(pCertContext,
			CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			pwszSubjectName,
			SUBJECT_NAME_BUFFER_SIZE)))
		{
			free(pSignerInfo);
			//nRetCode = CRYPT_E_NOT_FOUND;
			//TraceHelpers::TraceConsoleDown("Error [%#x]: CertGetNameString() failed.\n", nRetCode);
			return(retValue);
		}

		std::wstring cad(pwszSubjectName);
		retValue = cad;
		// Display signer's simple name.
		//TraceHelpers::TraceConsoleDown("%ls was signed by %ls.\n", fileName.c_str(), pwszSubjectName);

		free(pSignerInfo);
		return retValue;
	}

	bool CompareCertThumbPrint(PCCERT_CONTEXT certContext, const BYTE *thumbprintToVerify)
	{
		bool result = false;
		DWORD thumbPrintSize = 0;
		BYTE* thumbPrint = NULL;

		if (CryptHashCertificate(0, CALG_SHA1, 0, certContext->pbCertEncoded, certContext->cbCertEncoded, NULL, &thumbPrintSize))
		{
			thumbPrint = (BYTE *)calloc(thumbPrintSize, sizeof(BYTE));

			if (thumbPrint != NULL)
			{
				if (CryptHashCertificate(0, CALG_SHA1, 0, certContext->pbCertEncoded, certContext->cbCertEncoded, thumbPrint, &thumbPrintSize))
				{
					if (memcmp(thumbprintToVerify, thumbPrint, thumbPrintSize) == 0)
					{
						result = true;
					}
				}

				free(thumbPrint);
				thumbPrint = NULL;
			}
		}

		return result;
	}

	bool VerifyRootCAChainThumbPrint(PCCERT_CONTEXT &pCertContext)
	{
		bool ret = false;
		DWORD thumbPrintSize = 0;
		BYTE* thumbPrint = NULL;
		CERT_CHAIN_PARA chainPara = { 0 };
		PCCERT_CHAIN_CONTEXT pChainContext = NULL;
		PCERT_SIMPLE_CHAIN pSimpleChain = NULL;
		DWORD  dwTrustErrorMask = ~(CERT_TRUST_IS_NOT_TIME_NESTED | CERT_TRUST_IS_NOT_TIME_VALID | CERT_TRUST_REVOCATION_STATUS_UNKNOWN);
		DWORD  dwFlags = ~(CERT_CHAIN_CACHE_END_CERT | CERT_CHAIN_REVOCATION_CHECK_END_CERT);
		DWORD  dwErr = NO_ERROR;
		int numCerts = 0;

		if (CertGetCertificateChain(
			NULL,
			pCertContext,
			NULL,
			pCertContext->hCertStore,
			&chainPara,
			dwFlags,
			NULL,
			&pChainContext))
		{

			pSimpleChain = pChainContext->rgpChain[0];

			//Check revocation
			dwTrustErrorMask &= pSimpleChain->TrustStatus.dwErrorStatus;

			if (dwTrustErrorMask)
			{
				if (dwTrustErrorMask & CERT_TRUST_IS_OFFLINE_REVOCATION)
				{
					dwErr = NO_ERROR;
				}
				else if (dwTrustErrorMask & (CERT_TRUST_IS_PARTIAL_CHAIN | CERT_TRUST_IS_UNTRUSTED_ROOT))
				{
					dwErr = SEC_E_UNTRUSTED_ROOT;
				}
				else
				{
					dwErr = SEC_E_CERT_UNKNOWN;
				}
			}

			if (dwErr == NO_ERROR)
			{
				numCerts = pSimpleChain->cElement;
				if (numCerts > 0)
				{
					PCERT_CHAIN_ELEMENT* certPtr = pSimpleChain->rgpElement;

					for (int i = 0; i < numCerts; ++i)
					{
						PCCERT_CONTEXT rootCAContext = certPtr[i]->pCertContext;
						bool isValidChainRootCA = CompareCertThumbPrint(rootCAContext, CommonDefs::VERISIGN_CERT_THUMBPRINT)
							|| CompareCertThumbPrint(rootCAContext, CommonDefs::ADDTRUST_CERT_THUMBPRINT);
						if (isValidChainRootCA)
						{
							ret = true;
							break;
						}
					}
				}
			}
		}

		return ret;
	}


	bool IsTrustedSignedFile(const std::wstring &fileName)
	{
		bool ret = false;
		PCCERT_CONTEXT pCertContext = NULL;

		if (VerifyEmbeddedSignature(fileName.c_str()))
		{
			std::wstring signer = GetCertsSigner(fileName, pCertContext);
			if (signer.compare(0, CommonDefs::MICROSOFT_CORP_SIGNER_TO_VERIFY.length(), CommonDefs::MICROSOFT_CORP_SIGNER_TO_VERIFY) == 0)
			{
				//if (VerifyRootCAChainThumbPrint(pCertContext))
				//{
				ret = true;
				//
			}
		}

		return(ret);
	}

	bool GetBaseDir(const std::wstring& fullpathFile, std::wstring& basedir)
	{
		bool ret = false;
		size_t found;

		found = fullpathFile.find_last_of(L"/\\");
		if (found > 0)
		{
			basedir.assign(fullpathFile.substr(0, found));
			basedir.append(L"\\");
			ret = true;
		}
		return ret;
	}

	bool IsFileInSafePath(const std::wstring &fileName)
	{
		bool ret = false;
		TCHAR  sysroot[MAX_PATH] = { 0 };
		TCHAR  programFiles[MAX_PATH] = { 0 };
		std::vector<std::wstring> trustedLocations;
		std::wstring baseFileDirectory;

		std::wstring workingFileName(fileName);
		std::transform(workingFileName.begin(), workingFileName.end(), workingFileName.begin(), ::tolower);

		if (GetWindowsDirectory(sysroot, MAX_PATH) &&
			SHGetSpecialFolderPath(0, programFiles, CSIDL_PROGRAM_FILES, FALSE))
		{
			std::wstring workingSysroot(sysroot);
			std::wstring workingProgramFiles(programFiles);
			std::transform(workingSysroot.begin(), workingSysroot.end(), workingSysroot.begin(), ::tolower);
			std::transform(workingProgramFiles.begin(), workingProgramFiles.end(), workingProgramFiles.begin(), ::tolower);

			std::wstring trustedNETBinariesLocation(workingSysroot);
			trustedNETBinariesLocation.append(L"\\assembly\\");
			trustedLocations.push_back(trustedNETBinariesLocation);

			std::wstring trustedDriversBinariesLocation(workingSysroot);
			trustedDriversBinariesLocation.append(L"\\system32\\drivers\\");
			trustedLocations.push_back(trustedDriversBinariesLocation);

			std::wstring trustedProgramFilesLocation(workingProgramFiles);
			trustedProgramFilesLocation.append(L"\\oracle\\");
			trustedLocations.push_back(trustedProgramFilesLocation);
		}

		if ((GetBaseDir(workingFileName, baseFileDirectory)) &&
			(trustedLocations.size() > 0))
		{
			for (std::vector<std::wstring>::const_iterator locationIt = trustedLocations.begin();
				locationIt != trustedLocations.end();
				++locationIt)
			{
				if (baseFileDirectory.find(*locationIt) == 0)
				{
					ret = true;
					break;
				}
			}
		}

		return ret;
	}

	bool IsTrustedFile(const std::wstring &fileName)
	{
		bool ret = false;

		if (IsFileInSafePath(fileName) || IsTrustedSignedFile(fileName))
		{
			ret = true;
		}

		return ret;
	}


	bool IsSuspiciousSymbolsOnThreadCallStackBeginning(const SysmonXCommon::CallStackDataList &csElements)
	{
		bool ret = false;
		SysmonXCommon::CallStackDataPtr element = nullptr;
		static const size_t MIN_NUMBER_OF_STACK_FRAMES = 3;

		//traversing elements from last to first
		if (csElements.size() > MIN_NUMBER_OF_STACK_FRAMES)
		{
			for (SysmonXCommon::CallStackDataList::const_reverse_iterator csIT = csElements.rbegin(); csIT != csElements.rend(); ++csIT)
			{
				element = *csIT;

				if (element && (!element->symbolName.empty()) && (GeneralHelpers::StrContainsPatternInsensitive(element->symbolName, "RtlUserThreadStart")))
				{
					csIT++;

					if (csIT != csElements.rend())
					{
						element = *csIT;

						if (element && (!element->symbolName.empty()) &&
							(GeneralHelpers::StrContainsPatternInsensitive(element->symbolName, "BaseThreadInitThunk")))
						{
							csIT++;

							if (csIT != csElements.rend())
							{
								element = *csIT;

								if (element && (!element->symbolName.empty()) &&
									((GeneralHelpers::StrContainsPatternInsensitive(element->symbolName, "LoadlibraryExW")) ||
									(GeneralHelpers::StrContainsPatternInsensitive(element->symbolName, "LoadlibraryExA")) ||
									(GeneralHelpers::StrContainsPatternInsensitive(element->symbolName, "LoadlibraryEx"))))
								{
									ret = true;
									break;
								}
							}
						}
					}
				}
			}
		}
		
		return ret;
	}

	bool IsAPCInjectionPresent(const SysmonXCommon::CallStackDataList &csElements)
	{
		bool ret = false;
		SysmonXCommon::CallStackDataPtr element = nullptr;
		static const size_t MIN_NUMBER_OF_STACK_FRAMES = 3;

		//traversing elements from last to first
		if (csElements.size() > MIN_NUMBER_OF_STACK_FRAMES)
		{
			for (SysmonXCommon::CallStackDataList::const_reverse_iterator csIT = csElements.rbegin(); csIT != csElements.rend(); ++csIT)
			{
				element = *csIT;

				if (element && (!element->symbolName.empty()) && (GeneralHelpers::StrContainsPatternInsensitive(element->symbolName, "UserApcDispatcher")))
				{					
					csIT++;

					if (csIT != csElements.rend())
					{
						element = *csIT;

						if (element && (!element->symbolName.empty()))
						{							
							csIT++;

							if (csIT != csElements.rend())
							{
								element = *csIT;

								if (element && (!element->symbolName.empty()) &&
									(GeneralHelpers::StrContainsPatternInsensitive(element->symbolName, "Loadlibrary")))
								{
									ret = true;
									break;
								}
							}
						}
					}
				}
			}
		}

		return ret;
	}

	bool IsModuleInThreadsCollection(DWORD &dwProcessId, HANDLE &hProcess, const SysmonXCommon::ThreadsCollection &threadsData, const SysmonXCommon::ModulesCollection &modules, const std::wstring &module, DWORD &tid)
	{
		bool ret = false;

		if (hProcess != INVALID_HANDLE_VALUE)
		{
			std::wstring moduleBaseName;
			if (GeneralHelpers::GetBaseFileName(module, moduleBaseName))
			{
				for (SysmonXCommon::ThreadsCollection::const_iterator threadIT = threadsData.begin();
					threadIT != threadsData.end();
					++threadIT)
				{
					//HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadIT->th32ThreadID);
					HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, threadIT->th32ThreadID);
					if (hThread != INVALID_HANDLE_VALUE)
					{
						if (IsModuleInThread(dwProcessId, hProcess, threadIT->th32ThreadID, modules, moduleBaseName))
						{
							tid = threadIT->th32ThreadID;
							ret = true;
							break;
						}

						CloseHandle(hThread);
					}
				}
			}
		}

		return ret;
	}

	bool IsModuleInThread(const DWORD &dwProcessId, const HANDLE &hProcess, const DWORD &tid, const SysmonXCommon::ModulesCollection &modules, const std::wstring &module)
	{
		bool ret = false;

		std::string moduleToCompare = GeneralHelpers::WStrToStr(module);
		if (hProcess != INVALID_HANDLE_VALUE)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);
			if (hThread != INVALID_HANDLE_VALUE)
			{
				SysmonXCommon::CallStackDataList csElements;

				if (ThreadCallstackWalker::GetInstance().GetThreadCallStack(dwProcessId, hProcess, hThread, modules, csElements))
				{
					for (auto csIT = csElements.begin(); csIT != csElements.end(); csIT++)
					{
						SysmonXCommon::CallStackDataPtr element = *csIT;
						if (element && (moduleToCompare.compare(element->moduleName) == 0))
						{
							ret = true;
							break;
						}
					}
				}

				CloseHandle(hThread);
			}
		}

		return ret;
	}

	bool GetListOfCallStackModules(const DWORD &dwProcessId, const HANDLE &hProcess, const HANDLE &hThread, const SysmonXCommon::ModulesCollection &modules, SysmonXCommon::CallStackDataList &csElements, bool &suspiciousFound)
	{
		bool ret = false;
		static HANDLE currentHandleProcess = INVALID_HANDLE_VALUE;

		if (hProcess != INVALID_HANDLE_VALUE && hThread != INVALID_HANDLE_VALUE)
		{
			csElements.clear();

			if (ThreadCallstackWalker::GetInstance().GetThreadCallStack(dwProcessId, hProcess, hThread, modules, csElements))
			{
				ret = true;

				//Only checking last pushed element of the stack (thread start)
				if (!csElements.empty())
				{
					SysmonXCommon::CallStackDataPtr element = csElements.back();

					//check for suspicious empty module stack frames 
					if (element && (element->loadedImageName.empty()))
					{
						suspiciousFound = true;
					}
					//or symbols that indicates thread is starting from dll injection
					else if (SysmonXHelpers::IsSuspiciousSymbolsOnThreadCallStackBeginning(csElements))
					{
						suspiciousFound = true;
					}
					//Check for APC injection
					else if (SysmonXHelpers::IsAPCInjectionPresent(csElements))
					{
						suspiciousFound = true;
					}
				}				
			}
		}

		return ret;
	}

	bool ThreadCallstackWalker::GetThreadCallStack(const DWORD &pid, HANDLE hProcess, HANDLE hThread, const SysmonXCommon::ModulesCollection &modules, SysmonXCommon::CallStackDataList &csElements)
	{
		bool ret = false;
		bool goodToGetSymbols = true;

		static const DWORD MAX_NUMBER_OF_FRAMES = 64;
		if ((pid != 0) && (hProcess != INVALID_HANDLE_VALUE) && (hThread != INVALID_HANDLE_VALUE))
		{
			CONTEXT ctx = { 0 };
			ctx.ContextFlags = CONTEXT_CONTROL;
			bool threadContextWasRetrieved = false;

			//suspending the thread to get its exec context and resume it as quick as possible
			//Early resume will impact performance as we will have to check for thread status later
			if (SuspendThread(hThread) != ((DWORD)-1))
			{
				csElements.clear();

				if (GetThreadContext(hThread, &ctx) != FALSE)
				{
					threadContextWasRetrieved = true;
				}
				
				ResumeThread(hThread);
				
				if (threadContextWasRetrieved)
				{
					//Updating working process symbol initialization
					if (currentPID != pid)
					{
						if (currentProcessHandle != INVALID_HANDLE_VALUE)
						{
							SymCleanup(currentProcessHandle);
						}

						//closing previous initialization
						if (SymInitializeW(hProcess, NULL, TRUE))
						{
							currentPID = pid;
							currentProcessHandle = hProcess;
							
							/*
							//Manually initializing symbols
							for (auto modulesIT = modules.begin(); modulesIT != modules.end(); modulesIT++)
							{
								std::string moduleName(GeneralHelpers::WStrToStr(modulesIT->szExePath));
								PVOID addr = modulesIT->modBaseAddr;
								DWORD64 dwBaseAddr = reinterpret_cast<DWORD64>(addr);
								DWORD64 slret = SymLoadModuleEx(hProcess, NULL, moduleName.c_str(), NULL, dwBaseAddr, 0, NULL, 0);
								if (slret == 0)
								{
									//TODO: We might want to flag an error here
								}
							}		
							*/
						}
						else
						{
							goodToGetSymbols = false;
						}
					}

					if (goodToGetSymbols)
					{
						STACKFRAME64 sf;
						memset(&sf, 0, sizeof(STACKFRAME64));
						DWORD dwImageType = IMAGE_FILE_MACHINE_UNKNOWN;
#ifdef _M_IX86
						dwImageType = IMAGE_FILE_MACHINE_I386;
						sf.AddrPC.Offset = ctx.Eip;
						sf.AddrPC.Mode = AddrModeFlat;
						sf.AddrStack.Offset = ctx.Esp;
						sf.AddrStack.Mode = AddrModeFlat;
						sf.AddrFrame.Offset = ctx.Ebp;
						sf.AddrFrame.Mode = AddrModeFlat;
#elif _M_X64
						dwImageType = IMAGE_FILE_MACHINE_AMD64;
						sf.AddrPC.Offset = ctx.Rip;
						sf.AddrPC.Mode = AddrModeFlat;
						sf.AddrFrame.Offset = ctx.Rsp;
						sf.AddrFrame.Mode = AddrModeFlat;
						sf.AddrStack.Offset = ctx.Rsp;
						sf.AddrStack.Mode = AddrModeFlat;
#endif
						DWORD frameIT = 0;
						//Stack walk will continue until 
						// - process is still active
						// - thread is still active
						// - there are no more frames to read
						// - max number of frames have been reach
						while ((frameIT < MAX_NUMBER_OF_FRAMES) &&
							   (hProcess != NULL) &&
							   (hProcess != INVALID_HANDLE_VALUE) &&
							   (hThread != NULL) &&
							   (hThread != INVALID_HANDLE_VALUE))
//							   (GeneralHelpers::IsProcessStillRunning(hProcess)) &&
//							   (GeneralHelpers::IsThreadStillRunning(hThread)))
						{
							if (!StackWalk64(dwImageType, hProcess, hThread, &sf, &ctx, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) break;

							DWORD64 address = sf.AddrPC.Offset;
							if (address == 0) break;

							SysmonXCommon::CallStackDataPtr newElement(new SysmonXCommon::CallStackData());
							newElement->stackFrameAddress = address;

							// Get symbol name
							char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(CHAR)] = { 0 };
							PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
							pSymbol->MaxNameLen = MAX_SYM_NAME;
							pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
							if (SymFromAddr(currentProcessHandle, address, NULL, pSymbol))
							{
								newElement->symbolAvailable = pSymbol->Value;
								newElement->symbolAddress = pSymbol->Address;
								newElement->symbolName.assign(pSymbol->Name);
							}

							// Get module name
							IMAGEHLP_MODULE64 moduleInfo = { 0 };
							moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
							if (SymGetModuleInfo64(currentProcessHandle, address, &moduleInfo))
							{
								newElement->baseOfImageAddress = moduleInfo.BaseOfImage;
								newElement->imageSize = moduleInfo.ImageSize;
								newElement->timeDateStamp = moduleInfo.TimeDateStamp;
								newElement->checksum = moduleInfo.CheckSum;
								newElement->numSyms = moduleInfo.NumSyms;
								newElement->symType = moduleInfo.SymType;
								newElement->moduleName.assign(moduleInfo.ModuleName);
								newElement->imageName.assign(moduleInfo.ImageName);
								newElement->loadedImageName.assign(moduleInfo.LoadedImageName);
							}

							frameIT++;

							//Adding new stackframe element
							csElements.push_back(newElement);
						}
					}
				}

				if (!csElements.empty())
				{
					ret = true;
				}
			}
		}

		return ret;
	}
}



namespace WindowsHelpers
{
	bool GetBBProcessFromPID(const size_t targetPID, blackbone::Process &bbProcess)
	{
		bool ret = false;

		bbProcess.Attach((DWORD)targetPID);

		if (bbProcess.valid())
		{
			ret = true;
		}

		return ret;
	}

	bool GetMainModuleFromMemory(blackbone::Process &process, WindowsTypes::BytePTR &buffer, blackbone::pe::PEImage &module)
	{
		bool ret = false;

		if (process.valid())
		{
			blackbone::ModuleDataPtr mainModule = process.modules().GetMainModule();

			WindowsTypes::BytePTR localBuff(new uint8_t[mainModule->size]);

			memset(localBuff.get(), 0, mainModule->size);
			process.memory().Read(mainModule->baseAddress, mainModule->size, localBuff.get());

			if ((localBuff != nullptr) &&
				(module.Parse(localBuff.get()) == ERROR_SUCCESS) &&
				(module.base()) &&
				(module.imageSize() > 0))
			{
				buffer = localBuff;
				ret = true;
			}
		}

		return ret;
	}

	bool GetMainModuleFromDisk(blackbone::Process &process, blackbone::pe::PEImage &module)
	{
		bool ret = false;

		if (process.valid())
		{
			blackbone::ModuleDataPtr mainModule = process.modules().GetMainModule();

			if (!mainModule->fullPath.empty() &&
				(module.Load(mainModule->fullPath) == ERROR_SUCCESS) &&
				(module.base()) &&
				(module.imageSize() > 0))
			{
				ret = true;
			}
		}

		return ret;
	}
}
