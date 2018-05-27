#include <serialization_mesh.h>

#include <serialization_utilities.h>

namespace luminos { namespace utilities
{
	
	size_t SerializationMesh::Size() const
	{
		size_t result = 0;
		result += sizeof MESH;
		result += sizeof(size_t);
		result += (sizeof(char) * Name.length());
		result += sizeof NULL_TERMINATION;

		result += sizeof(bool) * 7;

		if (HasIndices) result += (sizeof(unsigned int) * IndexCount);
		result += sizeof(VertexSerial) * VertexCount;

		result += sizeof(size_t);
		result += sizeof(size_t);
		result += sizeof(size_t);
		for (auto bone : Bones) result += bone.Size();
		return result;
	}

	SerializationMesh SerializationMesh::CreateMesh(std::string Name)
	{
		SerializationMesh mesh;
		mesh.Name = Name;
		return mesh;
	}

	void SerializationMesh::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		SerializationUtils::WriteToArray(MESH, Dest, Pointer);
		SerializationUtils::WriteToArray(Name.length(), Dest, Pointer);
		SerializationUtils::WriteToArray(const_cast<char*>(Name.c_str()), Dest, Pointer, Name.length());
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(HasVertices, Dest, Pointer);
		SerializationUtils::WriteToArray(HasUVs, Dest, Pointer);
		SerializationUtils::WriteToArray(HasNormals, Dest, Pointer);
		SerializationUtils::WriteToArray(HasTangents, Dest, Pointer);
		SerializationUtils::WriteToArray(HasDiffuse, Dest, Pointer);
		SerializationUtils::WriteToArray(HasSpecular, Dest, Pointer);
		SerializationUtils::WriteToArray(HasIndices, Dest, Pointer);
		SerializationUtils::WriteToArray(VertexCount, Dest, Pointer);
		SerializationUtils::WriteToArray(IndexCount, Dest, Pointer);
		SerializationUtils::WriteToArray(reinterpret_cast<float*>(VertexArray), Dest, Pointer, sizeof(VertexSerial) / sizeof(float) * VertexCount);
		if (HasIndices) SerializationUtils::WriteToArray(Indices, Dest, Pointer, IndexCount);

		SerializationUtils::WriteToArray(Bones.size(), Dest, Pointer);
		for (auto bone : Bones)
		{
			bone.Serialize(Dest, Pointer);
		}
	}

	void SerializationMesh::Deserialize(unsigned char* src, SerializationMesh& dest, size_t& pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(src, namelength, pointer);
		const auto name = new char[namelength + 1];
		SerializationUtils::ReadFromArray(src, name, pointer, namelength + 1);
		dest.Name = name;
		delete[] name;
		SerializationUtils::ReadFromArray(src, dest.HasVertices, pointer);
		SerializationUtils::ReadFromArray(src, dest.HasUVs, pointer);
		SerializationUtils::ReadFromArray(src, dest.HasNormals, pointer);
		SerializationUtils::ReadFromArray(src, dest.HasTangents, pointer);
		SerializationUtils::ReadFromArray(src, dest.HasDiffuse, pointer);
		SerializationUtils::ReadFromArray(src, dest.HasSpecular, pointer);
		SerializationUtils::ReadFromArray(src, dest.HasIndices, pointer);
		SerializationUtils::ReadFromArray(src, dest.VertexCount, pointer);
		SerializationUtils::ReadFromArray(src, dest.IndexCount, pointer);
		
		dest.VertexArray = new VertexSerial[dest.VertexCount];
		if (dest.HasIndices) dest.Indices = new unsigned int[dest.IndexCount];

		dest.Init = true;

		SerializationUtils::ReadFromArray(src, reinterpret_cast<float*>(dest.VertexArray), pointer, sizeof(VertexSerial) / sizeof(float) * dest.VertexCount);

		if (dest.HasIndices) SerializationUtils::ReadFromArray(src, dest.Indices, pointer, dest.IndexCount);

		size_t count;
		SerializationUtils::ReadFromArray(src, count, pointer);
		for (auto i = 0; i < count; i++)
		{
			SerializationBone bone;
			uint32_t type;
			SerializationUtils::ReadFromArray(src, type, pointer);
#ifdef DEBUG
			if (type != BONE) __debugbreak();
#endif
			SerializationBone::Deserialize(src, bone, pointer);
			dest.Bones.push_back(bone);
		}
	}

	void SerializationMesh::Free()
	{
		if (Init)
		{
			delete[] VertexArray;
			delete[] Indices;
			for (auto bone : Bones) bone.Free();
			Init = false;
		}
	}

} }
