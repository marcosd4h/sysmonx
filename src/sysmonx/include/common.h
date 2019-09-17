#pragma once

//LibCommon
#include <libcommon.h>

//SysmonX
#include "cmdparser.h"
#include "localdefs.h"
#include "localtypes.h"
#include "securitytypes.h"
#include "localhelpers.h"
#include "interfaces.h"
#include "configmanager.h"
#include "reportmanager.h"
#include "matchingengine.h"
#include "eventsmanager.h"
#include "executionflows.h"
#include "matchingengine.h"
#include "rulesmanager.h"
#include "servicemanager.h"
#include "executionflows.h"
#include "cachemanager.h"

//Matchers
#include "matchers\filter_property_processor.h"
#include "matchers\match_event_base.h"
#include "matchers\match_event_sysmon_create_process.h"
#include "matchers\match_event_sysmox_powershell.h"

//Reports
#include "reporters\report_channel_debug_events.h"
#include "reporters\report_channel_etw.h"
#include "reporters\report_channel_eventlog.h"
#include "reporters\report_channel_logfile.h"

//Event Collectors
#include "collectors\event_collector_etw.h"
#include "collectors\event_collector_dummy.h"