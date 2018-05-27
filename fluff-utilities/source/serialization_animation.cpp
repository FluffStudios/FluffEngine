#include <serialization_animation.h>

#include <serialization_utilities.h>

namespace luminos { namespace utilities
{
	
	void SerializationAnimation::Free()
	{
		for (auto channel : Channels) channel.Free();
		for (auto channel : MeshChannels) channel.Free();
	}

	void SerializationAnimation::Serialize(unsigned char* dest, size_t& pointer)
	{
		SerializationUtils::WriteToArray(ANIMATION, dest, pointer);
		SerializationUtils::WriteToArray(Name.length(), dest, pointer);
		SerializationUtils::WriteToArray(const_cast<char*>(Name.c_str()), dest, pointer, Name.length());
		SerializationUtils::WriteToArray(NULL_TERMINATION, dest, pointer);
		SerializationUtils::WriteToArray(Duration, dest, pointer);
		SerializationUtils::WriteToArray(TicksPerSecond, dest, pointer);
		SerializationUtils::WriteToArray(NumChannels, dest, pointer);
		for (size_t i = 0; i < NumChannels; i++) Channels[i].Serialize(dest, pointer);
		SerializationUtils::WriteToArray(NumMeshChannels, dest, pointer);
		for (size_t i = 0; i < NumMeshChannels; i++) MeshChannels[i].Serialize(dest, pointer);
	}

	void SerializationAnimation::Deserialize(unsigned char* src, SerializationAnimation& dest, size_t& pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(src, namelength, pointer);
		const auto name = new char[namelength + 1];
		SerializationUtils::ReadFromArray(src, name, pointer, namelength + 1);
		dest.Name = name;
		delete[] name;
		SerializationUtils::ReadFromArray(src, dest.Duration, pointer);
		SerializationUtils::ReadFromArray(src, dest.TicksPerSecond, pointer);
		SerializationUtils::ReadFromArray(src, dest.NumChannels, pointer);
		for (size_t i = 0; i < dest.NumChannels; i++)
		{
			SerializationNodeAnim anim;
			unsigned int value;
			SerializationUtils::ReadFromArray(src, value, pointer);
#ifdef DEBUG
			if (value != NODEANIM) __debugbreak();
#endif
			SerializationNodeAnim::Deserialize(src, anim, pointer);
			dest.Channels.push_back(anim);
		}

		SerializationUtils::ReadFromArray(src, dest.NumMeshChannels, pointer);
		for (size_t i = 0; i < dest.NumMeshChannels; i++)
		{
			SerializationMeshAnim anim;
			unsigned int value;
			SerializationUtils::ReadFromArray(src, value, pointer);
#ifdef DEBUG
			if (value != MESHANIM) __debugbreak();
#endif
			SerializationMeshAnim::Deserialize(src, anim, pointer);
			dest.MeshChannels.push_back(anim);
		}
	}

	size_t SerializationAnimation::Size()
	{
		size_t result = 0;
		result += sizeof ANIMATION;
		result += sizeof size_t;
		result += (sizeof(char) * Name.length());
		result += sizeof NULL_TERMINATION;
		result += sizeof(long double);
		result += sizeof(long double);
		result += sizeof(size_t);
		for (size_t i = 0; i < NumChannels; i++) result += Channels[i].Size();
		result += sizeof(size_t);
		for (int i = 0; i < NumMeshChannels; i++) result += MeshChannels[i].Size();
		return result;
	}

} }
