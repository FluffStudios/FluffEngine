#pragma once

#include <common.h>

namespace luminos { namespace utilities
{
	
	class LUMINOS_API FileReader
	{
	public:
		static std::string ReadTextFile(const char *path);
	};

} }