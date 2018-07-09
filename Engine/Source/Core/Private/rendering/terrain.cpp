#include <rendering/terrain.h>
#include <gfx/context.h>
#include <util/maths.h>
#include <core/allocators/allocator.h>

namespace fluff { namespace render
{
	
	Terrain::Terrain(const unsigned int VertexCountPerSide) 
		: Terrain(VertexCountPerSide, nullptr, nullptr)
	{
	}

	Terrain::Terrain(const Terrain & Other) : Renderable(Other.Material_, Other.Model_)
	{
		HeightMap_ = new float[Other.VertexCountPerSide_ * Other.VertexCountPerSide_];
		this->VertexCountPerSide_ = Other.VertexCountPerSide_;

		memmove(HeightMap_, Other.HeightMap_, sizeof(int16_t) * VertexCountPerSide_ * VertexCountPerSide_);
	}

	Terrain::Terrain(const unsigned int VertexCountPerSide, Material* MaterialHandle)
		:Terrain(VertexCountPerSide, MaterialHandle, nullptr)
	{
	}

	Terrain::Terrain(const unsigned int VertexCountPerSide, Material* MaterialHandle, Model* ModelHandle)
		: Renderable(MaterialHandle, ModelHandle), VertexCountPerSide_(VertexCountPerSide)
	{
	}

	Terrain::Terrain(unsigned int VertexCountPerSide, Material * MaterialHandle, Transformation * TransformHandle, FastNoise & Noise, const float Amplitude, const unsigned int Size)
	{
		VertexCountPerSide += 1;
		float * positions = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		float * uvs = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		float * normals = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		unsigned int * indices = static_cast<unsigned int*>(Allocator::Alloc(sizeof(unsigned int) * 6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)));

		size_t vertex_pointer = 0;
		this->HeightMap_ = new float[VertexCountPerSide * VertexCountPerSide];

		for (int x = VertexCountPerSide - 1; x >= 0; x--)
		{
			for (size_t z = 0; z < VertexCountPerSide; z++)
			{
				auto height = Noise.GetValue(x + TransformHandle->GetPosition().x, z + TransformHandle->GetPosition().z) * Amplitude;
				HeightMap_[(VertexCountPerSide - x - 1) * (VertexCountPerSide) + z] = height;
				
				positions[3 * vertex_pointer + 0] = static_cast<float>(-((int32_t)VertexCountPerSide - x - 1));
				positions[3 * vertex_pointer + 1] = height;
				positions[3 * vertex_pointer + 2] = static_cast<float>(z);

				uvs[3 * vertex_pointer + 0] = static_cast<float>(0);
				uvs[3 * vertex_pointer + 1] = static_cast<float>((z / (VertexCountPerSide - 1)));
				uvs[3 * vertex_pointer + 2] = static_cast<float>((x / (VertexCountPerSide - 1)));
				vertex_pointer++;
			}
		}

		TransformHandle->SetPosition(TransformHandle->GetPosition() * static_cast<float>(Size));
		TransformHandle->SetScale(TransformHandle->GetScale() * glm::vec3(-1, 1, 1));

		size_t indexPointer = 0;
		for (size_t gz = 0; gz < VertexCountPerSide - 1; gz++) {
			for (size_t gx = 0; gx < VertexCountPerSide - 1; gx++) {
				auto top_left = (gz * VertexCountPerSide) + gx;
				auto top_right = top_left + 1;
				auto bottom_left = ((gz + 1) * VertexCountPerSide) + gx;
				auto bottom_right = bottom_left + 1;

				indices[indexPointer++] = static_cast<uint32_t>(bottom_right);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(top_right);
				indices[indexPointer++] = static_cast<uint32_t>(top_right);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(top_left);
			}
		}

