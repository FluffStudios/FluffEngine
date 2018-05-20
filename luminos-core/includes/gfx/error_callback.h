#pragma once

#include <glew.h>
#include <common.h>
#include <sstream>

#define BREAK_ON_GL_ERROR

namespace luminos { namespace gfx
{
	
	/*
		Error checking utility for GLFW
		
		File - File error is in
		Line - Line error is on
	 */
	inline GLenum glCheckError_(const char *File, int Line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			default:;
			}
			std::stringstream stream;
			stream << error << " | " << File << " (" << Line << ")" << std::endl;
			LUMINOS_ERROR_FUNC(stream.str().c_str())
		}
		return errorCode;
	}

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
	inline void APIENTRY glDebugOutput(GLenum Source,
		GLenum Type,
		GLuint Id,
		GLenum Severity,
		GLsizei Length,
		const GLchar *Message,
		const void *UserParam)
	{
		if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204) return; // ignore these non-significant error codes

		std::stringstream stream;
		stream << "---------------" << std::endl;
		stream << "Debug message (" << Id << "): " << Message << std::endl;

		switch (Source)
		{
		case GL_DEBUG_SOURCE_API:             stream << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   stream << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: stream << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     stream << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     stream << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           stream << "Source: Other"; break;
		default:;
		} stream << std::endl;

		switch (Type)
		{
		case GL_DEBUG_TYPE_ERROR:               stream << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: stream << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  stream << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         stream << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         stream << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              stream << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          stream << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           stream << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               stream << "Type: Other"; break;
		default:;
		} stream << std::endl;

		switch (Severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         stream << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       stream << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          stream << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: stream << "Severity: notification"; break;
		default:;
		} stream << std::endl;
		stream << std::endl;

		LUMINOS_ERROR_FUNC(stream.str().c_str())
#ifdef BREAK_ON_GL_ERROR
	if (Severity == GL_DEBUG_SEVERITY_HIGH)
		LUMINOS_ASSERT()
#endif
	}

} }

// Checks for errors
#define glCheckError() glCheckError_(__FILE__, __LINE__)
