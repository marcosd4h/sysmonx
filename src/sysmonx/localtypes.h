#pragma once

#include "common.h"

namespace SysmonXTypes
{
	namespace data = cista::offset;

	//Configuration
	struct ConfigSerializedData
	{
		int Version;
		int ShouldCheckSignatureRevocation;
		int OptionsFlags;
		int WorkerThreads;
		int CollectionServiceLoggingVerbosity;
		CommonTypes::SerializedUIntVector CollectionServiceLoggingChannel;
		CommonTypes::SerializedUIntVector CollectionServiceReportChannel;
		CommonTypes::SerializedWCharVector CollectionServiceName;
		CommonTypes::SerializedWCharVector CollectionServiceLogfile;
		CommonTypes::SerializedWCharVector PreviousCollectionServiceName;
		CommonTypes::SerializedWCharVector PreviousBackend32BitsName;
		CommonTypes::SerializedWCharVector PreviousBackend64BitsName;
		CommonTypes::SerializedWCharVector Backend32BitsName;
		CommonTypes::SerializedWCharVector Backend64BitsName;
		CommonTypes::SerializedCistaStringVector ProcessesLoadingModules;
		CommonTypes::SerializedCistaStringVector ProcessesNetworkConnections;
		CommonTypes::SerializedWCharVector PreviousWorkingDirectory;
		CommonTypes::SerializedWCharVector WorkingDirectory;
		CommonTypes::SerializedWCharVector ConfigFile;
		CommonTypes::SerializedWCharVector HashAlgorithmToUse;
	};

}