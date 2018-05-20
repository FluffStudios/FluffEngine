#pragma once

#include <common.h>
#include <core/ecs/system.h>
#include <core/debug_event.h>

namespace luminos
{

	struct LUMINOS_API DebugSystem : public ecs::System<DebugSystem>, public ecs::Receiver<DebugSystem>
	{
	public:
		/*
			Creates the debug system
		*/
		DebugSystem();
		
		/*
			Configures debug system to respond to debug events

			Events - Event manager associated with debug system
		*/
		void Configure(ecs::EventManager & Events);

		/*
			Ticks through the debug messages

			Entities - Entity manager associated with debug system
			Events - Event manager associated with debug system
			TimeDelta - Time since last update
		*/
		void Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double TimeDelta) override;

		/*
			Response to the Debug Event

			Msg - Debug Message
		*/
		void Receive(const DebugEvent & Msg);
	};

}