#pragma once

#include <common.h>
#include <core/debug_event.h>
#include <core/ecs/ecs_manager.h>

namespace fluff
{
	class FLUFF_API DebugMessage
	{
	public:
		/*
			Creates and emits a debug event on creation

			Severity - Significance of error
			Message - Error Message
		*/
		DebugMessage(std::shared_ptr<ECSManager> & Manager, SEVERITY Severity, const char * Message)
		{
			Manager->GetEventManager()->EmitEvent<DebugEvent>(Severity, Message);
		}

		static void SendMsg(std::shared_ptr<ECSManager> & Manager, SEVERITY Severity, const char * Message)
		{
			Manager->GetEventManager()->EmitEvent<DebugEvent>(Severity, Message);
		}
	};
}