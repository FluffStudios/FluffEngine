#pragma once

#include <common.h>
#include <unordered_map>

namespace luminos { namespace utilities
{
	
	class ShaderUtility
	{
	public:
		LUMINOS_API static std::string LoadShader(std::string FileName, std::unordered_map<std::string, unsigned int> Uniforms = std::unordered_map<std::string, unsigned int>());
		LUMINOS_API static bool SaveShader(std::string FileName, std::string Data);
	};

} }
