#include "payloadManager.h"

bool PayloadManager::RunPayload(const PayloadCommon::PayloadMode mode,
	const std::wstring payload,
	const std::wstring extra)
{
	bool ret = false;

	for (std::vector<std::shared_ptr<Payload>>::const_iterator payloadIt = m_payloads.begin();
		payloadIt != m_payloads.end();
		++payloadIt)
	{
		if (*payloadIt != nullptr)
		{
			std::shared_ptr<Payload> injector = *payloadIt;
			if (injector->GetMode() == mode)
			{
				ret = injector->Execute(payload, extra);
				break;
			}
		}
	}

	return ret;
}