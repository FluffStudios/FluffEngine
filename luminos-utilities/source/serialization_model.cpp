#include <serialization_model.h>

#include <serialization_utilities.h>

namespace luminos { namespace utilities
{
	
	void SerializationModel::Free()
	{
		for (auto mesh : Meshes) mesh.Free();
		for (auto anim : Anims)  anim.Free();
	}

	void SerializationModel::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		SerializationUtils::WriteToArray(MODEL, Dest, Pointer);
		SerializationUtils::WriteToArray(Name.length(), Dest, Pointer);
		SerializationUtils::WriteToArray(const_cast<char*>(Name.c_str()), Dest, Pointer, Name.length());
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(MeshCount, Dest, Pointer);
		for (auto mesh : Meshes) mesh.Serialize(Dest, Pointer);
		SerializationUtils::WriteToArray(AnimCount, Dest, Pointer);
		for (auto anim : Anims) anim.Serialize(Dest, Pointer);
	}

	void SerializationModel::Deserialize(unsigned char* Src, SerializationModel& Dest, size_t& Pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(Src, namelength, Pointer);
		const auto name = new char[namelength + 1];
		SerializationUtils::ReadFromArray(Src, name, Pointer, namelength + 1);
		Dest.Name = name;
		delete[] name;
		SerializationUtils::ReadFromArray(Src, Dest.MeshCount, Pointer);
		for (size_t i = 0; i < Dest.MeshCount; i++)
		{
			unsigned int value;
			SerializationUtils::ReadFromArray(Src, value, Pointer);
#if defined(DEBUG) || defined(_DEBUG)
			if (value != MESH) __debugbreak();
#endif
			SerializationMesh mesh;
			SerializationMesh::Deserialize(Src, mesh, Pointer);
			Dest.Meshes.push_back(mesh);
		}

		SerializationUtils::ReadFromArray(Src, Dest.AnimCount, Pointer);
		for (size_t i = 0; i < Dest.AnimCount; i++)
		{
			unsigned int value;
			SerializationUtils::ReadFromArray(Src, value, Pointer);
#if defined(DEBUG) || defined(_DEBUG)
			if (value != ANIMATION) __debugbreak();
#endif
			SerializationAnimation anim;
			SerializationAnimation::Deserialize(Src, anim, Pointer);
			Dest.Anims.push_back(anim);
		}
	}

	size_t SerializationModel::Size() const
	{
		size_t result = 0;
		result += sizeof MODEL;
		result += sizeof(size_t);
		result += (sizeof(char) * Name.length());
		result += sizeof NULL_TERMINATION;

		result += sizeof(size_t);
		for (auto mesh : Meshes) result += mesh.Size();
		result += sizeof(size_t);
		for (auto anim : Anims) result += anim.Size();

		return result;
	}

} }