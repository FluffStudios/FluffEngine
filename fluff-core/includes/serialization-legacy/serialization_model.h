#pragma once

#include <common.h>

#include <serialization-legacy/serialization_mesh.h>
#include <serialization-legacy/serialization_animation.h>

namespace fluff { namespace utilities
{
	
	struct SerializationModel
	{
		std::string Name;

		size_t MeshCount;
		std::vector<SerializationMesh> Meshes;

		size_t AnimCount;
		std::vector<SerializationAnimation> Anims;

		void FLUFF_API Free();
		void FLUFF_API Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void FLUFF_API Deserialize(unsigned char* Src, SerializationModel& Dest, size_t& Pointer);
		size_t FLUFF_API Size() const;
	};

} }