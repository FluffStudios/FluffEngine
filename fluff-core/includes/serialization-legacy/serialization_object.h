#pragma once

#include <common.h>

#include <serialization-legacy/serialization_array.h>
#include <serialization-legacy/serialization_field.h>
#include <serialization-legacy/serialization_string.h>
#include <vector>

namespace fluff { namespace utilities
{
	
	struct SerializationObject
	{
		std::vector<SerializationArray> arrays;
		std::vector<SerializationField> fields;
		std::vector<SerializationString> strings;
		std::vector<SerializationObject> objects;
		std::string name;

		static SerializationObject Create(std::string name);
		void FLUFF_API Free();
		void FLUFF_API AttachArray(SerializationArray& array);
		SerializationArray FLUFF_API* FindArray(std::string name);
		void FLUFF_API AttachField(SerializationField& field);
		SerializationField FLUFF_API* FindField(std::string name);
		void FLUFF_API AttachString(SerializationString string);
		SerializationString FLUFF_API* FindString(std::string name);
		void FLUFF_API AttachObject(SerializationObject & Object);
		SerializationObject FLUFF_API* FindObject(std::string name);
		void FLUFF_API Serialize(unsigned char* dest, size_t& pointer) const;
		static void Deserialize(unsigned char* src, SerializationObject& dest, size_t& pointer);
		size_t FLUFF_API Size() const;
	};

} }
