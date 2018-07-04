#pragma once

#include <assimp/mesh.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <serialization-legacy/serialization_model.h>
#include <serialization-legacy/serialization_database.h>

#include <common.h>

namespace fluff { namespace utilities
	{
		class ModelLoader
		{
		public:
			SerializationModel* Convert(std::string filename);
			SerializationModel* Load(std::string name, Database& db);
		private:
			std::vector<SerializationMesh> LoadMesh(const aiScene* scene);
			std::vector<SerializationBone> LoadBones(aiMesh* mesh);
			std::vector<SerializationAnimation> LoadAnimation(const aiScene* scene);
			SerializationNodeAnim LoadNodeAnimation(aiAnimation* animation, size_t i);
			SerializationMeshAnim LoadMeshAnimation(aiAnimation* animation, size_t i);
		};
	}
}
