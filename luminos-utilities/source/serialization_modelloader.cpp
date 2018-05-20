#include <serialization_modelloader.h>

#include <serialization_vertex.h>

namespace luminos { namespace utilities
{

	SerializationModel* ModelLoader::Convert(std::string filename)
	{
		SerializationModel* model = new SerializationModel;
		const aiScene* scene = aiImportFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate);
#if defined(DEBUG) || defined(_DEBUG)
		if (!scene) __debugbreak();
#endif
		model->Name = filename;
		model->MeshCount = scene->mNumMeshes;
		if (model->MeshCount)
			model->Meshes = LoadMesh(scene);

		model->AnimCount = scene->mNumAnimations;
		if (model->AnimCount)
			model->Anims = LoadAnimation(scene);
		return model;
	}

	SerializationModel* ModelLoader::Load(std::string name, Database& db)
	{
		SerializationModel* model = new SerializationModel;

		return model;
	}

	std::vector<SerializationMesh> ModelLoader::LoadMesh(const aiScene* scene)
	{
		std::vector<SerializationMesh> meshes;
		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* m = scene->mMeshes[i];
			SerializationMesh mesh = SerializationMesh::CreateMesh(m->mName.C_Str());
			mesh.HasVertices = m->HasPositions();
			mesh.HasUVs = m->HasTextureCoords(0);
			mesh.HasNormals = m->HasNormals();
			mesh.HasTangents = m->HasTangentsAndBitangents();
			mesh.HasDiffuse = m->HasVertexColors(0);
			mesh.HasSpecular = m->HasVertexColors(1);
			mesh.HasIndices = m->HasFaces();
			mesh.VertexCount = m->mNumVertices;
			mesh.IndexCount = (m->mNumFaces) * 3;

			float *vertices = new float[mesh.VertexCount * 3];
			float *uvs = new float[mesh.VertexCount * 3];
			float *normals = new float[mesh.VertexCount * 3];
			float *tangents = new float[mesh.VertexCount * 3];
			float *bitangents = new float[mesh.VertexCount * 3];
			float *diffuse = new float[mesh.VertexCount * 3];
			float *specular = new float[mesh.VertexCount * 3];
			mesh.Indices = new unsigned int[mesh.IndexCount];

			for (size_t j = 0; j < m->mNumVertices; j++)
			{
				if (mesh.HasVertices)
				{
					vertices[3 * j + 0] = m->mVertices[j].x;
					vertices[3 * j + 1] = m->mVertices[j].y;
					vertices[3 * j + 2] = m->mVertices[j].z;
				}

				if (mesh.HasUVs)
				{
					uvs[2 * j + 0] = m->mTextureCoords[0][j].x;
					uvs[2 * j + 1] = m->mTextureCoords[0][j].y;
					uvs[2 * j + 2] = m->mTextureCoords[0][j].z;
				}

				if (mesh.HasNormals)
				{
					normals[3 * j + 0] = m->mNormals[j].x;
					normals[3 * j + 1] = m->mNormals[j].y;
					normals[3 * j + 2] = m->mNormals[j].z;
				}

				if (mesh.HasTangents)
				{
					tangents[3 * j + 0] = m->mTangents[j].x;
					tangents[3 * j + 1] = m->mTangents[j].y;
					tangents[3 * j + 2] = m->mTangents[j].z;

					bitangents[3 * j + 0] = m->mBitangents[j].x;
					bitangents[3 * j + 1] = m->mBitangents[j].y;
					bitangents[3 * j + 2] = m->mBitangents[j].z;
				}

				if (mesh.HasDiffuse)
				{
					for (size_t k = 0; k < m->GetNumColorChannels(); k++)
					{
						diffuse[3 * j + k] = m->mColors[0][j][k];
					}
					if (m->GetNumColorChannels() == 3)
					{
						diffuse[3 * j + 3] = 1.0f;
					}
				}

				if (mesh.HasSpecular)
				{
					for (size_t k = 0; k < m->GetNumColorChannels(); k++)
					{
						specular[3 * j + k] = m->mColors[1][j][k];
					}
					if (m->GetNumColorChannels() == 3)
					{
						specular[3 * j + 3] = 1.0f;
					}
				}
			}
			
			mesh.VertexArray = new VertexSerial[mesh.VertexCount];
			for (size_t j = 0; j < mesh.VertexCount; j++)
			{
				VertexSerial* v = &(mesh.VertexArray[j]);
				if (mesh.HasVertices)
				{
					v->Position[0] = vertices[3 * j + 0];
					v->Position[1] = vertices[3 * j + 1];
					v->Position[2] = vertices[3 * j + 2];
				}

				if (mesh.HasUVs)
				{	
					if (m->GetNumUVChannels() == 3) 
					{
						v->Uv[0] = uvs[3 * j + 0];
						v->Uv[1] = uvs[3 * j + 1];
						v->Uv[2] = uvs[3 * j + 2];
					}			
					else
					{
						v->Uv[0] = uvs[2 * j + 0];
						v->Uv[1] = uvs[2 * j + 1];
					}
				}

				if (mesh.HasNormals)
				{
					v->Normal[0] = normals[3 * j + 0];
					v->Normal[1] = normals[3 * j + 1];
					v->Normal[2] = normals[3 * j + 2];
				}

				if (mesh.HasTangents)
				{
					v->Tangent[0] = tangents[3 * j + 0];
					v->Tangent[1] = tangents[3 * j + 1];
					v->Tangent[2] = tangents[3 * j + 2];

					v->Bitangent[0] = bitangents[3 * j + 0];
					v->Bitangent[1] = bitangents[3 * j + 1];
					v->Bitangent[2] = bitangents[3 * j + 2];
				}

				if (mesh.HasDiffuse)
				{
					v->Diffuse[0] = diffuse[3 * j + 0];
					v->Diffuse[1] = diffuse[3 * j + 1];
					v->Diffuse[2] = diffuse[3 * j + 2];
					v->Diffuse[3] = diffuse[3 * j + 3];
				}

				if (mesh.HasDiffuse)
				{
					v->Specular[0] = specular[3 * j + 0];
					v->Specular[1] = specular[3 * j + 1];
					v->Specular[2] = specular[3 * j + 2];
					v->Specular[3] = specular[3 * j + 3];
				}

			}					 
			for (size_t j = 0; j < m->mNumFaces; j++)
			{
				if (mesh.HasIndices)
				{
					mesh.Indices[3 * j + 0] = m->mFaces[j].mIndices[0];
					mesh.Indices[3 * j + 1] = m->mFaces[j].mIndices[1];
					mesh.Indices[3 * j + 2] = m->mFaces[j].mIndices[2];
				}
			}

			delete [] vertices;
			delete [] uvs;
			delete [] normals;
			delete [] tangents;
			delete [] bitangents;
			delete [] diffuse;
			delete [] specular;

			if (m->HasBones())
			{
				mesh.Bones = LoadBones(m);
			}

			meshes.push_back(mesh);
		}
		return meshes;
	}

	std::vector<SerializationBone> ModelLoader::LoadBones(aiMesh* mesh)
	{
		std::vector<SerializationBone> bones;

		for (size_t i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* b = mesh->mBones[i];
			SerializationBone bone = SerializationBone::CreateBone(b->mName.C_Str());

			bone.WeightCount = b->mNumWeights;
			bone.Offset = new float[16];
			bone.Offset[0] = b->mOffsetMatrix[0][0];
			bone.Offset[1] = b->mOffsetMatrix[0][1];
			bone.Offset[2] = b->mOffsetMatrix[0][2];
			bone.Offset[3] = b->mOffsetMatrix[0][3];
			bone.Offset[4] = b->mOffsetMatrix[1][0];
			bone.Offset[5] = b->mOffsetMatrix[1][1];
			bone.Offset[6] = b->mOffsetMatrix[1][2];
			bone.Offset[7] = b->mOffsetMatrix[1][3];
			bone.Offset[8] = b->mOffsetMatrix[2][0];
			bone.Offset[9] = b->mOffsetMatrix[2][1];
			bone.Offset[10] = b->mOffsetMatrix[2][2];
			bone.Offset[11] = b->mOffsetMatrix[2][3];
			bone.Offset[12] = b->mOffsetMatrix[3][0];
			bone.Offset[13] = b->mOffsetMatrix[3][1];
			bone.Offset[14] = b->mOffsetMatrix[3][2];
			bone.Offset[15] = b->mOffsetMatrix[3][3];
			bone.Id = new unsigned int[b->mNumWeights];
			bone.Weight = new float[b->mNumWeights];
			
			for (size_t j = 0; j < b->mNumWeights; j++)
			{
				bone.Id[j] = b->mWeights[j].mVertexId;
				bone.Weight[j] = b->mWeights[j].mWeight;
			}

			bones.push_back(bone);
		}

		return bones;
	}

	std::vector<SerializationAnimation> ModelLoader::LoadAnimation(const aiScene* scene)
	{
		std::vector<SerializationAnimation> animations;

		for (size_t i = 0; i < scene->mNumAnimations; i++)
		{
			SerializationAnimation animation;
			aiAnimation* anim = scene->mAnimations[i];
			
			animation.NumChannels = anim->mNumChannels;
			animation.NumMeshChannels = anim->mNumMeshChannels;
			animation.Name = anim->mName.C_Str();
			animation.Duration = anim->mDuration;
			animation.TicksPerSecond = anim->mTicksPerSecond;

			for (size_t j = 0; j < animation.NumChannels; j++)
			{
				animation.Channels.push_back(LoadNodeAnimation(anim, j));
			}

			for (size_t j = 0; j < animation.NumMeshChannels; j++)
			{
				animation.MeshChannels.push_back(LoadMeshAnimation(anim, j));
			}

			animations.push_back(animation);
		}

		return animations;
	}

	SerializationNodeAnim ModelLoader::LoadNodeAnimation(aiAnimation* anim, size_t i)
	{
		SerializationNodeAnim nodeAnim;

		aiNodeAnim* animation = anim->mChannels[i];
		nodeAnim.Name = animation->mNodeName.C_Str();
		nodeAnim.NumPositionKeys = animation->mNumPositionKeys;
		nodeAnim.NumRotationKeys = animation->mNumRotationKeys;
		nodeAnim.NumScalingKeys = animation->mNumScalingKeys;

		nodeAnim.PositionKeys = new float[animation->mNumPositionKeys * 3]; // vec3
		nodeAnim.RotationKeys = new float[animation->mNumRotationKeys * 4]; // quaternion
		nodeAnim.ScaleKeys    = new float[animation->mNumScalingKeys  * 3]; // vec3

		nodeAnim.PositionTimes = new long double[animation->mNumPositionKeys];
		nodeAnim.RotationTimes = new long double[animation->mNumRotationKeys];
		nodeAnim.ScaleTimes    = new long double[animation->mNumScalingKeys];

		for (auto j = 0; j < nodeAnim.NumPositionKeys; j++)
		{
			nodeAnim.PositionKeys[3 * j + 0] = animation->mPositionKeys[j].mValue.x;
			nodeAnim.PositionKeys[3 * j + 1] = animation->mPositionKeys[j].mValue.y;
			nodeAnim.PositionKeys[3 * j + 2] = animation->mPositionKeys[j].mValue.z;
			nodeAnim.PositionTimes[j] = animation->mPositionKeys[j].mTime;
		}

		for (auto j = 0; j < nodeAnim.NumRotationKeys; j++)
		{
			nodeAnim.RotationKeys[3 * j + 0] = animation->mRotationKeys[j].mValue.x;
			nodeAnim.RotationKeys[3 * j + 1] = animation->mRotationKeys[j].mValue.y;
			nodeAnim.RotationKeys[3 * j + 2] = animation->mRotationKeys[j].mValue.z;
			nodeAnim.RotationKeys[3 * j + 3] = animation->mRotationKeys[j].mValue.w;
			nodeAnim.RotationTimes[j] = animation->mRotationKeys[j].mTime;
		}

		for (int j = 0; j < nodeAnim.NumScalingKeys; j++)
		{
			nodeAnim.ScaleKeys[3 * j + 0] = animation->mScalingKeys[j].mValue.x;
			nodeAnim.ScaleKeys[3 * j + 1] = animation->mScalingKeys[j].mValue.y;
			nodeAnim.ScaleKeys[3 * j + 2] = animation->mScalingKeys[j].mValue.z;
			nodeAnim.ScaleTimes[j] = animation->mScalingKeys[j].mTime;
		}

		switch (animation->mPreState)
		{
			case (aiAnimBehaviour::aiAnimBehaviour_CONSTANT):
			{
				nodeAnim.Pre.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::CONSTANT;
				break;
			}
			case (aiAnimBehaviour::aiAnimBehaviour_DEFAULT):
			{
				nodeAnim.Pre.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::DEFAULT;
				break;
			}
			case (aiAnimBehaviour::aiAnimBehaviour_LINEAR):
			{
				nodeAnim.Pre.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::LINEAR;
				break;
			}
			case (aiAnimBehaviour::aiAnimBehaviour_REPEAT):
			{
				nodeAnim.Pre.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::REPEAT;
				break;
			}
		}

		switch (animation->mPostState)
		{
		case (aiAnimBehaviour::aiAnimBehaviour_CONSTANT):
		{
			nodeAnim.Post.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::CONSTANT;
			break;
		}
		case (aiAnimBehaviour::aiAnimBehaviour_DEFAULT):
		{
			nodeAnim.Post.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::DEFAULT;
			break;
		}
		case (aiAnimBehaviour::aiAnimBehaviour_LINEAR):
		{
			nodeAnim.Post.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::LINEAR;
			break;
		}
		case (aiAnimBehaviour::aiAnimBehaviour_REPEAT):
		{
			nodeAnim.Post.Behavior = SERIALIZATION_ANIMATION_BEHAVIOR::REPEAT;
			break;
		}
		}

		return nodeAnim;
	}

	SerializationMeshAnim ModelLoader::LoadMeshAnimation(aiAnimation* anim, size_t i)
	{
		SerializationMeshAnim meshAnim;
		aiMeshAnim* animation = anim->mMeshChannels[i];
		meshAnim.Name = animation->mName.C_Str();
		meshAnim.KeyCount = animation->mNumKeys;
		meshAnim.KeyIndex = new unsigned int[meshAnim.KeyCount];
		meshAnim.Time = new long double[meshAnim.KeyCount];

		for (size_t j = 0; j < meshAnim.KeyCount; j++)
		{
			meshAnim.KeyIndex[j] = animation->mKeys[j].mValue;
			meshAnim.Time[j]     = animation->mKeys[j].mTime;
		}

		return meshAnim;
	}

} }