#include "common.h"
#include "callbacks_etw_event_ids.h"

bool EventCollectorETW::SetupCallbackKernelProcessCreateHandler()
{
	bool ret = false;

	static const wchar_t *ETW_PROVIDER_NAME = L"Windows-Kernel-Process";

	if (m_config.IsInitialized())
	{
		m_kernelProcessProvider.add_on_event_callback([&](const EVENT_RECORD &record)
		{
			UINT32 currentEventID = SysmonXDefs::INVALID_EVENT_ID;
			UINT32 currentPID = SysmonXDefs::INVALID_PROCESS_ID;

			try
			{
				// Grabbing event schema
				krabs::schema schema(record);

				//process start
				if ((schema.event_opcode() == 1))
				{
					krabs::parser parser(schema);

					ULONG SourceProcessID = record.EventHeader.ProcessId;
					uint32_t TargetProcessID = parser.parse<uint32_t>(L"ProcessId");
					std::string TargetProcessName = parser.parse<std::string>(L"ImageFileName");
					uint32_t TargetSessionID = parser.parse<uint32_t>(L"SessionId");
				}

				//Event 1 - SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS
				if ((schema.event_opcode() == EventID::SECURITY_EVENT_ID_SYSMONX_CREATE_PROCESS))
				{
					krabs::parser parser(schema);

					//Assigning working data
					currentEventID = schema.event_id();
					currentPID = schema.process_id();

					//Creating new security event and transfering ownership
					SysmonXTypes::EventObject newEvent = SysmonXEvents::GetNewSecurityEvent(EventID::SECURITY_EVENT_ID_SYSMON_CREATE_PROCESS);

					//Filling up Common Section of event
					newEvent->EventVersion = SysmonXDefs::CURRENT_EVENT_VERSION;
					newEvent->EventID = currentEventID;
					newEvent->EventCollectorTechID = EventCollectorTechID::EVENT_COLLECTOR_TECH_ETW;
					newEvent->EventCollectorVectorID = EventCollectorVectorID::EVENT_SUBTYPE_ETW_KERNEL;
					newEvent->EventCreationTimestamp = schema.timestamp();
					newEvent->EventETWProviderPID = currentPID;
					newEvent->EventETWProviderName.assign(ETW_PROVIDER_NAME);

					//Filling up event-specific data
					if (!(parser.try_parse<EventUINT32>(L"ProcessId", newEvent->ProcessId)))
					{
						m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing property {} at Event ID {}", "ProcessId", currentEventID);
					}

					m_eventsProcessor.DispatchEvent(newEvent);
				}

			}
			catch (...)
			{
				m_logger.Error("EventCollectorETW::SetupCallbackSysmonEventsHandler - There was a problem parsing Event ID {}", record.EventHeader.EventDescriptor.Id);
			}
		});

		ret = true;
	}


	return ret;
}