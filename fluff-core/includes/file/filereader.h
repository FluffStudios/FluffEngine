#pragma once

#include <common.h>

namespace fluff { namespace utilities
{
	
	class FileReader
	{
	public:
		static std::string ReadTextFile(const char *path);
	};

} }