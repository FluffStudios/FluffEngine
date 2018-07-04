#pragma once

#include <common.h>

namespace fluff { namespace utilities
{

	class FileWriter
	{
	public:
		static void WriteToFile(std::string FileName, std::string Contents);
	};
	
} }