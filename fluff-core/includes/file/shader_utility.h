#pragma once

#include <common.h>
#include <unordered_map>

namespace fluff { namespace utilities
{
	
	class ShaderUtility
	{
	public:
		static std::string LoadShader(std::string FileName, std::unordered_map<std::string, unsigned int> Uniforms = std::unordered_map<std::string, unsigned int>());
		static bool SaveShader(std::string FileName, std::string Data);
	};

} }
