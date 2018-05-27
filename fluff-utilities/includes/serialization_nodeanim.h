#pragma once

#include "serialization_animationbehavior.h"
#include "common.h"

namespace luminos { namespace utilities
{
	struct LUMINOS_API SerializationNodeAnim
	{
		bool Init = false;
		std::string Name;
		size_t NumPositionKeys;
		size_t NumRotationKeys;
		size_t NumScalingKeys;

		SerializationAnimationBehvaior Pre;
		SerializationAnimationBehvaior Post;
		
		// position
		float* PositionKeys;
		long double* PositionTimes;

		// rotation
		float* RotationKeys;
		long double* RotationTimes;

		// scale
		float* ScaleKeys;
		long double* ScaleTimes;
		void Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* Src, SerializationNodeAnim& Dest, size_t& Pointer);
		void Free();
		size_t Size() const;
	};

} }