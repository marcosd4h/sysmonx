#pragma once

#include "windows.h"
#include <string>
#include <vector>
#include <algorithm>

class CmdArgsParser 
{

public:
	CmdArgsParser() = default;
	~CmdArgsParser() = default;

	bool Initialize(int argc, wchar_t *argv[])
	{
		bool ret = false;

		for (int i = 1; i < argc; ++i)
		{
			std::wstring workingStr(argv[i]);
			this->m_cmdTokens.push_back(workingStr);
			std::transform(workingStr.begin(), workingStr.end(), workingStr.begin(), ::towlower);
			this->m_cmdITokens.push_back(workingStr);
		}

		if (!this->m_cmdTokens.empty())
		{
			ret = true;
		}

		return ret;
	}

	const std::wstring& GetOptionValue(const std::wstring &cmdOption) const
	{
		static const std::wstring empty;

		std::vector<std::wstring>::const_iterator cmdIt;
		cmdIt = std::find(this->m_cmdTokens.begin(), this->m_cmdTokens.end(), cmdOption);

		if (cmdIt != this->m_cmdTokens.end() && ++cmdIt != this->m_cmdTokens.end())
		{
			return *cmdIt;
		}
		else
		{
			return empty;
		}		
	}

	const std::wstring& GetOptionValueInsensitive(const std::wstring &cmdOption) const
	{
		static const std::wstring empty;

		std::wstring workingStr(cmdOption);
		std::transform(workingStr.begin(), workingStr.end(), workingStr.begin(), ::towlower);

		std::vector<std::wstring>::const_iterator cmdIt;
		cmdIt = std::find(this->m_cmdITokens.begin(), this->m_cmdITokens.end(), workingStr);
		if (cmdIt != this->m_cmdITokens.end() && ++cmdIt != this->m_cmdITokens.end())
		{
			return *cmdIt;
		}
		else
		{
			return empty;
		}
	}

	const bool GetTwoOptionValue(const std::wstring &cmdOption, std::wstring &value1, std::wstring &value2) const
	{
		bool ret = false;
		std::vector<std::wstring>::const_iterator cmdIt;
		value1.clear();
		value2.clear();

		cmdIt = std::find(this->m_cmdTokens.begin(), this->m_cmdTokens.end(), cmdOption);
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

	const bool GetTwoOptionValueInsensitive(const std::wstring &cmdOption, std::wstring &value1, std::wstring &value2) const
	{
		bool ret = false;
		std::vector<std::wstring>::const_iterator cmdIt;

		std::wstring workingStr(cmdOption);
		std::transform(workingStr.begin(), workingStr.end(), workingStr.begin(), ::towlower);
		value1.clear();
		value2.clear();

		cmdIt = std::find(this->m_cmdITokens.begin(), this->m_cmdITokens.end(), workingStr);
		if (cmdIt != this->m_cmdITokens.end() && ++cmdIt != this->m_cmdITokens.end())
		{
			std::wstring workValue1(*cmdIt);

			if (!workValue1.empty())
			{
				value1.assign(*cmdIt);
				ret = true;

				if (cmdIt != this->m_cmdITokens.end() && ++cmdIt != this->m_cmdITokens.end())
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

	bool WasOptionRequestedInsensitive(const std::wstring &cmdOption) const
	{
		std::wstring workingStr(cmdOption);
		std::transform(workingStr.begin(), workingStr.end(), workingStr.begin(), ::towlower);
		return std::find(this->m_cmdITokens.begin(), this->m_cmdITokens.end(), workingStr) != this->m_cmdITokens.end();
	}

private:
	std::vector <std::wstring> m_cmdTokens;
	std::vector <std::wstring> m_cmdITokens;
};
