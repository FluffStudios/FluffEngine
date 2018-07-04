#pragma once

#include <common.h>

#include <serialization-legacy/serialization_mesh.h>
#include <vector>
#include <common.h>
#include <core/allocators/allocator.h>

#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <common.h>

namespace fluff { namespace gfx {

	struct FLUFF_API Vertex
	{
		float Position[3];
		float TextureCoordinates[3];
		float Normal[3];
		float Tangent[3];
		float BiTangent[3];
		float Diffuse[4];
		float Specular[4];

		/*
			Creates default vertex object
		 */
		Vertex() { };

		/*
			Creates a vertex from a serialized vertex
			
			Serial - Serialized vertex
		 */
		explicit Vertex(utilities::VertexSerial Serial)
		{
			memcpy(Position, Serial.Position, 3 * sizeof(float));
			memcpy(TextureCoordinates, Serial.Uv, 3 * sizeof(float));
			memcpy(Normal, Serial.Normal, 3 * sizeof(float));
			memcpy(Tangent, Serial.Tangent, 3 * sizeof(float));
			memcpy(BiTangent, Serial.Bitangent, 3 * sizeof(float));
			memcpy(Diffuse, Serial.Diffuse, 4 * sizeof(float));
			memcpy(Specular, Serial.Specular, 4 * sizeof(float));
		}
	};

	class FLUFF_API Mesh
	{
		static size_t IDCounter_;
		size_t ID_;

		float * Positions_ = nullptr;
		float * TextureCoords_ = nullptr;
		float * Normals_ = nullptr;
		float * Tangents_ = nullptr;
		float * BiTangents_ = nullptr;
		unsigned int * Indices_ = nullptr;

		friend class cereal::access;

		template <class Archive>
		void save(Archive & ar) const
		{
			std::vector<float> Positions;
			std::vector<float> Textures;
			std::vector<float> Normals;
			std::vector<float> Tangents;
			std::vector<float> BiTangents;
			std::vector<unsigned int> Indices;

			if (HasPositions) Positions.resize(VertexCount * 3);
			if (HasUVs) Textures.resize(VertexCount * 3);
			if (HasNormals) Normals.resize(VertexCount * 3);
			if (HasTangents) Tangents.resize(VertexCount * 3);
			if (HasTangents) BiTangents.resize(VertexCount * 3);
			if (HasIndices) Indices.resize(IndexCount);

			if (HasPositions) memcpy(Positions.data(), Positions_, Positions.size() * sizeof(float));
			if (HasUVs) memcpy(Textures.data(), TextureCoords_, Textures.size() * sizeof(float));
			if (HasNormals) memcpy(Normals.data(), Normals_, Normals.size() * sizeof(float));
			if (HasTangents) memcpy(Tangents.data(), Tangents_, Tangents.size() * sizeof(float));
			if (HasTangents) memcpy(BiTangents.data(), BiTangents_, BiTangents.size() * sizeof(float));
			if (HasIndices) memcpy(Indices.data(), Indices_, Indices.size() * sizeof(unsigned int));

			ar(Positions, Textures, Normals, Tangents, BiTangents, Indices, HasPositions, HasUVs, HasNormals, HasTangents, HasIndices, VertexCount, IndexCount);
		}

		template <class Archive>
		void load(Archive & ar)
		{
			std::vector<float> Positions;
			std::vector<float> Textures;
			std::vector<float> Normals;
			std::vector<float> Tangents;
			std::vector<float> BiTangents;
			std::vector<unsigned int> Indices;

			ar(Positions, Textures, Normals, Tangents, BiTangents, Indices, HasPositions, HasUVs, HasNormals, HasTangents, HasIndices, VertexCount, IndexCount);

			if (HasPositions)
			{
				Positions_ = new float[VertexCount * 3];
				memcpy(Positions_, Positions.data(), Positions.size() * sizeof(float));
			}
			if (HasUVs)
			{
				TextureCoords_ = new float[VertexCount * 3];
				memcpy(TextureCoords_, Textures.data(), Textures.size() * sizeof(float));
			}
			if (HasNormals)
			{
				Normals_ = new float[VertexCount * 3];
				memcpy(Normals_, Normals.data(), Normals.size() * sizeof(float));
			}
			if (HasTangents)
			{
				Tangents_ = new float[VertexCount * 3];
				BiTangents_ = new float[VertexCount * 3];

				memcpy(Tangents_, Tangents.data(), Tangents.size() * sizeof(float));
				memcpy(BiTangents_, BiTangents.data(), BiTangents.size() * sizeof(float));
			}
			if (HasIndices)
			{
				Indices_ = new unsigned int[IndexCount];
				memcpy(Indices_, Indices.data(), Indices.size() * sizeof(unsigned int));
			}
		}
	public:
		bool HasPositions = true;
		bool HasUVs = true;
		bool HasNormals = true;
		bool HasTangents = true;
		bool HasIndices = true;
		std::string Name;

