#pragma once

#include "serialization_array.h"
#include "serialization_field.h"
#include "serialization_string.h"
#include <vector>

namespace luminos { namespace utilities
{
	
	struct LUMINOS_API SerializationObject
	{
		std::vector<SerializationArray> arrays;
		std::vector<SerializationField> fields;
		std::vector<SerializationString> strings;
		std::vector<SerializationObject> objects;
		std::string name;

		static SerializationObject Create(std::string name);
		void Free();
		void AttachArray(SerializationArray& array);
		SerializationArray* FindArray(std::string name);
		void AttachField(SerializationField& field);
		SerializationField* FindField(std::string name);
		void AttachString(SerializationString string);
		SerializationString* FindString(std::string name);
		void AttachObject(SerializationObject & Object);
		SerializationObject* FindObject(std::string name);
		void Serialize(unsigned char* dest, size_t& pointer) const;
		static void Deserialize(unsigned char* src, SerializationObject& dest, size_t& pointer);
		size_t Size() const;
	};

} }
