#pragma once

#include "common.h"

namespace luminos { namespace utilities
{
	
	struct LUMINOS_API SerializationMeshAnim
	{
		bool Init = false;
		std::string Name;
		size_t KeyCount;
		unsigned int* KeyIndex;
		long double* Time;

		void Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* Src, SerializationMeshAnim& Dest, size_t& Pointer);
		size_t Size() const;
		void Free();
	};

} }