		const auto face_norms = static_cast<glm::vec3*>(Allocator::Alloc(sizeof(glm::vec3) * VertexCountPerSide * VertexCountPerSide));
		const auto at_loc = static_cast<bool*>(Allocator::Alloc(sizeof(bool) * VertexCountPerSide * VertexCountPerSide));
		memset(at_loc, false, sizeof(bool) * VertexCountPerSide * VertexCountPerSide);

		for (uint32_t i = 0; i < (6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)); i += 3)
		{
			auto i1 = indices[i + 0];
			auto i2 = indices[i + 1];
			auto i3 = indices[i + 2];

			auto p1 = glm::vec3(positions[3 * i1], positions[3 * i1 + 1], positions[3 * i1 + 2]);
			auto p2 = glm::vec3(positions[3 * i2], positions[3 * i2 + 1], positions[3 * i2 + 2]);
			auto p3 = glm::vec3(positions[3 * i3], positions[3 * i3 + 1], positions[3 * i3 + 2]);

			glm::vec3 normal = glm::cross(p3 - p1, p3 - p2);
			normal *= glm::vec3{ -1, 1, -1 };
			auto tmp = normal;
			tmp.x = -normal.z;
			tmp.z = normal.x;
			normal = tmp;
			if (!at_loc[i1])
			{
				face_norms[i1] = normal;
				at_loc[i1] = true;
			}
			else
			{
				face_norms[i1] += normal;
			}

			if (!at_loc[i2])
			{
				face_norms[i2] = normal;
				at_loc[i2] = true;
			}
			else
			{
				face_norms[i2] += normal;
			}

			if (!at_loc[i3])
			{
				face_norms[i3] = normal;
				at_loc[i3] = true;
			}
			else
			{
				face_norms[i3] += normal;
			}
		}

		for (uint32_t i = 0; i < (VertexCountPerSide * VertexCountPerSide); i++)
		{
			face_norms[i] = glm::normalize(face_norms[i]);
			face_norms[i] *= -1;
			normals[3 * i] = face_norms[i].z;
			normals[3 * i + 1] = face_norms[i].y;
			normals[3 * i + 2] = face_norms[i].x;
		}

		Allocator::Free(face_norms);
		Allocator::Free(at_loc);

		this->VertexCountPerSide_ = VertexCountPerSide;

