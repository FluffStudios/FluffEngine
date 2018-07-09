#pragma once

#include <common.h>

namespace fluff { namespace debug {

	enum DebugSeverity : uint8_t
	{
		 WARN = 0x0,
		ERROR = 0x1,
		FATAL = 0x2
	};

} }