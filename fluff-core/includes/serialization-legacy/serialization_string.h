#pragma once

#include <string>
#include <common.h>

namespace fluff { namespace utilities
{

	struct SerializationString
	{
		std::string Name;
		std::string Value;

		size_t FLUFF_API Size() const;
		void FLUFF_API Serialize(unsigned char *Dest, size_t &Pointer) const;
		static void Deserialize(unsigned char *Src, SerializationString &Dest, size_t &Pointer);
		void FLUFF_API Free();
	};
	
} }