#pragma once

#include "common.h"

namespace SysmonXTypes
{
	namespace data = cista::offset;

	//Configuration Data - Initialization is not allowed due to limitation of serialization lib
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
		CommonTypes::SerializedWCharVector ConfigFileContent;
		CommonTypes::SerializedWCharVector HashAlgorithmToUse;
	};

	//Common Types
	typedef UINT32		PROCESS_ID_T;
	typedef UINT32		THREAD_ID_TYPE;

	//Security Events Types
	typedef std::wstring		MATCHING_TYPE_STRING;
	typedef UINT64				MATCHING_TYPE_UINT64;
	typedef UINT32				MATCHING_TYPE_UINT32;
	typedef UINT16				MATCHING_TYPE_UINT16;
	typedef UINT8				MATCHING_TYPE_UINT8;

	//Event types - string based types
	typedef std::wstring		EventWString;
	typedef std::string			EventString;
	typedef std::wstring		EventPropertyName;
	typedef std::wstring		EventIDName;
	typedef std::wstring		EventPropertyName;
	typedef std::wstring		EventConditionFilterName;
	typedef std::wstring		EventName;
	typedef std::wstring		EventAggregationName;
	typedef std::vector<EventPropertyName> EventPropertyNameContainer;
	typedef std::vector<std::wstring> WStringCollection;;

	//Event types - composed types
	struct EventBool
	{
	public:
		EventBool() = default;

		EventBool(const bool& value)
		{
			BoolValue = value;
			if (value)
			{
				StrValue.assign(L"1");
			}
			else
			{
				StrValue.assign(L"0");
			}
		}

		EventBool& operator=(const bool& value)
		{
			BoolValue = value;
			if (value)
			{
				StrValue.assign(L"1");
			}
			else
			{
				StrValue.assign(L"0");
			}
			return *this;
		}

		void PrecomputeStr()
		{
			if (BoolValue)
			{
				StrValue.assign(L"1");
			}
			else
			{
				StrValue.assign(L"0");
			}
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const bool& GetValue() { return BoolValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator bool() const & { return BoolValue; }

		//Public internal members
		bool BoolValue = false;
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventUINT16
	{
	public:
		EventUINT16() = default;

		EventUINT16(const UINT16& value)
		{
			IntValue = value;
			boost::conversion::try_lexical_convert(value, StrValue);
		}

		EventUINT16& operator=(const UINT16& value)
		{
			IntValue = value;
			boost::conversion::try_lexical_convert(value, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			boost::conversion::try_lexical_convert(IntValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const UINT16& GetValue() { return IntValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator UINT16() const & { return IntValue; }

		UINT16 IntValue = 0;
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventUINT32
	{
	public:
		EventUINT32() = default;

		EventUINT32(const UINT32& value)
		{
			IntValue = value;
			boost::conversion::try_lexical_convert(value, StrValue);
		}

		EventUINT32& operator=(const UINT32& value)
		{
			IntValue = value;
			boost::conversion::try_lexical_convert(value, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			boost::conversion::try_lexical_convert(IntValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const UINT32& GetValue() { return IntValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator UINT32() const & { return IntValue; }

		//Public internal members
		UINT32 IntValue = 0;
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventUINT64
	{
	public:
		EventUINT64() = default;

		EventUINT64(const UINT64& value)
		{
			IntValue = value;
			boost::conversion::try_lexical_convert(value, StrValue);
		}

		EventUINT64& operator=(const UINT64& value)
		{
			IntValue = value;
			boost::conversion::try_lexical_convert(value, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			boost::conversion::try_lexical_convert(IntValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const UINT64& GetValue() { return IntValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator UINT64() const & { return IntValue; }

		//Public internal members
		UINT64 IntValue = 0;
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventLARGEINT
	{
	public:
		EventLARGEINT() = default;

		EventLARGEINT(const LARGE_INTEGER& value)
		{
			LargeIntValue = value;
			boost::conversion::try_lexical_convert(value.QuadPart, StrValue);
		}

		EventLARGEINT& operator=(const LARGE_INTEGER& value)
		{
			LargeIntValue = value;
			boost::conversion::try_lexical_convert(LargeIntValue.QuadPart, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			boost::conversion::try_lexical_convert(LargeIntValue.QuadPart, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const LARGE_INTEGER& GetValue() { return LargeIntValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator LARGE_INTEGER() const & { return LargeIntValue; }

		//Public internal members
		LARGE_INTEGER LargeIntValue = { 0 };
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventGUID
	{
	public:
		EventGUID() = default;

		EventGUID(const GUID& value)
		{
			GUIDValue = value;
			GeneralHelpers::GetWStringFromGUID(GUIDValue, StrValue);
		}

		EventGUID& operator=(const GUID& value)
		{
			GUIDValue = value;
			GeneralHelpers::GetWStringFromGUID(GUIDValue, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			GeneralHelpers::GetWStringFromGUID(GUIDValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const GUID& GetValue() { return GUIDValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator GUID() const & { return GUIDValue; }

		//Public internal members
		GUID GUIDValue = { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventArrayBool
	{
	public:
		EventArrayBool() = default;

		EventArrayBool(const CommonTypes::ArrayBoolT& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		EventArrayBool& operator=(const CommonTypes::ArrayBoolT& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const CommonTypes::ArrayBoolT& GetValue() { return ArrayValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator CommonTypes::ArrayBoolT() const & { return ArrayValue; }

		//Public internal members
		CommonTypes::ArrayBoolT ArrayValue;
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventArray16
	{
	public:
		EventArray16() = default;

		EventArray16(const CommonTypes::Array16T& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		EventArray16& operator=(const CommonTypes::Array16T& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const CommonTypes::Array16T& GetValue() { return ArrayValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator CommonTypes::Array16T() const & { return ArrayValue; }

		//Public internal members
		CommonTypes::Array16T ArrayValue;
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventArray32
	{
	public:
		EventArray32() = default;

		EventArray32(const CommonTypes::Array32T& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		EventArray32& operator=(const CommonTypes::Array32T& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const CommonTypes::Array32T& GetValue() { return ArrayValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator CommonTypes::Array32T() const & { return ArrayValue; }

		//Public internal members
		CommonTypes::Array32T ArrayValue;
		MATCHING_TYPE_STRING StrValue;
	};


	struct EventArray64
	{
	public:
		EventArray64() = default;

		EventArray64(const CommonTypes::Array64T& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		EventArray64& operator=(const CommonTypes::Array64T& value)
		{
			ArrayValue = value;
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
			return *this;
		}

		void PrecomputeStr()
		{
			GeneralHelpers::GetStringFromArray(ArrayValue, StrValue);
		}

		const MATCHING_TYPE_STRING& GetWString() { return StrValue; }
		const CommonTypes::Array64T& GetValue() { return ArrayValue; }
		operator MATCHING_TYPE_STRING() const & { return StrValue; }
		operator CommonTypes::Array64T() const & { return ArrayValue; }

		//Public internal members
		CommonTypes::Array64T ArrayValue;
		MATCHING_TYPE_STRING StrValue;
	};
}