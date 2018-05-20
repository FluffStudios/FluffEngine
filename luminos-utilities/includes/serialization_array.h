#pragma once

#include "common.h"

namespace luminos { namespace utilities
{
	
	struct LUMINOS_API SerializationArray
	{
		std::string Name;
		uint32_t Type;
		unsigned char *Value = nullptr;
		size_t Count;

		static SerializationArray Create(std::string Name, bool* Values, size_t Count);
		static SerializationArray Create(std::string Name, unsigned char* Values, size_t Count);
		static SerializationArray Create(std::string Name, char* Values, size_t Count);
		static SerializationArray Create(std::string Name, unsigned short* Values, size_t Count);
		static SerializationArray Create(std::string Name, short* Values, size_t Count);
		static SerializationArray Create(std::string Name, unsigned int* Values, size_t Count);
		static SerializationArray Create(std::string Name, int* Values, size_t Count);
		static SerializationArray Create(std::string Name, unsigned long long* Values, size_t Count);
		static SerializationArray Create(std::string Name, long long* Values, size_t Count);
		static SerializationArray Create(std::string Name, float* Values, size_t Count);
		static SerializationArray Create(std::string Name, long double* Values, size_t Count);
		static void ToBool(SerializationArray& Src, bool*& Dest);
		static void ToUnsignedChar(SerializationArray& Src, unsigned char*& Dest);
		static void ToChar(SerializationArray& Src, char*& Dest);
		static void ToUnsignedShort(SerializationArray& Src, unsigned short*& Dest);
		static void ToShort(SerializationArray& Src, short*& Dest);
		static void ToUnsignedInt(SerializationArray& Src, unsigned int*& Dest);
		static void ToInt(SerializationArray& Src, int*& Dest);
		static void ToUnsignedLong(SerializationArray& Src, unsigned long long*& Dest);
		static void ToLong(SerializationArray& Src, long long*& Dest);
		static void ToFloat(SerializationArray& Src, float*& Dest);
		static void ToDouble(SerializationArray& Src, long double*& Dest);
		void Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* Src, SerializationArray& Dest, size_t& Pointer);
		size_t Size() const;
		void Free();
	};

} }