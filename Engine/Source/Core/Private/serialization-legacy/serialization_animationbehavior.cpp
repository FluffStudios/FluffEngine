#include <serialization-legacy/serialization_animationbehavior.h>
#include <serialization-legacy/serialization_utilities.h>

namespace fluff { namespace utilities
{
	void SerializationAnimationBehvaior::Serialize(unsigned char* Dest, size_t& Pointer) const
	{
		SerializationUtils::WriteToArray(ANIMBEHAVE, Dest, Pointer);
		SerializationUtils::WriteToArray(Behavior, Dest, Pointer);
	}

	void SerializationAnimationBehvaior::Deserialize(unsigned char* Src, SerializationAnimationBehvaior& Dest, size_t& Pointer)
	{
		unsigned int value;
		auto val = Dest.Behavior;
		SerializationUtils::ReadFromArray(Src, value, Pointer);
		switch (value)
		{
		case (DEFAULT):
		{
			val = DEFAULT;
			break;
		}
		case (CONSTANT):
		{
			val = CONSTANT;
			break;
		}
		case (LINEAR):
		{
			val = LINEAR;
			break;
		}
		case (REPEAT):
		{
			val = REPEAT;
			break;
		}
		default:
		{
#ifdef DEBUG
			__debugbreak();
#endif
			break;
		}
		}
	}

	size_t SerializationAnimationBehvaior::Size()
	{
		return sizeof(unsigned int) + sizeof(unsigned int);
	}
} }