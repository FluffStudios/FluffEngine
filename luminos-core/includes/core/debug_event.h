#pragma once

#include <common.h>
#include <core/ecs/event.h>

namespace luminos
{
	enum LUMINOS_API SEVERITY
	{
		MESSAGE,
		WARNING,
		FATAL
	};

	struct LUMINOS_API DebugEvent : public ecs::Event<DebugEvent>
	{
		/*
			Creates a new Debug Event

			Severity - Level of significance of event
			Message - Associated error message
		*/
		DebugEvent(SEVERITY Severity, const char * Message) : Severity(Severity), Message(Message) { };

		/*
			Deconstructor
		*/
		~DebugEvent() { }

		SEVERITY Severity;
		const char * Message;
	};
}