		size_t VertexCount;
		size_t IndexCount;

		Mesh() { }
		~Mesh();

		/*
			Checks if two meshes are equivalent by ID

			Other - other mesh
			Returns if equivalent
		*/
		inline bool operator == (const Mesh & Other) const { return ID_ == Other.ID_; }

		/*
			Checks if two meshes are not equivalent by ID

			Other - other mesh
			Returns if not equivalent
		*/
		inline bool operator != (const Mesh & Other) const { return ID_ != Other.ID_; }

		/*
			Creates a new mesh
			
			Positions - Position array
			UVs - UV coordinate array
			Normals - Normal vector array
			Indices - Index buffer
		 */
		explicit Mesh(std::vector<float> & Positions, std::vector<float> & UVs, std::vector<float> & Normals, std::vector<uint32_t> & Indices)
		{
			ID_ = IDCounter_++;
			Positions_ = static_cast<float*>(Allocator::Alloc(Positions.size() * sizeof(float)));
			TextureCoords_ = static_cast<float*>(Allocator::Alloc(UVs.size() * sizeof(float)));
			Normals_ = static_cast<float*>(Allocator::Alloc(Normals.size() * sizeof(float)));
			Indices_ = static_cast<unsigned int*>(Allocator::Alloc(Indices.size() * sizeof(unsigned int)));

			memcpy(Positions_, Positions.data(), sizeof(float) * Positions.size());
			memcpy(TextureCoords_, UVs.data(), sizeof(float) * UVs.size());
			memcpy(Normals_, Normals.data(), sizeof(float) * Normals.size());
			memcpy(Indices_, Indices.data(), sizeof(unsigned int) * Indices.size());

			VertexCount = Positions.size() / 3;
			IndexCount = Indices.size();

			HasTangents = false;
		}

		/*
			Creates a new mesh
			
			Positions - Position array
			UVs - UV coordinate array
			Normals - Normal vector array
			Indices - Index buffer
			VertexCount - Number of vertices
			IndexCount - Number of indices
		 */
		Mesh(float * Positions, float * UVs, float * Normals, unsigned int * Indices, const size_t VertexCount, const size_t IndexCount)
		{
			ID_ = IDCounter_++;
			Positions_ = static_cast<float*>(Allocator::Alloc(VertexCount * 3 * sizeof(float)));
			TextureCoords_ = static_cast<float*>(Allocator::Alloc(VertexCount * 3 * sizeof(float)));
			Normals_ = static_cast<float*>(Allocator::Alloc(VertexCount * 3 * sizeof(float)));
			Indices_ = static_cast<unsigned int*>(Allocator::Alloc(IndexCount * sizeof(unsigned int)));

			memcpy(Positions_, Positions, sizeof(float) * 3 * VertexCount);
			memcpy(TextureCoords_, UVs, sizeof(float) * 3 * VertexCount);
			memcpy(Normals_, Normals, sizeof(float) * 3 * VertexCount);
			memcpy(Indices_, Indices, sizeof(unsigned int) * IndexCount);

			this->VertexCount = VertexCount;
			this->IndexCount = IndexCount;
		}

