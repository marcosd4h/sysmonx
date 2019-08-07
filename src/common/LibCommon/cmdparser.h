#pragma once

#include "windows.h"
#include <string>
#include <vector>

class CmdArgsParser 
{

public:
	CmdArgsParser() {};

	bool Initialize(int argc, wchar_t *argv[])
	{
		bool ret = false;

		for (int i = 1; i < argc; ++i)
		{
			this->m_cmdTokens.push_back(std::wstring(argv[i]));
			ret = true;
		}

		return ret;
	}

	const std::wstring& GetOptionValue(const std::wstring &cmdOption) const
	{
		std::vector<std::wstring>::const_iterator cmdIt;
		cmdIt = std::find(this->m_cmdTokens.begin(), this->m_cmdTokens.end(), cmdOption);

		if (cmdIt != this->m_cmdTokens.end() && ++cmdIt != this->m_cmdTokens.end())
		{
			return *cmdIt;
		}

		static const std::wstring empty(L"");
		return empty;
	}

	const bool GetTwoOptionValue(const std::wstring &cmdOption, std::wstring &value1, std::wstring &value2) const
	{
		bool ret = false;
		std::vector<std::wstring>::const_iterator cmdIt;
		cmdIt = std::find(this->m_cmdTokens.begin(), this->m_cmdTokens.end(), cmdOption);
		value1.clear();
		value2.clear();

		if (cmdIt != this->m_cmdTokens.end() && ++cmdIt != this->m_cmdTokens.end())
		{
			std::wstring workValue1(*cmdIt);

			if (!workValue1.empty())
			{
				value1.assign(*cmdIt);
				ret = true;

				if (cmdIt != this->m_cmdTokens.end() && ++cmdIt != this->m_cmdTokens.end())
				{
					std::wstring workValue2(*cmdIt);

					if (workValue2[0] != L'-')
					{
						value2.assign(workValue2);
					}					
				}
			}
		}

		return ret;
	}

	bool WasOptionRequested(const std::wstring &cmdOption) const
	{
		return std::find(this->m_cmdTokens.begin(), this->m_cmdTokens.end(), cmdOption) != this->m_cmdTokens.end();
	}

private:
	std::vector <std::wstring> m_cmdTokens;
};
