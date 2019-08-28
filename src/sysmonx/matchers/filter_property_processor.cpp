#pragma once

#include "common.h"

bool FilterPropertyBase::IsThereAMatch(const MATCHING_TYPE_STRING &matchData)
{
	bool ret = false;

	if (!matchData.empty())
	{
		//Magic happens here! Prioritizing less operations with less performance impact
		if (IsReadyPolicyFilterCondition_IS() && EvalFilterCondition_IS(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_IS_NOT() && EvalFilterCondition_IS_NOT(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_EXCLUDES() && EvalFilterCondition_EXCLUDES(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_BEGIN_WITH() && EvalFilterCondition_BEGIN_WITH(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_END_WITH() && EvalFilterCondition_END_WITH(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_CONTAINS() && EvalFilterCondition_CONTAINS(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_IMAGE() && EvalFilterCondition_IMAGE(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_LESS_THAN() && EvalFilterCondition_LESS_THAN(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_MORE_THAN() && EvalFilterCondition_MORE_THAN(matchData))
		{
			ret = true;
		}
		else if (IsReadyPolicyFilterCondition_REGEX_MATCH() && EvalFilterCondition_REGEX_MATCH(matchData))
		{
			ret = true;
		}
	}

	return ret;
}

bool FilterPropertyBase::IsReadyToProcess()
{
	bool ret = false;

	if (IsReadyPolicyFilterCondition_IS() || IsReadyPolicyFilterCondition_IS_NOT() || IsReadyPolicyFilterCondition_CONTAINS() || IsReadyPolicyFilterCondition_EXCLUDES() ||
		IsReadyPolicyFilterCondition_BEGIN_WITH() || IsReadyPolicyFilterCondition_END_WITH() || IsReadyPolicyFilterCondition_LESS_THAN() || IsReadyPolicyFilterCondition_MORE_THAN() ||
		IsReadyPolicyFilterCondition_IMAGE() || IsReadyPolicyFilterCondition_REGEX_MATCH())
	{
		ret = true;
	}

	return ret;
}

bool FilterPropertyBase::AddEvalCondition(const EventFilterOperation &operation, const MATCHING_TYPE_STRING &stringData)
{
	bool ret = false;

	if ((operation >= EventFilterOperation::EVENT_FILTER_OPERATION_IS) &&
		(operation < EventFilterOperation::EVENT_FILTER_OPERATION_NA) &&
		(!stringData.empty()))
	{
		switch (operation)
		{
		case EVENT_FILTER_OPERATION_IS:
			m_evalPolicyCondition_IS.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_IS_NOT:
			m_evalPolicyCondition_IS_NOT.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_CONTAINS:
			m_evalPolicyCondition_CONTAINS.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_EXCLUDES:
			m_evalPolicyCondition_EXCLUDES.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_BEGIN_WITH:
			m_evalPolicyCondition_BEGIN_WITH.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_END_WITH:
			m_evalPolicyCondition_END_WITH.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_LESS_THAN:
			m_evalPolicyCondition_LESS_THAN.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_MORE_THAN:
			m_evalPolicyCondition_MORE_THAN.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_IMAGE:
			m_evalPolicyCondition_IMAGE.push_back(stringData);
			break;
		case EVENT_FILTER_OPERATION_REGEX_MATCH:
			try
			{
				boost::wregex expr{ stringData.c_str() };
				m_evalPolicyCondition_REGEX_MATCH.push_back(expr);
			}
			catch (...)
			{
				return ret;
			}
			break;
		default:
			break;
		}

		ret = true;
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_IS(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_IS())
	{
		for (auto itValue = m_evalPolicyCondition_IS.begin(); itValue != m_evalPolicyCondition_IS.end(); itValue++)
		{
			if (boost::algorithm::iequals(*itValue, value))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_IS_NOT(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_IS_NOT())
	{
		for (auto itValue = m_evalPolicyCondition_IS_NOT.begin(); itValue != m_evalPolicyCondition_IS_NOT.end(); itValue++)
		{
			if (!boost::algorithm::iequals(*itValue, value))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_CONTAINS(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_CONTAINS())
	{
		for (auto itValue = m_evalPolicyCondition_CONTAINS.begin(); itValue != m_evalPolicyCondition_CONTAINS.end(); itValue++)
		{
			if (boost::algorithm::icontains(value, *itValue))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_EXCLUDES(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_EXCLUDES())
	{
		for (auto itValue = m_evalPolicyCondition_EXCLUDES.begin(); itValue != m_evalPolicyCondition_EXCLUDES.end(); itValue++)
		{
			if (!boost::algorithm::icontains(value, *itValue))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_BEGIN_WITH(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_BEGIN_WITH())
	{
		for (auto itValue = m_evalPolicyCondition_BEGIN_WITH.begin(); itValue != m_evalPolicyCondition_BEGIN_WITH.end(); itValue++)
		{
			if (boost::algorithm::istarts_with(value, *itValue))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_END_WITH(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_END_WITH())
	{
		for (auto itValue = m_evalPolicyCondition_END_WITH.begin(); itValue != m_evalPolicyCondition_END_WITH.end(); itValue++)
		{
			if (boost::algorithm::iends_with(value, *itValue))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_LESS_THAN(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_LESS_THAN())
	{
		for (auto itValue = m_evalPolicyCondition_LESS_THAN.begin(); itValue != m_evalPolicyCondition_LESS_THAN.end(); itValue++)
		{
			//TODO: Horrible and slow due to exceptions. Refactor this
			try
			{
				if (boost::algorithm::ilexicographical_compare(value, *itValue))
				{
					ret = true;
					break;
				}
			}
			catch (...)
			{
				ret = false;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_MORE_THAN(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_MORE_THAN())
	{
		for (auto itValue = m_evalPolicyCondition_MORE_THAN.begin(); itValue != m_evalPolicyCondition_MORE_THAN.end(); itValue++)
		{
			//TODO: Horrible and slow due to exceptions. Refactor this
			try
			{
				if (!boost::algorithm::ilexicographical_compare(value, *itValue))
				{
					ret = true;
					break;
				}
			}
			catch (...)
			{
				ret = false;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_IMAGE(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	std::wstring ImageBase;
	if (!value.empty() && IsReadyPolicyFilterCondition_IMAGE() && GeneralHelpers::GetBaseFileName(value, ImageBase))
	{
		for (auto itValue = m_evalPolicyCondition_IMAGE.begin(); itValue != m_evalPolicyCondition_IMAGE.end(); itValue++)
		{
			if (boost::algorithm::iequals(*itValue, ImageBase))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool FilterPropertyBase::EvalFilterCondition_REGEX_MATCH(const MATCHING_TYPE_STRING &value)
{
	bool ret = false;

	if (!value.empty() && IsReadyPolicyFilterCondition_REGEX_MATCH())
	{
		for (auto itValue = m_evalPolicyCondition_REGEX_MATCH.begin(); itValue != m_evalPolicyCondition_REGEX_MATCH.end(); itValue++)
		{
			if (boost::regex_match(value, *itValue))
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}