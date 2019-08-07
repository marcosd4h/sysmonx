#pragma once

#include "../common.h"
#include "../payload.h"

class PayloadCreateUserThread : public Payload
{
public:
	bool Execute(const std::wstring &payload, const std::wstring &extra);

	PayloadCreateUserThread() :
		Payload(PayloadCommon::PayloadModeToString(PayloadCommon::PayloadMode::PYLD_CREATE_REMOTE_THREAD),
			PayloadCommon::PayloadMode::PYLD_CREATE_REMOTE_THREAD) {}

private:

};