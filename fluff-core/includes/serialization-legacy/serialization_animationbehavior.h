#pragma once

#include <serialization-legacy/serialization_animbehave.h>
#include <common.h>

namespace fluff { namespace utilities
{
	
	struct FLUFF_API SerializationAnimationBehvaior
	{
		SERIALIZATION_ANIMATION_BEHAVIOR Behavior;

		void Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* Src, SerializationAnimationBehvaior& Dest, size_t& Pointer);
		static size_t Size();
	};

} }
