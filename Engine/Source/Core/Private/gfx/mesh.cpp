#include <gfx/mesh.h>

namespace fluff { namespace gfx {

	size_t Mesh::IDCounter_ = 0;

	Mesh::~Mesh()
	{
		if (Positions_)
		{
			Allocator::Free(Positions_);
		}
		if (TextureCoords_)
		{
			Allocator::Free(TextureCoords_);
		}
		if (Normals_)
		{
			Allocator::Free(Normals_);
		}
		if (Tangents_)
		{
			Allocator::Free(Tangents_);
		}
		if (BiTangents_)
		{
			Allocator::Free(BiTangents_);
		}
		if (Indices_)
		{
			Allocator::Free(Indices_);
		}
		ID_ = 0;
	}

	Mesh::Mesh(std::vector<Vertex> & Vertices, std::vector<unsigned int> & Indices)
	{
		VertexCount = 0;
		size_t i = 0;
		
		Positions_ = static_cast<float*>(Allocator::Alloc(Vertices.size() * 3 * sizeof(float)));
		TextureCoords_ = static_cast<float*>(Allocator::Alloc(Vertices.size() * 3 * sizeof(float)));
		Normals_ = static_cast<float*>(Allocator::Alloc(Vertices.size() * 3 * sizeof(float)));
		Tangents_ = static_cast<float*>(Allocator::Alloc(Vertices.size() * 3* sizeof(float)));
		BiTangents_ = static_cast<float*>(Allocator::Alloc(Vertices.size() * 3 * sizeof(float)));
		Indices_ = static_cast<unsigned int*>(Allocator::Alloc(Indices.size() * sizeof(unsigned int)));

		for (const auto vertex : Vertices) AddVertex(vertex, i++);
		this->Indices_ = new unsigned int[Indices.size()];
		memcpy(Indices_, Indices.data(), sizeof(unsigned int) * Indices.size());
		this->IndexCount = Indices.size();
		ID_ = IDCounter_++;
	}

	Mesh::Mesh(utilities::SerializationMesh & Serial)
	{

		Positions_ = new float[Serial.VertexCount * 3];
		TextureCoords_ = new float[Serial.VertexCount * 3];
		Normals_ = new float[Serial.VertexCount * 3];
		Tangents_ = new float[Serial.VertexCount * 3];
		BiTangents_ = new float[Serial.VertexCount * 3];
		Indices_ = new unsigned int[Serial.IndexCount];

		for (auto i = 0; i < Serial.VertexCount; i++)
		{
			const auto v = Vertex(Serial.VertexArray[i]);
			AddVertex(v, i);
		}

		memcpy(Indices_, Serial.Indices, sizeof(unsigned int) * Serial.IndexCount);

		IndexCount = Serial.IndexCount;
		VertexCount = Serial.VertexCount;

		this->HasPositions = Serial.HasVertices;
		this->HasUVs = Serial.HasUVs;
		this->HasNormals = Serial.HasNormals;
		this->HasTangents = Serial.HasTangents;
		this->HasIndices = Serial.HasIndices;
		ID_ = IDCounter_++;
	}

	Mesh::Mesh(utilities::SerializationMesh * SerialMesh, size_t Count)
	{
		auto VertexCount = 0L;
		IndexCount = 0L;

		for (auto i = 0; i < Count; i++)
		{
			VertexCount += SerialMesh[i].VertexCount;
			IndexCount += SerialMesh[i].IndexCount;
		}

		Positions_ = new float[VertexCount * 3];
		TextureCoords_ = new float[VertexCount * 3];
		Normals_ = new float[VertexCount * 3];
		Tangents_ = new float[VertexCount * 3];
		BiTangents_ = new float[VertexCount * 3];
		Indices_ = new unsigned int[IndexCount];

		auto c = 0L;
		for (auto i = 0; i < Count; i++)
		{
			auto Serial = SerialMesh[i];
			for (auto j = 0; j < Serial.VertexCount; j++)
			{
				const auto v = Vertex(Serial.VertexArray[j]);
				AddVertex(v, c++);
			}
		}

		auto offset = 0L;
		auto index_offset = 0L;
		for (auto i = 0; i < Count; i++)
		{
			for (auto j = 0; j < SerialMesh[i].IndexCount; j++)
			{
				Indices_[offset++] = SerialMesh[i].Indices[j] + index_offset;
			}
			index_offset += SerialMesh[i].VertexCount;
		}

		for (auto i = 0; i < Count; i++)
		{
			auto Serial = SerialMesh[i];
			if (!this->HasPositions) this->HasPositions = Serial.HasVertices;
			if (!this->HasUVs) this->HasUVs = Serial.HasUVs;
			if (!this->HasNormals) this->HasNormals = Serial.HasNormals;
			if (!this->HasTangents) this->HasTangents = Serial.HasTangents;
			if (!this->HasIndices) this->HasIndices = Serial.HasIndices;
		}
		ID_ = IDCounter_++;
	}

	void Mesh::AddVertex(const Vertex& Vert, const size_t Offset)
	{
		Positions_[Offset * 3 + 0] = (Vert.Position[0]);
		Positions_[Offset * 3 + 1] = (Vert.Position[1]);
		Positions_[Offset * 3 + 2] = (Vert.Position[2]);

		TextureCoords_[Offset * 3 + 0] = (Vert.TextureCoordinates[0]);
		TextureCoords_[Offset * 3 + 1] = (Vert.TextureCoordinates[1]);
		TextureCoords_[Offset * 3 + 2] = (Vert.TextureCoordinates[2]);

		Normals_[Offset * 3 + 0] = (Vert.Normal[0]);
		Normals_[Offset * 3 + 1] = (Vert.Normal[1]);
		Normals_[Offset * 3 + 2] = (Vert.Normal[2]);

		Tangents_[Offset * 3 + 0] = (Vert.Tangent[0]);
		Tangents_[Offset * 3 + 1] = (Vert.Tangent[1]);
		Tangents_[Offset * 3 + 2] = (Vert.Tangent[2]);

		BiTangents_[Offset * 3 + 0] = (Vert.BiTangent[0]);
		BiTangents_[Offset * 3 + 1] = (Vert.BiTangent[1]);
		BiTangents_[Offset * 3 + 2] = (Vert.BiTangent[2]);

		VertexCount++;
	}

} }