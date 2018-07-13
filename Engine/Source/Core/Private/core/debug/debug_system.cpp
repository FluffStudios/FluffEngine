#include <core/debug/debug_system.h>
#include <iostream>

namespace fluff { namespace debug
{
	DebugSystem::DebugSystem()
	{

	}

	void DebugSystem::Configure(std::shared_ptr<ecs::EventManager> & Events)
	{
		Events->SubscribeToEvent<DebugEvent>(*this);
	}

	void DebugSystem::Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double Delta)
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
		case WARN:
		{
			Message.append("Warning: ");
			break;
		}
		case ERROR:
		{
			Message.append("Error Message: ");
			break;
		}
		}

		Message.append(Msg.Description);
		std::cout << Message.c_str() << std::endl;
	}
}	}