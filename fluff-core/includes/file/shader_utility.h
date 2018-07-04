#pragma once

#include <common.h>
#include <unordered_map>

namespace fluff { namespace utilities
{
	
	class ShaderUtility
	{
	public:
		FLUFF_API static std::string LoadShader(std::string FileName, std::unordered_map<std::string, unsigned int> Uniforms = std::unordered_map<std::string, unsigned int>());
		FLUFF_API static bool SaveShader(std::string FileName, std::string Data);
	};

} }
