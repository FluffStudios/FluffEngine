#pragma once

#include <common.h>
#include <string>
#include <vector>

#include <serialization-legacy/serialization_meshanim.h>
#include <serialization-legacy/serialization_nodeanim.h>

namespace fluff { namespace utilities
{
	
	struct SerializationAnimation
	{
		long double Duration;
		long double TicksPerSecond;
		size_t NumChannels;
		std::vector<SerializationNodeAnim> Channels;
		size_t NumMeshChannels;
		std::vector<SerializationMeshAnim> MeshChannels;
		std::string Name;

		void FLUFF_API Free();
		void FLUFF_API Serialize(unsigned char* Dest, size_t& Pointer);
		static void Deserialize(unsigned char* Src, SerializationAnimation& Dest, size_t& Pointer);
		size_t FLUFF_API Size();
	};

} }