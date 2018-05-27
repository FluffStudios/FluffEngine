#pragma once

#include <string>
#include "common.h"

namespace luminos { namespace utilities
{

	struct LUMINOS_API SerializationString
	{
		std::string Name;
		std::string Value;

		size_t Size() const;
		void Serialize(unsigned char *Dest, size_t &Pointer) const;
		static void Deserialize(unsigned char *Src, SerializationString &Dest, size_t &Pointer);
		void Free();
	};
	
} }