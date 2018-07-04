#include <serialization-legacy/serialization_bone.h>
#include <serialization-legacy/serialization_utilities.h>

namespace fluff { namespace utilities
{
	
	size_t SerializationBone::Size() const
	{
		size_t result = 0;
		result += sizeof BONE;
		result += sizeof(size_t);
		result += (sizeof(char) * Name.length());
		result += sizeof NULL_TERMINATION;
		result += sizeof(size_t);
		result += (sizeof(float) * 16);
		result += (WeightCount * (sizeof(float) + sizeof(unsigned int)));
		return result;
	}

	SerializationBone SerializationBone::CreateBone(std::string name)
	{
		SerializationBone bone;
		bone.Name = name;
		return bone;
	}

	void SerializationBone::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		SerializationUtils::WriteToArray(BONE, Dest, Pointer);
		SerializationUtils::WriteToArray(Name.length(), Dest, Pointer);
		SerializationUtils::WriteToArray(const_cast<char*>(Name.c_str()), Dest, Pointer, Name.length());
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(WeightCount, Dest, Pointer);
		SerializationUtils::WriteToArray(Offset, Dest, Pointer, 16);
		SerializationUtils::WriteToArray(Id, Dest, Pointer, WeightCount);
		SerializationUtils::WriteToArray(Weight, Dest, Pointer, WeightCount);
	}

	void SerializationBone::Deserialize(unsigned char* Src, SerializationBone& Dest, size_t& Pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(Src, namelength, Pointer);
		const auto name = new char[namelength + 1];
		SerializationUtils::ReadFromArray(Src, name, Pointer, namelength + 1);
		Dest.Name = name;
		delete[] name;
		SerializationUtils::ReadFromArray(Src, Dest.WeightCount, Pointer);

		Dest.Offset = new float[16];
		Dest.Id = new unsigned int[Dest.WeightCount];
		Dest.Weight = new float[Dest.WeightCount];

		Dest.Init = true;

		SerializationUtils::ReadFromArray(Src, Dest.Offset, Pointer, 16);
		SerializationUtils::ReadFromArray(Src, Dest.Id, Pointer, Dest.WeightCount);
		SerializationUtils::ReadFromArray(Src, Dest.Weight, Pointer, Dest.WeightCount);
	}

	void SerializationBone::Free()
	{
		if (Init)
		{
			delete[] Id;
			delete[] Weight;
			delete[] Offset;
			Init = false;
		}
	}

} }