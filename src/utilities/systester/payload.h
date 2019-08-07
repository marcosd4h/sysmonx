#pragma once

#include <string>
#include "customtypes.h"

class Payload
{

public:
	virtual bool Execute(const std::wstring &payload, const std::wstring &extra) = 0;

	const std::wstring GetDescription()
	{
		return m_description;
	}

	const PayloadCommon::PayloadMode GetMode()
	{
		return m_injectorMode;
	}

	Payload(std::wstring description, PayloadCommon::PayloadMode mode) :
		m_description(description), m_injectorMode(mode) {}

	Payload() :
		m_description(L""), m_injectorMode(PayloadCommon::PayloadMode::PYLD_NA) {}

private:
	std::wstring m_description;
	PayloadCommon::PayloadMode m_injectorMode;
};

