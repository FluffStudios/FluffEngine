#pragma once

#include <common.h>
#include <string>
#include <vector>

#include "serialization_meshanim.h"
#include "serialization_nodeanim.h"

namespace luminos { namespace utilities
{
	
	struct LUMINOS_API SerializationAnimation
	{
		long double Duration;
		long double TicksPerSecond;
		size_t NumChannels;
		std::vector<SerializationNodeAnim> Channels;
		size_t NumMeshChannels;
		std::vector<SerializationMeshAnim> MeshChannels;
		std::string Name;

		void Free();
		void Serialize(unsigned char* Dest, size_t& Pointer);
		static void Deserialize(unsigned char* Src, SerializationAnimation& Dest, size_t& Pointer);
		size_t Size();
	};

} }