		this->Model_ = Context::LoadMesh(new Mesh(positions, uvs, normals, indices, VertexCountPerSide * VertexCountPerSide, 6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)));
		this->Material_ = MaterialHandle;

		Allocator::Free(positions);
		Allocator::Free(uvs);
		Allocator::Free(normals);
		Allocator::Free(indices);
	}

	Terrain* Terrain::GenerateFlatTerrain(unsigned int VertexCountPerSide, Material* MaterialHandle, Transformation* TransformHandle)
	{
		VertexCountPerSide += 1;
		float * positions = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		float * uvs = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		float * normals = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		unsigned int * indices = static_cast<unsigned int*>(Allocator::Alloc(sizeof(unsigned int) * 6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)));

		size_t vertex_pointer = 0;
		auto HeightMap = new float[VertexCountPerSide * VertexCountPerSide];

		for (int x = VertexCountPerSide - 1; x >= 0; x--)
		{
			for (uint32_t z = 0; z < VertexCountPerSide; z++)
			{
				positions[3 * vertex_pointer + 0] = static_cast<float>(-x);
				auto height = 0.0f;
				HeightMap[vertex_pointer] = height;

				positions[3 * vertex_pointer + 1] = height;
				positions[3 * vertex_pointer + 2] = static_cast<float>(z);

				uvs[3 * vertex_pointer + 0] = static_cast<float>(0);
				uvs[3 * vertex_pointer + 1] = static_cast<float>((z / (VertexCountPerSide - 1)));
				uvs[3 * vertex_pointer + 2] = static_cast<float>((x / (VertexCountPerSide - 1)));
				vertex_pointer++;
			}
		}

		memset(HeightMap, 0, sizeof(float) * VertexCountPerSide * VertexCountPerSide);

		size_t indexPointer = 0;
		for (size_t gz = 0; gz < VertexCountPerSide - 1; gz++) {
			for (size_t gx = 0; gx < VertexCountPerSide - 1; gx++) {
				auto top_left = (gz * VertexCountPerSide) + gx;
				auto top_right = top_left + 1;
				auto bottom_left = ((gz + 1) * VertexCountPerSide) + gx;
				auto bottom_right = bottom_left + 1;

				indices[indexPointer++] = static_cast<uint32_t>(bottom_right);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(top_right);

				indices[indexPointer++] = static_cast<uint32_t>(top_right);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(top_left);
			}
		}

		const auto face_norms = static_cast<glm::vec3*>(Allocator::Alloc(sizeof(glm::vec3) * VertexCountPerSide * VertexCountPerSide));
		const auto at_loc = static_cast<bool*>(Allocator::Alloc(sizeof(bool) * VertexCountPerSide * VertexCountPerSide));
		memset(at_loc, false, sizeof(bool) * VertexCountPerSide * VertexCountPerSide);

		for (size_t i = 0; i < (6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)); i += 3)
		{
			auto i1 = indices[i + 0];
			auto i2 = indices[i + 1];
			auto i3 = indices[i + 2];

			auto p1 = glm::vec3(positions[3 * i1], positions[3 * i1 + 1], positions[3 * i1 + 2]);
			auto p2 = glm::vec3(positions[3 * i2], positions[3 * i2 + 1], positions[3 * i2 + 2]);
			auto p3 = glm::vec3(positions[3 * i3], positions[3 * i3 + 1], positions[3 * i3 + 2]);

			glm::vec3 normal = glm::cross(p3 - p1, p3 - p2);
			normal *= glm::vec3{ -1, 1, -1 };
			auto tmp = normal;
			tmp.x = -normal.z;
			tmp.z = normal.x;
			normal = tmp;
			if (!at_loc[i1])
			{
				face_norms[i1] = normal;
				at_loc[i1] = true;
			}
			else
			{
				face_norms[i1] += normal;
			}

			if (!at_loc[i2])
			{
				face_norms[i2] = normal;
				at_loc[i2] = true;
			}
			else
			{
				face_norms[i2] += normal;
			}

			if (!at_loc[i3])
			{
				face_norms[i3] = normal;
				at_loc[i3] = true;
			}
			else
			{
				face_norms[i3] += normal;
			}
		}

		for (size_t i = 0; i < (VertexCountPerSide * VertexCountPerSide); i++)
		{
			face_norms[i] = glm::normalize(face_norms[i]);
			normals[3 * i] = face_norms[i].z;
			normals[3 * i + 1] = face_norms[i].y;
			normals[3 * i + 2] = face_norms[i].x;
		}

		Allocator::Free(face_norms);
		Allocator::Free(at_loc);


		auto model = Context::LoadMesh(new Mesh(positions, uvs, normals, indices, VertexCountPerSide * VertexCountPerSide, 6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)));

		Allocator::Free(positions);
		Allocator::Free(uvs);
		Allocator::Free(normals);
		Allocator::Free(indices);

		auto t = new Terrain(VertexCountPerSide, MaterialHandle, model);

		t->HeightMap_ = HeightMap;
		t->VertexCountPerSide_ = VertexCountPerSide;
		return t;
	}

	Terrain * Terrain::GenerateTerrainFromMap(unsigned int VertexCountPerSide, Material * MaterialHandle, Transformation * TransformHandle, std::vector<float> HeightMap, float Amplitude)
	{
		std::vector<float32_t> positions(3 * VertexCountPerSide * VertexCountPerSide); // preallocate the vertices
		std::vector<float32_t> uvs(3 * VertexCountPerSide * VertexCountPerSide); // preallocate the uvs
		std::vector<uint32_t>  indices(6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1));

		for (size_t x = 0; x < VertexCountPerSide; x++)
		{
			for (size_t z = 0; z < VertexCountPerSide; z++)
			{
				positions[3 * (z + x * VertexCountPerSide)] = (z / (float)(VertexCountPerSide - 1));
				auto height = HeightMap[x * VertexCountPerSide + z] * Amplitude;
				positions[3 * (z + x * VertexCountPerSide) + 1] = (height);
				positions[3 * (z + x * VertexCountPerSide) + 2] = (x / (float)(VertexCountPerSide - 1));

				uvs[3 * (z + x * VertexCountPerSide) + 0] = static_cast<float>(0);
				uvs[3 * (z + x * VertexCountPerSide) + 1] = static_cast<float>((z / (VertexCountPerSide - 1)));
				uvs[3 * (z + x * VertexCountPerSide) + 2] = static_cast<float>((x / (VertexCountPerSide - 1)));
			}
		}

		size_t indexPointer = 0;

		for (size_t gz = 0; gz < VertexCountPerSide - 1; gz++) {
			for (size_t gx = 0; gx < VertexCountPerSide - 1; gx++) {
				auto top_left = (gz * VertexCountPerSide) + gx;
				auto top_right = top_left + 1;
				auto bottom_left = ((gz + 1) * VertexCountPerSide) + gx;
				auto bottom_right = bottom_left + 1;

				indices[indexPointer++] = static_cast<uint32_t>(top_left);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(top_right);
				indices[indexPointer++] = static_cast<uint32_t>(top_right);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_right);
			}
		}

		std::vector<float32_t> normals(3 * VertexCountPerSide * VertexCountPerSide);

		std::unordered_map<uint32_t, glm::vec3> face_normals;
		for (auto i = 0; i < indices.size(); i += 3)
		{
			auto i1 = indices[i + 0];
			auto i2 = indices[i + 1];
			auto i3 = indices[i + 2];

			auto p1 = glm::vec3(positions[3 * i1], positions[3 * i1 + 1], positions[3 * i1 + 2]);
			auto p2 = glm::vec3(positions[3 * i2], positions[3 * i2 + 1], positions[3 * i2 + 2]);
			auto p3 = glm::vec3(positions[3 * i3], positions[3 * i3 + 1], positions[3 * i3 + 2]);

			glm::vec3 normal = glm::cross(p3 - p2, p3 - p1);
			if (face_normals.find(i1) == face_normals.end())
			{
				face_normals.insert(std::pair<uint32_t, glm::vec3>(i1, normal));
			}
			else
			{
				face_normals.find(i1)->second += normal;
			}

			if (face_normals.find(i2) == face_normals.end())
			{
				face_normals.insert(std::pair<uint32_t, glm::vec3>(i2, normal));
			}
			else
			{
				face_normals.find(i2)->second += normal;
			}

			if (face_normals.find(i3) == face_normals.end())
			{
				face_normals.insert(std::pair<uint32_t, glm::vec3>(i3, normal));
			}
			else
			{
				face_normals.find(i3)->second += normal;
			}
		}

		for (auto i = 0; i < face_normals.size(); i++)
		{
			face_normals.at(i) = glm::normalize(face_normals.at(i));
			normals[3 * i] = face_normals.at(i).z;
			normals[3 * i + 1] = face_normals.at(i).y;
			normals[3 * i + 2] = face_normals.at(i).x;
		}

		const auto mesh = new Mesh(positions, uvs, normals, indices);
		auto model = Context::LoadMesh(mesh);
		auto t = new Terrain(VertexCountPerSide, MaterialHandle, model);

		t->HeightMap_ = new float[VertexCountPerSide * VertexCountPerSide];
		t->VertexCountPerSide_ = VertexCountPerSide;
		for (auto i = 0; i < positions.size(); i += 3) t->HeightMap_[(i - 1) / 3] = positions[i];
		return t;
	}
	
	Terrain * Terrain::GenerateTerrainFromNoiseFunction(unsigned int VertexCountPerSide, Material * MaterialHandle, Transformation * TransformHandle, FastNoise & Noise, const float Amplitude, const unsigned int Size)
	{
		VertexCountPerSide += 1;
		float * positions = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		float * uvs = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		float * normals = static_cast<float*>(Allocator::Alloc(sizeof(float) * 3 * VertexCountPerSide * VertexCountPerSide));
		unsigned int * indices = static_cast<unsigned int*>(Allocator::Alloc(sizeof(unsigned int) * 6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)));

		size_t vertex_pointer = 0;
		for (size_t x = 0; x < VertexCountPerSide; x++)
		{
			for (size_t z = 0; z < VertexCountPerSide; z++)
			{
				positions[3 * vertex_pointer + 0] = (static_cast<float>(x) / (VertexCountPerSide - 1)) * Size;
				const auto height = Noise.GetValue(x + (TransformHandle->GetPosition().x * (VertexCountPerSide - 1)), z + (TransformHandle->GetPosition().z * (VertexCountPerSide - 1))) * Amplitude;
				positions[3 * vertex_pointer + 1] = height;
				positions[3 * vertex_pointer + 2] = (static_cast<float>(z) / (VertexCountPerSide - 1)) * Size;

				uvs[3 * vertex_pointer + 0] = static_cast<float>(0);
				uvs[3 * vertex_pointer + 1] = static_cast<float>((z / (VertexCountPerSide - 1)));
				uvs[3 * vertex_pointer + 2] = static_cast<float>((x / (VertexCountPerSide - 1)));
				vertex_pointer++;
			}
		}

		TransformHandle->SetPosition(TransformHandle->GetPosition() * static_cast<float>(Size));
		TransformHandle->SetScale(glm::vec3(1, 1, 1));

		size_t indexPointer = 0;
		for (size_t gz = 0; gz < VertexCountPerSide - 1; gz++) {
			for (size_t gx = 0; gx < VertexCountPerSide - 1; gx++) {
				auto top_left = (gz * VertexCountPerSide) + gx;
				auto top_right = top_left + 1;
				auto bottom_left = ((gz + 1) * VertexCountPerSide) + gx;
				auto bottom_right = bottom_left + 1;

				indices[indexPointer++] = static_cast<uint32_t>(top_right);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(top_left);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_right);
				indices[indexPointer++] = static_cast<uint32_t>(bottom_left);
				indices[indexPointer++] = static_cast<uint32_t>(top_right);
			}
		}

		const auto face_norms = static_cast<glm::vec3*>(Allocator::Alloc(sizeof(glm::vec3) * VertexCountPerSide * VertexCountPerSide));
		const auto at_loc = static_cast<bool*>(Allocator::Alloc(sizeof(bool) * VertexCountPerSide * VertexCountPerSide));
		memset(at_loc, false, sizeof(bool) * VertexCountPerSide * VertexCountPerSide);

		for (size_t i = 0; i < (6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1)); i += 3)
		{
			auto i1 = indices[i + 0];
			auto i2 = indices[i + 1];
			auto i3 = indices[i + 2];

			auto p1 = glm::vec3(positions[3 * i1], positions[3 * i1 + 1], positions[3 * i1 + 2]);
			auto p2 = glm::vec3(positions[3 * i2], positions[3 * i2 + 1], positions[3 * i2 + 2]);
			auto p3 = glm::vec3(positions[3 * i3], positions[3 * i3 + 1], positions[3 * i3 + 2]);

			glm::vec3 normal = glm::cross(p3 - p1, p3 - p2);
			normal *= glm::vec3{ -1, 1, -1 };
			auto tmp = normal;
			tmp.x = -normal.z;
			tmp.z = normal.x;
			normal = tmp;
			if (!at_loc[i1])
			{
				face_norms[i1] = normal;
				at_loc[i1] = true;
			}
			else
			{
				face_norms[i1] += normal;
			}

			if (!at_loc[i2])
			{
				face_norms[i2] = normal;
				at_loc[i2] = true;
			}
			else
			{
				face_norms[i2] += normal;
			}

			if (!at_loc[i3])
			{
				face_norms[i3] = normal;
				at_loc[i3] = true;
			}
			else
			{
				face_norms[i3] += normal;
			}
		}

		for (size_t i = 0; i < (VertexCountPerSide * VertexCountPerSide); i++)
		{
			face_norms[i] = glm::normalize(face_norms[i]);
			normals[3 * i] = face_norms[i].z;
			normals[3 * i + 1] = face_norms[i].y;
			normals[3 * i + 2] = face_norms[i].x;
		}

		Allocator::Free(face_norms);
		Allocator::Free(at_loc);
		
		const auto mesh = new Mesh(positions, uvs, normals, indices, VertexCountPerSide * VertexCountPerSide, 6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1));
		auto model = Context::LoadMesh(mesh);
		auto t = new Terrain(VertexCountPerSide, MaterialHandle, model);

		t->HeightMap_ = new float[VertexCountPerSide * VertexCountPerSide];
		t->VertexCountPerSide_ = VertexCountPerSide;
		for (size_t i = 0; i < 3 * VertexCountPerSide * VertexCountPerSide; i += 3) t->HeightMap_[(i - 1) / 3] = positions[i];

		Allocator::Free(positions);
		Allocator::Free(uvs);
		Allocator::Free(normals);
		Allocator::Free(indices);

		return t;
	}

	Terrain* Terrain::GenerateTerrainFromNoiseFunction(unsigned VertexCountPerSide, unsigned Samples, Material* MaterialHandle, Transformation* TransformHandle, FastNoise& Noise, const float32_t Amplitude, const unsigned int Size)
	{
		FLUFF_ASSERT(VertexCountPerSide >= Samples)
		FLUFF_ASSERT((VertexCountPerSide % Samples) == 0)
		const auto division = VertexCountPerSide / Samples;
		VertexCountPerSide += 1;
		Samples += 1;

		std::vector<float> positions(VertexCountPerSide * VertexCountPerSide * 3);
		std::vector<float> uvs(VertexCountPerSide * VertexCountPerSide * 3);
		std::vector<float> normals(VertexCountPerSide * VertexCountPerSide * 3);
		std::vector<unsigned int> indices(6 * (VertexCountPerSide - 1) * (VertexCountPerSide - 1));

		for (size_t z = 0; z < VertexCountPerSide; z++)
		{
			for (size_t x = 0; x < VertexCountPerSide; x++)
			{
				positions[3 * (z + x * VertexCountPerSide)] = (z / static_cast<float>(VertexCountPerSide - 1)) * Size;

				auto height = 0.0f;
				const uint32_t L = static_cast<uint32_t>(x / Samples + TransformHandle->GetPosition().x * (VertexCountPerSide - 1));
				const uint32_t R = static_cast<uint32_t>(x / Samples  + 1 + TransformHandle->GetPosition().x * (VertexCountPerSide - 1));
				const uint32_t T = static_cast<uint32_t>(z / Samples + 1 + TransformHandle->GetPosition().x * (VertexCountPerSide - 1));
				const uint32_t B = static_cast<uint32_t>(z / Samples + TransformHandle->GetPosition().x * (VertexCountPerSide - 1));

				height = Smooth(
					Noise.GetValue(static_cast<FN_DECIMAL>(B), static_cast<FN_DECIMAL>(L)),
					Noise.GetValue(static_cast<FN_DECIMAL>(B), static_cast<FN_DECIMAL>(R)),
					Noise.GetValue(static_cast<FN_DECIMAL>(T), static_cast<FN_DECIMAL>(L)),
					Noise.GetValue(static_cast<FN_DECIMAL>(T), static_cast<FN_DECIMAL>(R)), 
					static_cast<float>(x) / Samples - static_cast<float>(x), static_cast<float>(z) / Samples - static_cast<float>(z));

				positions[3 * (z + x * VertexCountPerSide) + 1] = (height * Amplitude);
				positions[3 * (z + x * VertexCountPerSide) + 2] = (x / static_cast<float>(VertexCountPerSide - 1)) * Size;

				uvs[3 * (z + x * VertexCountPerSide) + 0] = static_cast<float>(0);
				uvs[3 * (z + x * VertexCountPerSide) + 1] = static_cast<float>((z / (VertexCountPerSide - 1)));
				uvs[3 * (z + x * VertexCountPerSide) + 2] = static_cast<float>((x / (VertexCountPerSide - 1)));
			}
		}

		std::unordered_map<uint32_t, glm::vec3> face_normals;
		for (auto i = 0; i < indices.size(); i += 3)
		{
			auto i1 = indices[i + 0];
			auto i2 = indices[i + 1];
			auto i3 = indices[i + 2];

			auto p1 = glm::vec3(positions[3 * i1], positions[3 * i1 + 1], positions[3 * i1 + 2]);
			auto p2 = glm::vec3(positions[3 * i2], positions[3 * i2 + 1], positions[3 * i2 + 2]);
			auto p3 = glm::vec3(positions[3 * i3], positions[3 * i3 + 1], positions[3 * i3 + 2]);

			glm::vec3 normal = glm::cross(p3 - p1, p3 - p2);
			normal *= glm::vec3{ -1, 1, -1 };
			auto tmp = normal;
			tmp.x = -normal.z;
			tmp.z = normal.x;
			normal = tmp;
			if (face_normals.find(i1) == face_normals.end())
			{
				face_normals.insert(std::pair<uint32_t, glm::vec3>(i1, normal));
			}
			else
			{
				face_normals.find(i1)->second += normal;
			}

			if (face_normals.find(i2) == face_normals.end())
			{
				face_normals.insert(std::pair<uint32_t, glm::vec3>(i2, normal));
			}
			else
			{
				face_normals.find(i2)->second += normal;
			}

			if (face_normals.find(i3) == face_normals.end())
			{
				face_normals.insert(std::pair<uint32_t, glm::vec3>(i3, normal));
			}
			else
			{
				face_normals.find(i3)->second += normal;
			}
		}

		for (auto i = 0; i < face_normals.size(); i++)
		{
			face_normals.at(i) = glm::normalize(face_normals.at(i));
			normals[3 * i] = face_normals.at(i).z;
			normals[3 * i + 1] = face_normals.at(i).y;
			normals[3 * i + 2] = face_normals.at(i).x;
		}

		const auto mesh = new Mesh(positions, uvs, normals, indices);
		auto model = Context::LoadMesh(mesh);
		auto t = new Terrain(VertexCountPerSide, MaterialHandle, model);
		TransformHandle->SetPosition(TransformHandle->GetPosition() * static_cast<float>(Size));
		TransformHandle->SetScale(glm::vec3(1, 1, 1));

		t->HeightMap_ = new float[VertexCountPerSide * VertexCountPerSide];
		t->VertexCountPerSide_ = VertexCountPerSide;
		for (auto i = 0; i < positions.size(); i += 3) t->HeightMap_[(i - 1) / 3] = positions[i];
		return t;
	}

	Terrain * Terrain::GenerateTerrain(std::vector<float32_t> Positions, std::vector<float32_t> Normals, std::vector<float32_t> UVCoordinate, std::vector<uint32_t> Indices, Material * Mat)
	{
		const auto mesh = new Mesh(Positions, Normals, UVCoordinate, Indices);
		auto model = Context::LoadMesh(mesh);
		auto t = new Terrain(0, Mat);
		return t;
	}

	float Terrain::Smooth(const float BL, const float BR, const float TL, const float TR, const float X, const float Z)
	{
		const auto left = CosineInterpolation(BL, TL, X);
		const auto right = CosineInterpolation(BR, TR, X);
		return CosineInterpolation(left, right, Z);
	}
} }
