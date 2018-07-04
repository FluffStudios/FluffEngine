#pragma once

#include <common.h>
#include <core/ecs/event.h>

namespace fluff
{
	enum SEVERITY
	{
		MESSAGE,
		WARNING,
		FATAL
	};

	struct DebugEvent : public ecs::Event<DebugEvent>
	{
		/*
			Creates a new Debug Event

			Severity - Level of significance of event
			Message - Associated error message
		*/
		FLUFF_API DebugEvent(SEVERITY Severity, const char * Message) : Severity(Severity), Message(Message) { };

		/*
			Deconstructor
		*/
		FLUFF_API ~DebugEvent() { }

		SEVERITY Severity;
		const char * Message;
	};
}