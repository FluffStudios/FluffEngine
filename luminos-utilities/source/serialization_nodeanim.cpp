#include <serialization_nodeanim.h>

#include <serialization_utilities.h>
#include "../../depend/include/glm/detail/type_vec3.hpp"
#include "../../depend/include/glm/gtx/quaternion.hpp"

namespace luminos { namespace utilities
{

	void SerializationNodeAnim::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		SerializationUtils::WriteToArray(NODEANIM, Dest, Pointer);
		SerializationUtils::WriteToArray(Name.length(), Dest, Pointer);
		SerializationUtils::WriteToArray(const_cast<char*>(Name.c_str()), Dest, Pointer, Name.length());
		SerializationUtils::WriteToArray(NULL_TERMINATION, Dest, Pointer);
		SerializationUtils::WriteToArray(NumPositionKeys, Dest, Pointer);
		SerializationUtils::WriteToArray(NumRotationKeys, Dest, Pointer);
		SerializationUtils::WriteToArray(NumScalingKeys, Dest, Pointer);
		Pre.Serialize(Dest, Pointer);
		Post.Serialize(Dest, Pointer);
		SerializationUtils::WriteToArray(PositionKeys, Dest, Pointer, NumPositionKeys * 3);
		SerializationUtils::WriteToArray(PositionTimes, Dest, Pointer, NumPositionKeys);
		SerializationUtils::WriteToArray(RotationKeys, Dest, Pointer, NumRotationKeys * 4);
		SerializationUtils::WriteToArray(RotationTimes, Dest, Pointer, NumRotationKeys);
		SerializationUtils::WriteToArray(ScaleKeys, Dest, Pointer, NumScalingKeys * 3);
		SerializationUtils::WriteToArray(ScaleTimes, Dest, Pointer, NumScalingKeys);
	}

	void SerializationNodeAnim::Deserialize(unsigned char* Src, SerializationNodeAnim& Dest, size_t& Pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(Src, namelength, Pointer);
		const auto name = new char[namelength + 1];
		SerializationUtils::ReadFromArray(Src, name, Pointer, namelength + 1);
		Dest.Name = name;
		delete[] name;
		SerializationUtils::ReadFromArray(Src, Dest.NumPositionKeys, Pointer);
		SerializationUtils::ReadFromArray(Src, Dest.NumRotationKeys, Pointer);
		SerializationUtils::ReadFromArray(Src, Dest.NumScalingKeys, Pointer);
		unsigned int value;
		SerializationUtils::ReadFromArray(Src, value, Pointer);
#ifdef DEBUG
		if (value != ANIMBEHAVE) __debugbreak();
#endif
		SerializationAnimationBehvaior::Deserialize(Src, Dest.Pre, Pointer);

		SerializationUtils::ReadFromArray(Src, value, Pointer);
#ifdef DEBUG
		if (value != ANIMBEHAVE) __debugbreak();
#endif
		SerializationAnimationBehvaior::Deserialize(Src, Dest.Post, Pointer);

		Dest.PositionKeys = new float[3 * Dest.NumPositionKeys];
		Dest.PositionTimes = new long double[Dest.NumPositionKeys];
		Dest.RotationKeys = new float[4 * Dest.NumRotationKeys];
		Dest.RotationTimes = new long double[Dest.NumRotationKeys];
		Dest.ScaleKeys = new float[3 * Dest.NumScalingKeys];
		Dest.ScaleTimes = new long double[Dest.NumScalingKeys];

		Dest.Init = true;

		SerializationUtils::ReadFromArray(Src, Dest.PositionKeys, Pointer, 3 * Dest.NumPositionKeys);
		SerializationUtils::ReadFromArray(Src, Dest.PositionTimes, Pointer, Dest.NumPositionKeys);
		SerializationUtils::ReadFromArray(Src, Dest.RotationKeys, Pointer, 4 * Dest.NumRotationKeys);
		SerializationUtils::ReadFromArray(Src, Dest.RotationTimes, Pointer, Dest.NumRotationKeys);
		SerializationUtils::ReadFromArray(Src, Dest.ScaleKeys, Pointer, 3 * Dest.NumScalingKeys);
		SerializationUtils::ReadFromArray(Src, Dest.ScaleTimes, Pointer, Dest.NumScalingKeys);
	}

	void SerializationNodeAnim::Free()
	{
		if (Init)
		{
			delete[] PositionKeys;
			delete[] PositionTimes;
			delete[] RotationKeys;
			delete[] RotationTimes;
			delete[] ScaleKeys;
			delete[] ScaleTimes;

			Init = false;
		}
	}

	size_t SerializationNodeAnim::Size() const
	{
		size_t result = 0;
		result += sizeof NODEANIM;
		result += sizeof(size_t);
		result += (sizeof(char) * Name.length());
		result += sizeof NULL_TERMINATION;
		result += sizeof(size_t);
		result += sizeof(size_t);
		result += sizeof(size_t);
		result += Pre.Size();
		result += Post.Size();
		result += (NumPositionKeys * (sizeof(glm::vec3) + sizeof(long double)));
		result += (NumRotationKeys * (sizeof(glm::quat) + sizeof(long double)));
		result += (NumScalingKeys * (sizeof(glm::vec3) + sizeof(long double)));
		return result;
	}

} }
