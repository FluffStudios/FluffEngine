#pragma once

#include <core/debug/debug_error_type.h>
#include <core/debug/debug_severity.h>
#include <core/debug/debug_event.h>
#include <core/ecs/event.h>
#include <core/ecs/ecs_manager.h>
#include <string>

namespace fluff { namespace debug {

	struct DebugMessage
	{
		DebugMessage(std::shared_ptr<ECSManager> & Manager, DebugErrorType ErrorType, DebugSeverity Severity, size_t LineNumber, std::string FileName, std::string Description)
		{
			Manager->GetEventManager()->EmitEvent<DebugEvent>(ErrorType, Severity, LineNumber, FileName, Description);
		}
	};

} }