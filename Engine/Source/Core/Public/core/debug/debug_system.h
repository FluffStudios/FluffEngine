#pragma once

#include <common.h>
#include <core/ecs/system.h>
#include <core/debug/debug_event.h>

namespace fluff { namespace debug 
{

	struct FLUFF_API DebugSystem : public ecs::System<DebugSystem>, public ecs::Receiver<DebugSystem>
	{
	public:
		/*
			Creates the debug system
		*/
		FLUFF_API DebugSystem();
		
		/*
			Configures debug system to respond to debug events

			Events - Event manager associated with debug system
		*/
		void FLUFF_API Configure(ecs::EventManager & Events);

		/*
			Ticks through the debug messages

			Entities - Entity manager associated with debug system
			Events - Event manager associated with debug system
			TimeDelta - Time since last update
		*/
		void FLUFF_API Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double TimeDelta) override;

		/*
			Response to the Debug Event

			Msg - Debug Message
		*/
		void FLUFF_API Receive(const DebugEvent & Msg);
	};

}	}