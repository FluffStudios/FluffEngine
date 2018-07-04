#pragma once

#include <common.h>
#include <core/debug_event.h>
#include <core/ecs/ecs_manager.h>

namespace fluff
{
	class DebugMessage
	{
	public:
		/*
			Creates and emits a debug event on creation

			Severity - Significance of error
			Message - Error Message
		*/
		FLUFF_API DebugMessage(std::shared_ptr<ECSManager> & Manager, SEVERITY Severity, const char * Message)
		{
			Manager->GetEventManager()->EmitEvent<DebugEvent>(Severity, Message);
		}
	};
}