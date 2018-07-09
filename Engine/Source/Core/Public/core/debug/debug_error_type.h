#pragma once

#include <common.h>

namespace fluff { namespace debug {

	enum DebugErrorType : uint8_t
	{
		PERFORMANCE       = 0x0,
		ILLEGAL_DATA      = 0x1,
		ILLEGAL_STATE     = 0x2,
		INVALID_PARAMETER = 0x3
	};

} }