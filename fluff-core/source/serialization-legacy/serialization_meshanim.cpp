#include <serialization-legacy/serialization_meshanim.h>
#include <serialization-legacy/serialization_utilities.h>

namespace fluff { namespace utilities
{

	void SerializationMeshAnim::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		SerializationUtils::WriteToArray(MESHANIM, Dest, Pointer);
		SerializationUtils::WriteToArray(Name.length(), Dest, Pointer);
		SerializationUtils::WriteToArray(const_cast<char*>(Name.c_str()), Dest, Pointer, Name.length());
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(KeyCount, Dest, Pointer);
		SerializationUtils::WriteToArray(KeyIndex, Dest, Pointer, KeyCount);
		SerializationUtils::WriteToArray(Time, Dest, Pointer, KeyCount);
	}

	void SerializationMeshAnim::Deserialize(unsigned char* Src, SerializationMeshAnim& Dest, size_t& Pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(Src, namelength, Pointer);
		const auto name = new char[namelength + 1];
		SerializationUtils::ReadFromArray(Src, name, Pointer, namelength + 1);
		Dest.Name = name;
		delete[] name;

		Dest.KeyIndex = new unsigned int[Dest.KeyCount];
		Dest.Time = new long double[Dest.KeyCount];

		Dest.Init = true;

		SerializationUtils::ReadFromArray(Src, Dest.KeyCount, Pointer);
		SerializationUtils::ReadFromArray(Src, Dest.KeyIndex, Pointer, Dest.KeyCount);
		SerializationUtils::ReadFromArray(Src, Dest.Time, Pointer, Dest.KeyCount);
	}

	size_t SerializationMeshAnim::Size() const
	{
		size_t result = 0;
		result += sizeof MESHANIM;
		result += sizeof(size_t);
		result += sizeof(char) * Name.length();
		result += sizeof NULL_TERMINATION;
		result += sizeof(size_t);
		result += (sizeof(unsigned int) * KeyCount);
		result += (sizeof(long double) * KeyCount);
		return result;
	}

	void SerializationMeshAnim::Free()
	{
		if (Init)
		{
			delete[] KeyIndex;
			delete[] Time;
			Init = false;
		}
	}

} }