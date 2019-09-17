#include "common.h"
#include "callbacks_etw_event_ids.h"

bool EventCollectorETW::SetupCallbackSysmonEventsHandler()
{
	bool ret = false;

	if (m_config.IsInitialized() && m_sysmonProvider)
	{
		m_sysmonProvider->add_on_event_callback([&](const EVENT_RECORD &record)
		{
			UINT32 currentEventID = SysmonXDefs::INVALID_EVENT_ID;
			UINT32 currentPID = SysmonXDefs::INVALID_PROCESS_ID;
			UINT32 currentOPCode = SysmonXDefs::DEFAULT_OPCODE;

			try
			{
				// Grabbing event schema
				krabs::schema schema(record);

				//Assigning working data
				currentEventID = schema.event_id();
				currentPID = schema.process_id();
				currentOPCode = schema.event_opcode();

				//Sysmon events parsing happens here
				//Just to avoid unnecesary calls everything happens on a big if-else section below
				//Elements will be fetch from ETW source one-by-one and will be precomputed into str when required (non-str types)

				//Event 1 - SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS
				if ((currentEventID == EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS))
				{
					krabs::parser parser(schema);
					krabs::binary workingBinary;

					//Creating new security event and transfering ownership
					SysmonXTypes::EventObject newEvent = SysmonXEvents::GetNewSecurityEvent(EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS);
										
					//Filling up Event common section
					newEvent->EventVersion = SysmonXDefs::CURRENT_EVENT_VERSION;
					newEvent->EventID = currentEventID;
					newEvent->EventCollectorTechID = EventCollectorTechID::EVENT_COLLECTOR_TECH_ETW;
					newEvent->EventCollectorVectorID = EventCollectorVectorID::EVENT_SUBTYPE_ETW_USER_CUSTOM;
					newEvent->EventCreationTimestamp = schema.timestamp();
					newEvent->EventCommonProcessID = currentPID;
					newEvent->EventETWProviderName.assign(ETWProvidersName::SYSMON_PROVIDER_NAME);

					//Filling up event-specific data
					if (!(parser.try_parse<std::wstring>(L"RuleName", newEvent->RuleName)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "RuleName", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"UtcTime", newEvent->UtcTime)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "UtcTime", currentEventID);
					}
					
					workingBinary.clear();
					if (parser.try_parse<krabs::binary>(L"ProcessGuid", workingBinary) && KrabsETWHelper::GetGUIDValue(workingBinary, newEvent->ProcessGuid.GUIDValue))
					{
						newEvent->ProcessGuid.PrecomputeStr();
					}
					else
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "ProcessGuid", currentEventID);
					}

					if (parser.try_parse<UINT32>(L"ProcessId", newEvent->ProcessId.IntValue))
					{
						newEvent->ProcessId.PrecomputeStr();
					}
					else
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "ProcessId", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"Image", newEvent->Image)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "Image", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"FileVersion", newEvent->FileVersion)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "FileVersion", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"Description", newEvent->Description)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "Description", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"Product", newEvent->Product)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "Product", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"Company", newEvent->Company)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "Company", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"OriginalFileName", newEvent->OriginalFileName)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "OriginalFileName", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"CommandLine", newEvent->CommandLine)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "CommandLine", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"CurrentDirectory", newEvent->CurrentDirectory)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "CurrentDirectory", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"User", newEvent->User)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "User", currentEventID);
					}

					workingBinary.clear();
					if (parser.try_parse<krabs::binary>(L"LogonGuid", workingBinary) && KrabsETWHelper::GetGUIDValue(workingBinary, newEvent->LogonGuid.GUIDValue))
					{
						newEvent->LogonGuid.PrecomputeStr();
					}
					else
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "LogonGuid", currentEventID);
					}

					/*
					workingBinary.clear();
					if (parser.try_parse<krabs::binary>(L"LogonId", workingBinary) && KrabsETWHelper::GetHEXINT64Value(workingBinary, newEvent->LogonId.ArrayValue))
					{
						newEvent->LogonGuid.PrecomputeStr();
					}
					else
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "LogonId", currentEventID);
					}
					*/

					if (parser.try_parse<UINT32>(L"TerminalSessionId", newEvent->TerminalSessionId.IntValue))
					{
						newEvent->TerminalSessionId.PrecomputeStr();
					}
					else
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "TerminalSessionId", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"IntegrityLevel", newEvent->IntegrityLevel)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "IntegrityLevel", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"Hashes", newEvent->Hashes)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "Hashes", currentEventID);
					}

					workingBinary.clear();
					if (parser.try_parse<krabs::binary>(L"ParentProcessGuid", workingBinary) && KrabsETWHelper::GetGUIDValue(workingBinary, newEvent->ParentProcessGuid.GUIDValue))
					{
						newEvent->ParentProcessGuid.PrecomputeStr();
					}
					else
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "ParentProcessGuid", currentEventID);
					}

					if (parser.try_parse<UINT32>(L"ParentProcessId", newEvent->ParentProcessId.IntValue))
					{
						newEvent->ParentProcessId.PrecomputeStr();
					}
					else
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "ParentProcessId", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"ParentImage", newEvent->ParentImage)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "ParentImage", currentEventID);
					}

					if (!(parser.try_parse<std::wstring>(L"ParentCommandLine", newEvent->ParentCommandLine)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "ParentCommandLine", currentEventID);
					}

					m_eventsProcessor.DispatchEvent(newEvent);
				}
			}
			catch (...)
			{
				m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing Event ID {}", currentEventID);
			}
		});

		ret = true;
	}

	return ret;
}