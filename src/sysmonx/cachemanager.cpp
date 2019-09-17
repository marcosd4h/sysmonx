#include "common.h"

bool ReportManager::Initialize()
{
	boost::lock_guard<boost::mutex> lk(m_mutex);

	bool ret = true;

	m_logger.Trace("ReportManager::Initialize - About to initialize report channels");

	if (m_config.IsInitialized() && !IsInitialized())
	{
		//Initializgin reports available
		m_enabledReportersLists = m_config.GetReportOutputList();

		//Checking if there are reports enabled throug configuration
		if (m_enabledReportersLists.empty())
		{
			//If no of them was added, just returning true and avoid adding report channels
			m_isInitialized = true;
			m_foundReportersEnabledByConfig = false;
			ret = true;
		}
		else
		{
			//Report channels requested trough configuration
			//Adding reporters known at compile time
			ReporterChannelObject reportChannelDebugEvents = std::make_shared<ReportChannelDebugEvents>();
			ReporterChannelObject reportChannelETW = std::make_shared<ReportChannelETW>();
			ReporterChannelObject reportChannelEventLog = std::make_shared<ReportChannelEventlog>();
			ReporterChannelObject reportChannelLogfile = std::make_shared<ReportChannelLogfile>();

			//Registering ETW and Event Log ETW provider
			EventRegisterSysmonX();

			//Debug events report channel
			if (reportChannelDebugEvents->Initialize() && AddNewReporter(SysmonXTypes::ReportChannelID::REPORT_CHANNEL_DEBUG_EVENTS, reportChannelDebugEvents))
			{
				m_logger.Trace("ReportManager::Initialize - Debug Events report channel was succesfully added");
			}
			else
			{
				m_logger.Error("ReportManager::Initialize - There was a problem adding Debug Events report channel");
			}

			//ETW report channel
			if (reportChannelETW->Initialize() && AddNewReporter(SysmonXTypes::ReportChannelID::REPORT_CHANNEL_ETW, reportChannelETW))
			{

				m_logger.Trace("ReportManager::Initialize - ETW report channel was succesfully added");
			}
			else
			{
				m_logger.Error("ReportManager::Initialize - There was a problem adding ETW report channel");
			}

			//Logfile report channel
			if (reportChannelLogfile->Initialize() && AddNewReporter(SysmonXTypes::ReportChannelID::REPORT_CHANNEL_LOGFILE, reportChannelLogfile))
			{
				m_logger.Trace("ReportManager::Initialize - Logfile report channel was succesfully added");
			}
			else
			{
				m_logger.Error("ReportManager::Initialize - There was a problem adding Logfile report channel");
			}
			
			//Eventlog report channel
			if ( reportChannelEventLog->Initialize() && AddNewReporter(SysmonXTypes::ReportChannelID::REPORT_CHANNEL_EVENTLOG, reportChannelEventLog))
			{
				m_logger.Trace("ReportManager::Initialize - Eventlog report channel was succesfully added");
			}
			else
			{
				m_logger.Error("ReportManager::Initialize - There was a problem adding Eventlog report channel");
			}

			m_isInitialized = true;
			m_foundReportersEnabledByConfig = true;
			ret = true;
		}
	}
	else
	{
		ret = true;
	}

	return ret;
}


void ReportManager::SerializedReportHandler(const SysmonXTypes::ReportObject &newReport)
{
	boost::lock_guard<boost::mutex> lk(m_mutex);

	try
	{
		//Check if all reporters should be called or just the enabled ones
		if (IsInitialized() && 
			m_foundReportersEnabledByConfig &&
			!m_reporters.empty() &&
		   (newReport->ChannelID != SysmonXTypes::ReportChannelID::REPORT_CHANNEL_NA))
		{

			if (newReport->ChannelID == SysmonXTypes::ReportChannelID::REPORT_CHANNEL_META_ALL)
			{
				for (auto reporterIT = m_reporters.begin(); reporterIT != m_reporters.end(); reporterIT++)
				{
					std::shared_ptr<ReportChannelBase> &handleReportChannel = reporterIT->second;

					//Calling all reporters
					handleReportChannel->SendReportFinding(newReport);
				}
			}
			else
			{
				//Calling only enabled reporters
				for (auto enabledReporterIT = m_enabledReportersLists.begin(); enabledReporterIT != m_enabledReportersLists.end(); enabledReporterIT++)
				{
					SysmonXTypes::ReportChannelID reportID = *enabledReporterIT;

					SysmonXTypes::ReportersContainer::iterator reporterIT = m_reporters.find(reportID);
					if (reporterIT != m_reporters.end())
					{
						std::shared_ptr<ReportChannelBase> &handleReportChannel = reporterIT->second;
						if (handleReportChannel && handleReportChannel->IsEnabled())
						{
							//Reporter found, now calling it handling method
							handleReportChannel->SendReportFinding(newReport);
						}
					}
				}
			}
		}
	}
	catch (...)
	{
		m_logger.Trace("ReportManager::NewReportHandler - There was a problem handling new report request");
	}
}

bool ReportManager::SetTargetReportChannels(const SysmonXTypes::ReportOutputList &reportList)
{
	boost::lock_guard<boost::mutex> lk(m_mutex);
	bool ret = true;

	if (!reportList.empty())
	{
		m_logger.Trace("ReportManager::SetTargetReportChannels - About to set target report channels");

		m_targetConfigurationReporters.assign(reportList.begin(), reportList.end());

		if (!m_targetConfigurationReporters.empty())
		{
			//Iterating through list of reporters requested through m_configuration
			for (auto reportIT = m_targetConfigurationReporters.begin(); reportIT != m_targetConfigurationReporters.end(); reportIT++)
			{
				ReportChannelID &workingReportID = *reportIT;

				//Now checking if given report channel is on the list of initialized reporters
				SysmonXTypes::ReportersContainer::iterator reportChannelIT = m_reporters.find(workingReportID);
				if(reportChannelIT != m_reporters.end())
				{
					std::shared_ptr<ReportChannelBase> &reportChannelInstance = reportChannelIT->second;
					if (reportChannelInstance)
					{
						//Reporting channel found, enabling it
						reportChannelInstance->SetEnableStatus(true);
					}
				}
			}

			ret = true;
		}		
	}

	return ret;
}

//TODO: Optimize this by precomputing enabled channels or changing reportOutputList type to a map with deterministic access
bool ReportManager::WasReportChannelEnabledThroughConfig(const SysmonXTypes::ReportChannelID &reportID)
{
	bool ret = false;

	if (m_foundReportersEnabledByConfig && (reportID != SysmonXTypes::ReportChannelID::REPORT_CHANNEL_NA))
	{
		if (reportID == SysmonXTypes::ReportChannelID::REPORT_CHANNEL_META_ALL)
		{
			ret = true;
		}
		else
		{
			for (auto it = m_enabledReportersLists.begin(); it != m_enabledReportersLists.end(); it++)
			{
				const ReportChannelID &enabledReportID = *it;

				if (reportID == enabledReportID)
				{
					ret = true;
					break;
				}
			}
		}
	}

	return ret;
}
