#pragma once

#include <windows.h>
#include <string>

namespace PayloadCommon
{
	enum PayloadMode
	{
		PYLD_CREATE_REMOTE_THREAD = 0x01,
		PYLD_NA,
	};

	inline const wchar_t* PayloadModeToString(PayloadMode value)
	{
		switch (value)
		{
			case PYLD_CREATE_REMOTE_THREAD:			return L"Payload via CreateRemoteThread()";
			default:								return L"[Unknown Payload Mode]";
		}
	}

	static const PayloadMode DEFAULT_PAYLOAD_TYPE = PYLD_CREATE_REMOTE_THREAD;
	static const bool DEFAULT_VERBOSITY_STATUS = false;
	static const unsigned int DEFAULT_MINIMUM_USERSPACE_PID = 4;
	static const std::wstring SYSTESTER_VERSION = L"v0.2";
}