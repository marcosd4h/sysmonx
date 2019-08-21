#pragma once

#include <libcommon.h>

namespace CommonDefs
{
	static const uint32_t MAX_BUFFER_SIZE = 2048;
	static const std::string UPMARK = "[+] ";
	static const std::string DOWNMARK = "[-] ";
	static const std::string SEPARATOR = " - ";
	static const std::string DEFAULT_SEP = "";
	static const std::string ENDLINE = "\n";	
	static const std::wstring WUPMARK = L"[+] ";
	static const std::wstring WDOWNMARK = L"[-] ";
	static const std::wstring WSEPARATOR = L" - ";
	static const std::wstring WDEFAULT_SEP = L"";
	static const std::wstring WENDLINE = L"\n";
	static const std::wstring DEFAULT_VALUE = L"";
	static const DWORD SYSTEM_MIN_VALID_PID = 5; // System processes are found below this
	static const size_t DEFAULT_PAGE_SIZE = 4096;
	static const UINT32 RANDOM_STRING_LENGTH = 12;
	static const std::wstring DEFAULT_CSV_COLON_SEP = L";";
	static const std::wstring DEFAULT_CSV_COMMA_SEP = L",";
	
	static const std::wstring DEFAULT_RET_SEP = L"\n";
	static const std::wstring DEFAULT_SYSTEM_ROOT_DIRECTORY = L"c:\\windows\\";
	static const std::wstring DEFAULT_SYSTEM32_DIRECTORY = L"c:\\windows\\system32\\";
	//static const char * DEFAULT_LOG_PATTERN = "[%D][%H:%M:%S:%ems:%fus] [%^%L%$] %v";
	static const char * DEFAULT_LOG_PATTERN = "[%D][%H:%M:%S:%ems] [%^%L%$] %v";

	//Signatures Constants
	static const std::wstring MICROSOFT_CORP_SIGNER_TO_VERIFY = L"Microsoft Corporation";

	static const BYTE VERISIGN_CERT_THUMBPRINT[] = { 0x4E, 0xB6, 0xD5, 0x78, 0x49, 0x9B, 0x1C, 0xCF, 0x5F, 0x58, 0x1E, 0xAD, 0x56, 0xBE, 0x3D, 0x9B, 0x67, 0x44, 0xA5, 0xE5 };
	static const BYTE ADDTRUST_CERT_THUMBPRINT[] = { 0x02, 0xFA, 0xF3, 0xE2, 0x91, 0x43, 0x54, 0x68, 0x60, 0x78, 0x57, 0x69, 0x4D, 0xF5, 0xE4, 0x5B, 0x68, 0x85, 0x18, 0x68 };
}