		/*
			Creates a new mesh
			
			Positions - Position array
			UVs	- UV coordinate array
			Normals - Normal vector array
			Tangents - Tangent vector array
			BiTangents - Bitangent vector array
			Indices - Index buffer
		 */
		explicit Mesh(std::vector<float> & Positions, std::vector<float> & UVs, std::vector<float> & Normals, std::vector<float> & Tangents, std::vector<float> & BiTangents, std::vector<uint32_t> & Indices)
		{
			ID_ = IDCounter_++;
			Positions_ = new float[Positions.size()];
			TextureCoords_ = new float[UVs.size()];
			Normals_ = new float[Normals.size()];
			Tangents_ = new float[Tangents.size()];
			BiTangents_ = new float[BiTangents.size()];
			Indices_ = new unsigned int[Indices.size()];

			memcpy(Positions_, Positions.data(), sizeof(float) * Positions.size());
			memcpy(TextureCoords_, UVs.data(), sizeof(float) * UVs.size());
			memcpy(Normals_, Normals.data(), sizeof(float) * Normals.size());
			memcpy(Tangents_, Tangents.data(), sizeof(float) * Tangents.size());
			memcpy(BiTangents_, BiTangents.data(), sizeof(float) * BiTangents.size());
			memcpy(Indices_, Indices.data(), sizeof(unsigned int) * Indices.size());

			VertexCount = Positions.size() / 3;
			IndexCount = Indices.size();
		}

		/*
			Creates a mesh from an array of vertices and indices
			
			Vertices - Vertex array
			Indices - Index array
		 */
		explicit Mesh(std::vector<Vertex> & Vertices, std::vector<unsigned int> & Indices);
		
		/*
			Creates a mesh from a serialized mesh
			
			Serial - Serialized mesh
		 */
		explicit Mesh(utilities::SerializationMesh& Serial);

		/*
			Creates a mesh from a serialized mesh
			
			Serial - Serialized mesh
		 */
		explicit Mesh(utilities::SerializationMesh* Serial, size_t Count);

		/*
			Adds vertex to mesh
			
			Vert - Vertex to add
			Offset - Where to add vertex at
		 */
		void AddVertex(const Vertex& Vert, size_t Offset);

		/*
			Gets the position vector array
		  
			Returns positions
		 */
		inline float* GetPositionVector() const { return Positions_; }

		/*
			Gets the texture coordinate array
			
			Returns texture coordinates
		 */
		inline float* GetUVCoordinates() const { return TextureCoords_; }

		/*
			Gets the normals vector array
			
			Returns normals
		 */
		inline float* GetNormalVectors() const { return Normals_; }

		/*
			Gets the tangent vector array
			
			Returns tangents
		 */
		inline float* GetTangentVectors() const { return Tangents_; }

		/*
			Gets the bitanget vector array
			
			Returns bitangents
		 */
		inline float* GetBitangentVectors() const { return BiTangents_; }

		/*
			Gets the index buffer
			
			Returns index buffer
		 */
		inline unsigned int * GetIndexBuffer() const { return Indices_; }

		/*
			Set the position vector
			
			Positions - New positions
		 */
		void SetPositions(std::vector<float> & Positions)
		{
			if (Positions_) delete [] Positions_;
			Positions_ = new float[Positions.size()];
			memcpy(Positions_, Positions.data(), sizeof(float) * Positions.size());
		}

		/*
			Set the texture coordinates
			
			UVs - New texture coordinates
		 */
		void SetUVs(std::vector<float> & UVs)
		{
			if (TextureCoords_) delete[] TextureCoords_;
			TextureCoords_ = new float[UVs.size()];
			memcpy(TextureCoords_, UVs.data(), sizeof(float) * UVs.size());
		}

		/*
			Set the normal vector
			
			Normals - New normals
		 */
		void SetNormals(std::vector<float> & Normals)
		{
			if (Normals_) delete[] Normals_;
			Normals_ = new float[Normals.size()];
			memcpy(Normals_, Normals.data(), sizeof(float) * Normals.size());
		}

		/*
			Set the index buffer
		 
			Indices	- New indices
		 */
		void SetIndices(std::vector<unsigned int> & Indices)
		{
			if (Indices_) delete[] Indices_;
			Indices_ = new unsigned int[Indices.size()];
			memcpy(Indices_, Indices.data(), sizeof(unsigned int) * Indices.size());
		}

		/*
			Gets the index count

			Returns index count
		*/
		uint32_t GetIndexCount() const
		{
			return IndexCount;
		}

		/*
			Gets the number of vertices in the mesh

			Return vertex count
		*/
		uint32_t GetVertexCount() const
		{
			return VertexCount;
		}

		/*
			Gets the ID of the mesh

			Return ID
		*/
		size_t GetID() const
		{
			return ID_;
		}
	};	

} }
