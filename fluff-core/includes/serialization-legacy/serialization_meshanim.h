#pragma once

#include <common.h>

namespace fluff { namespace utilities
{
	
	struct SerializationMeshAnim
	{
		bool Init = false;
		std::string Name;
		size_t KeyCount;
		unsigned int* KeyIndex;
		long double* Time;

		void FLUFF_API Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* Src, SerializationMeshAnim& Dest, size_t& Pointer);
		size_t FLUFF_API Size() const;
		void FLUFF_API Free();
	};

} }