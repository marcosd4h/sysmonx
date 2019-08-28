#pragma once

#include "windows.h"
#include <string>
#include <vector>
#include <algorithm>

class CmdArgsParser 
{
	
public:

	enum CaseMode
	{
		CASE_MODE_SENSITIVE = 0,
		CASE_MODE_INSENSITIVE,
		CASE_MODE_NA
	};

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

	bool GetOptionValue(const std::wstring &cmdOption, std::wstring &value, int mode = CaseMode::CASE_MODE_SENSITIVE) const
	{
		bool ret = false;
		std::vector<std::wstring> *tokensCollection = nullptr;
		std::vector<std::wstring>::const_iterator cmdIt;
		
		if (!cmdOption.empty())
		{

			value.clear();
			std::wstring workingOptionStr(cmdOption);

			if (mode == CaseMode::CASE_MODE_SENSITIVE)
			{
				tokensCollection = (std::vector<std::wstring> *) &(this->m_cmdTokens);
			}
			else
			{
				std::transform(workingOptionStr.begin(), workingOptionStr.end(), workingOptionStr.begin(), ::towlower);
				tokensCollection = (std::vector<std::wstring> *) &(this->m_cmdITokens);
			}

			cmdIt = std::find((*tokensCollection).begin(), (*tokensCollection).end(), workingOptionStr);
			if (cmdIt != (*tokensCollection).end())
			{
				if (++cmdIt != (*tokensCollection).end())
				{
					std::wstring workArgValue(*cmdIt);

					if (workArgValue[0] != L'-')
					{
						value.assign(workArgValue);
					}
				}

				ret = true;
			}
		}

		return ret;
	}

	const bool GetTwoOptionValue(const std::wstring &cmdOption, std::wstring &value1, std::wstring &value2, int mode = CaseMode::CASE_MODE_SENSITIVE) const
	{
		bool ret = false;
		std::vector<std::wstring> *tokensCollection = nullptr;
		std::vector<std::wstring>::const_iterator cmdIt;
		value1.clear();
		value2.clear();

		if (!cmdOption.empty())
		{
			std::wstring workingOptionStr(cmdOption);

			if (mode == CaseMode::CASE_MODE_SENSITIVE)
			{
				tokensCollection = (std::vector<std::wstring> *) &(this->m_cmdTokens);
			}
			else
			{
				std::transform(workingOptionStr.begin(), workingOptionStr.end(), workingOptionStr.begin(), ::towlower);
				tokensCollection = (std::vector<std::wstring> *) &(this->m_cmdITokens);
			}

			cmdIt = std::find((*tokensCollection).begin(), (*tokensCollection).end(), workingOptionStr);

			if (cmdIt != (*tokensCollection).end() && ++cmdIt != (*tokensCollection).end())
			{
				std::wstring workValue1(*cmdIt);
				if (!workValue1.empty() && (workValue1[0] != L'-'))
				{
					value1.assign(workValue1);
					ret = true;

					if (cmdIt != (*tokensCollection).end() && ++cmdIt != (*tokensCollection).end())
					{
						std::wstring workValue2(*cmdIt);

						if (!workValue2.empty() && (workValue2[0] != L'-'))
						{
							value2.assign(workValue2);
						}
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
