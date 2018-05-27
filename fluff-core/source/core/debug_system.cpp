#include <core/debug_system.h>
#include <iostream>

namespace luminos
{
	DebugSystem::DebugSystem()
	{

	}

	void DebugSystem::Configure(ecs::EventManager & Events)
	{
		Events.SubscribeToEvent<DebugEvent>(*this);
	}

	void DebugSystem::Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double Delta)
	{

	}

	void DebugSystem::Receive(const DebugEvent & Msg)
	{
		std::string Message;
		switch (Msg.Severity)
		{
		case FATAL:
		{
			Message.append("Fatal Error: ");
			break;
		}
		case WARNING:
		{
			Message.append("Warning: ");
			break;
		}
		case MESSAGE:
		{
			Message.append("Debug Message: ");
			break;
		}
		}

		Message.append(Msg.Message);
		std::cout << Message.c_str() << std::endl;
	}
}