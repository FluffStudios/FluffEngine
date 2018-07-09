#pragma once

#include <common.h>
#include <core/ecs/event.h>
#include <core/debug/debug_error_type.h>
#include <core/debug/debug_severity.h>

namespace fluff { namespace debug 
{
	struct FLUFF_API DebugEvent : public ecs::Event<DebugEvent>
	{
		/*
			Creates a new Debug Event

			Severity - Level of significance of event
			Message - Associated error message
		*/
		DebugEvent(DebugErrorType ErrorType, DebugSeverity Severity, size_t LineNumber, std::string FileName, std::string Description) 
			: ErrorType(ErrorType), Severity(Severity), LineNumber(LineNumber), FileName(FileName), Description(Description) { };

		/*
			Deconstructor
		*/
		~DebugEvent() { }

		DebugSeverity Severity;
		DebugErrorType ErrorType;
		size_t LineNumber;
		std::string FileName;
		std::string Description;
	};
}	}