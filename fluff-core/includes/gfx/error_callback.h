#pragma once

#include <common.h>
#include <sstream>

#define BREAK_ON_GL_ERROR

namespace fluff { namespace gfx
{
	
	/*
		Error checking utility for GLFW
		
		File - File error is in
		Line - Line error is on
	 */
	inline uint32_t glCheckError_(const char *File, int Line);

	/*
		Error checking interface for luminos engine
		
		Source - Source of error
		Type - Type of error
		Id - ID of error
		Severity - Severity of error
		Length - Length of error
		Message - Error message
		UserParam - User parameters of error message
	 */
	void FLUFF_API glDebugOutput(uint32_t Source,
		uint32_t Type,
		uint32_t Id,
		uint32_t Severity,
		int32_t Length,
		const char *Message,
		const void *UserParam);

} }

// Checks for errors
#define glCheckError() glCheckError_(__FILE__, __LINE__)
