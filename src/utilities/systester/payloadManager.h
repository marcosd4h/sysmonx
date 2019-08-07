#pragma once

#include "common.h"
#include "payload.h"
#include "payloads/payload_create_user_thread.h"

class PayloadManager
{
public:
	PayloadManager() {};

	template <typename T>
	void AddInjector(std::shared_ptr<T>& injector)
	{
		m_payloads.push_back(injector);
	}

	bool RunPayload(const PayloadCommon::PayloadMode mode, 
					 const std::wstring payload, 
					 const std::wstring extra);

private:
	std::vector<std::shared_ptr<Payload>> m_payloads;
};
