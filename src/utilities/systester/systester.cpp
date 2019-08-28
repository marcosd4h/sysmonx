// minjector.cpp : Defines the entry point for the console application.
//
#include "common.h"
#include "payloadManager.h"

void ShowHelp()
{
	std::wcerr << L"Systester - Sysmon test Utility" << std::endl;
	std::wcerr << L"Version: " << PayloadCommon::SYSTESTER_VERSION << std::endl << std::endl;

	std::wcerr << L"Available Tests:" << std::endl;
	std::wcerr << L"  " << PayloadCommon::PYLD_CREATE_REMOTE_THREAD << L" - " << PayloadCommon::PayloadModeToString(PayloadCommon::PYLD_CREATE_REMOTE_THREAD) << std::endl;

	std::wcerr << L"Available Options:" << std::endl;
	std::wcerr << L"  -h for help" << std::endl;
	std::wcerr << L"  -m <test_to_use>" << std::endl;
	std::wcerr << L"  -s <testpayload>" << std::endl;
	std::wcerr << L"  -t <misc>" << std::endl << std::endl;

	std::wcerr << L"Usage Examples: " << std::endl;
	std::wcerr << L"  TBD" << std::endl;
}

int wmain(int argc, wchar_t *argv[])
{
	int ret = 0;
	UINT32 PayloadMode = PayloadCommon::DEFAULT_PAYLOAD_TYPE;
	std::wstring PayloadModeName;
	std::wstring fullPathToFileToInject;
	CmdArgsParser inputCmds;

	if (inputCmds.Initialize(argc, argv) && 
		(inputCmds.WasOptionRequested(L"-h") ||
		 !inputCmds.WasOptionRequested(L"-s") || 
		 !inputCmds.WasOptionRequested(L"-t") || 
		 !inputCmds.WasOptionRequested(L"-m")))
	{
		std::wcerr << L"[-] Make sure to provide all the required arguments" << std::endl;
		ShowHelp();
		return 1;
	}

	std::wstring codeToInject;
	
	inputCmds.GetOptionValue(L"-s", codeToInject);
	if (codeToInject.empty() ||
		!SystesterHelpers::IsValidFile(codeToInject) || 
		!SystesterHelpers::GetFullPathToFile(codeToInject, fullPathToFileToInject))
	{
		std::wcerr << L"[-] DLL file to inject cannot be found" << std::endl;
		ShowHelp();
		return 1;
	}

	std::wstring targetToInject;
	inputCmds.GetOptionValue(L"-t", targetToInject);
	if (targetToInject.empty())
	{
		std::wcerr << L"[-] Target PID/Process was not provided" << std::endl;
		ShowHelp();
		return 1;
	}

	std::wstring targetMode;
	inputCmds.GetOptionValue(L"-m", targetMode);
	if (targetMode.empty())
	{
		std::wcerr << L"[-] Target mode was not provided" << std::endl;
		ShowHelp();
		return 1;
	}
	else
	{
		PayloadMode = SystesterHelpers::ToInteger(targetMode);
	}

	//Real work starts here
	
	PayloadManager manager;
	auto payloadtest1 = std::make_shared<PayloadCreateUserThread>();

	manager.AddInjector(payloadtest1);
			
	if (manager.RunPayload((PayloadCommon::PayloadMode)PayloadMode, fullPathToFileToInject, targetToInject))
	{
		std::wcout << "[+] Test was succesfully performed!" << std::endl;
	}
	else
	{
		std::wcout << "[-] There was a problem performing the requested test: " << 
						PayloadCommon::PayloadModeToString((PayloadCommon::PayloadMode)PayloadMode) << 
						std::endl;
	}

    return ret;
}

