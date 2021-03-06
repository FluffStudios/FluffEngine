#pragma once

#include <common.h>
#include <serialization-legacy/serialization_bone.h>
#include <serialization-legacy/serialization_vertex.h>
#include <vector>

namespace fluff { namespace utilities
{
	
	struct SerializationMesh
	{
		std::string Name;
		bool Init = false;
		VertexSerial* VertexArray;
		unsigned int* Indices;

		bool HasVertices = false;
		bool HasUVs = false;
		bool HasNormals = false;
		bool HasTangents = false;
		bool HasIndices = false;
		bool HasDiffuse = false;
		bool HasSpecular = false;

		size_t VertexCount = 0;
		size_t IndexCount = 0;

		std::vector<SerializationBone> Bones;

		size_t FLUFF_API Size() const;
		static SerializationMesh CreateMesh(std::string Name);
		void FLUFF_API Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* src, SerializationMesh& dest, size_t& pointer);
		void FLUFF_API Free();
	};

} }
