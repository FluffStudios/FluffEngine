#pragma once

#include "common.h"

namespace luminos { namespace utilities
{

	class FileWriter
	{
	public:
		LUMINOS_API static void WriteToFile(std::string FileName, std::string Contents);
	};
	
} }