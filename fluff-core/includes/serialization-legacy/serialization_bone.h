#pragma once

#include <string>

#include <common.h>

namespace fluff { namespace utilities
{
	struct SerializationBone
	{
		std::string Name;
		size_t WeightCount = 0;
		bool Init = false;
		float* Offset = nullptr;
		unsigned int* Id = nullptr;
		float* Weight = nullptr;

		size_t FLUFF_API Size() const;
		static SerializationBone CreateBone(std::string name);
		void FLUFF_API Serialize(unsigned char* dest, size_t& pointer) const;
		static void Deserialize(unsigned char* src, SerializationBone& dest, size_t& pointer);
		void FLUFF_API Free();
	};

} }