#include <serialization-legacy/serialization_object.h>
#include <serialization-legacy/serialization_utilities.h>

namespace fluff { namespace utilities
{
	SerializationObject SerializationObject::Create(std::string Name)
	{
		SerializationObject obj;
		obj.name = Name;
		obj.arrays.resize(0);
		obj.fields.resize(0);
		obj.strings.resize(0);
		return obj;
	}

	void SerializationObject::Free()
	{
		for (auto array : arrays)  array.Free();
		for (auto field : fields)  field.Free();
		for (auto string : strings) string.Free();
	}

	void SerializationObject::AttachArray(SerializationArray& Array)
	{
		arrays.push_back(Array);
	}

	SerializationArray* SerializationObject::FindArray(std::string Name)
	{
		for (size_t i = 0; i < arrays.size(); i++)
		{
			if (arrays[i].Name == Name)
			{
				return &arrays[i];
			}
		}
		return nullptr;
	}

	void SerializationObject::AttachField(SerializationField& Field)
	{
		fields.push_back(Field);
	}

	SerializationField* SerializationObject::FindField(std::string Name)
	{
		for (size_t i = 0; i < fields.size(); i++)
		{
			if (fields[i].Name == Name)
			{
				return &fields[i];
			}
		}
		return nullptr;
	}

	void SerializationObject::AttachString(SerializationString string)
	{
		strings.push_back(string);
	}

	SerializationString* SerializationObject::FindString(std::string name)
	{
		for (size_t i = 0; i < strings.size(); i++)
		{
			if (strings[i].Name == name)
			{
				return &strings[i];
			}
		}
		return nullptr;
	}

	void SerializationObject::AttachObject(SerializationObject & Object)
	{
		objects.push_back(Object);
	}

	SerializationObject * SerializationObject::FindObject(std::string name)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i].name == name)
			{
				return &objects[i];
			}
		}
		return nullptr;
	}

	void SerializationObject::Serialize(unsigned char* dest, size_t& pointer) const
	{
		char* nameArray = (char*)name.c_str();
		size_t namelength = name.length();

		SerializationUtils::WriteToArray(OBJECT, dest, pointer);
		SerializationUtils::WriteToArray(namelength, dest, pointer);
		SerializationUtils::WriteToArray(nameArray, dest, pointer, namelength);
		SerializationUtils::WriteToArray(NULL_TERMINATION, dest, pointer);
		SerializationUtils::WriteToArray(arrays.size(), dest, pointer);
		for (SerializationArray array : arrays) array.Serialize(dest, pointer);
		SerializationUtils::WriteToArray(fields.size(), dest, pointer);
		for (SerializationField field : fields) field.Serialize(dest, pointer);
		SerializationUtils::WriteToArray(strings.size(), dest, pointer);
		for (SerializationString string : strings) string.Serialize(dest, pointer);
		SerializationUtils::WriteToArray(objects.size(), dest, pointer);
		for (SerializationObject object : objects) object.Serialize(dest, pointer);
	}

	void SerializationObject::Deserialize(unsigned char* src, SerializationObject& dest, size_t& pointer)
	{
		size_t namelength;
		SerializationUtils::ReadFromArray(src, namelength, pointer);
		char* name = (char*)malloc(namelength + 1);
		memset(name, NULL_TERMINATION, namelength + 1);
		SerializationUtils::ReadFromArray(src, name, pointer, namelength + 1);
		dest.name = name;
		free(name);
		size_t arraycount, fieldcount, stringcount, objectcount;
		SerializationUtils::ReadFromArray(src, arraycount, pointer);
		for (size_t i = 0; i < arraycount; i++)
		{
			uint32_t type;
			SerializationUtils::ReadFromArray(src, type, pointer);
#ifdef DEBUG
			if (type != ARRAY) __debugbreak();
#endif
			SerializationArray array;
			SerializationArray::Deserialize(src, array, pointer);
			dest.AttachArray(array);
		}

		SerializationUtils::ReadFromArray(src, fieldcount, pointer);
		for (size_t i = 0; i < fieldcount; i++)
		{
			uint32_t type;
			SerializationUtils::ReadFromArray(src, type, pointer);
#ifdef DEBUG
			if (type != FIELD) __debugbreak();
#endif
			SerializationField field;
			SerializationField::Deserialize(src, field, pointer);
			dest.AttachField(field);
		}

		SerializationUtils::ReadFromArray(src, stringcount, pointer);
		for (size_t i = 0; i < stringcount; i++)
		{
			uint32_t type;
			SerializationUtils::ReadFromArray(src, type, pointer);
#ifdef DEBUG
			if (type != STRING) __debugbreak();
#endif
			SerializationString string;
			SerializationString::Deserialize(src, string, pointer);
			dest.AttachString(string);
		}

		SerializationUtils::ReadFromArray(src, objectcount, pointer);
		for (size_t i = 0; i < objectcount; i++)
		{
			uint32_t type;
			SerializationUtils::ReadFromArray(src, type, pointer);
#ifdef DEBUG
			if (type != OBJECT) __debugbreak();
#endif
			SerializationObject string;
			SerializationObject::Deserialize(src, string, pointer);
			dest.AttachObject(string);
		}
	}

	size_t SerializationObject::Size() const
	{
		size_t result = 0;
		result += sizeof(uint32_t); // value type
		result += sizeof(size_t); // namelength
		result += name.length(); // name
		result += sizeof(NULL_TERMINATION); // null termination of name
		result += sizeof(size_t);
		for (SerializationArray arr : arrays) result += arr.Size();
		result += sizeof(size_t);
		for (SerializationField fie : fields) result += fie.Size();
		result += sizeof(size_t);
		for (SerializationString str : strings) result += str.Size();
		result += sizeof(size_t);
		for (SerializationObject obj : objects) result += obj.Size();
		return result;
	}

} }
