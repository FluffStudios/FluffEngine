#pragma once

#include "common.h"

#include "serialization_mesh.h"
#include "serialization_animation.h"

namespace luminos { namespace utilities
{
	
	struct LUMINOS_API SerializationModel
	{
		std::string Name;

		size_t MeshCount;
		std::vector<SerializationMesh> Meshes;

		size_t AnimCount;
		std::vector<SerializationAnimation> Anims;

		void Free();
		void Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* Src, SerializationModel& Dest, size_t& Pointer);
		size_t Size() const;
	};

} }