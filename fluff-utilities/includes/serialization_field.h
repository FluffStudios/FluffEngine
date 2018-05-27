#pragma once

#include "common.h"

namespace luminos { namespace utilities
{
	
	struct LUMINOS_API SerializationField
	{
		std::string Name;
		uint32_t Type;
		unsigned char* Value = nullptr;

		void Free();
		void Serialize(unsigned char* Dest, size_t& Pointer) const;
		static void Deserialize(unsigned char* Src, SerializationField& Dest, size_t& Pointer);
		static SerializationField Create(std::string name, bool Value);
		static SerializationField Create(std::string name, unsigned char Value);
		static SerializationField Create(std::string name, char Value);
		static SerializationField Create(std::string name, unsigned short Value);
		static SerializationField Create(std::string name, short Value);
		static SerializationField Create(std::string name, unsigned int Value);
		static SerializationField Create(std::string name, int Value);
		static SerializationField Create(std::string name, unsigned long long Value);
		static SerializationField Create(std::string name, long long Value);
		static SerializationField Create(std::string name, float Value);
		static SerializationField Create(std::string name, long double Value);
		static void ToBool(SerializationField& Src, bool& Dest);
		static void ToUnsignedChar(SerializationField& Src, unsigned char& Dest);
		static void ToChar(SerializationField& Src, char& Dest);
		static void ToUnsignedShort(SerializationField& Src, unsigned short& Dest);
		static void ToShort(SerializationField& Src, short& Dest);
		static void ToUnsignedInt(SerializationField& Src, unsigned int& Dest);
		static void ToInt(SerializationField& Src, int& Dest);
		static void ToUnsignedLong(SerializationField& Src, unsigned long long& Dest);
		static void ToLong(SerializationField& Src, long long& Dest);
		static void ToFloat(SerializationField& Src, float& Dest);
		static void ToDouble(SerializationField& Src, long double& Dest);
		size_t Size() const;
	};

} }
