#pragma once

#include <common.h>

namespace fluff { namespace utilities
{
	
	class FLUFF_API FileReader
	{
	public:
		static std::string ReadTextFile(const char *path);
	};

} }