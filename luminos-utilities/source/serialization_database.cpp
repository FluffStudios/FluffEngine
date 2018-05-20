#include <serialization_database.h>

#include <serialization_utilities.h>

namespace luminos { namespace utilities
{
	void Database::Find(std::string name, SerializationObject& object)
	{
		for (const auto obj : objects)
		{
			if (obj.name == name)
			{
				object = obj;
				return;
			}
		}

		for (const auto pair : locations)
		{
			if (pair.first == name)
			{
				size_t pointer = offset + pair.second;
				unsigned int type;
				SerializationUtils::ReadFromArray(Data, type, pointer);
#ifdef DEBUG
				if (type != OBJECT) __debugbreak();
#endif
				SerializationObject::Deserialize(Data, object, pointer);
				objects.push_back(object);
				return;
			}
		}
	}

	void Database::Find(std::string name, SerializationModel& object)
	{
		for (const auto obj : models)
		{
			if (obj.Name == name)
			{
				object = obj;
				return;
			}
		}

		for (const auto pair : locations)
		{
			if (pair.first == name)
			{
				size_t pointer = offset + pair.second;
				unsigned int type;
				SerializationUtils::ReadFromArray(Data, type, pointer);
#ifdef DEBUG
				if (type != MODEL) __debugbreak();
#endif
				SerializationModel::Deserialize(Data, object, pointer);
				models.push_back(object);
				return;
			}
		}
	}

	void Database::Attach(const SerializationObject& obj)
	{
		if (locations.find(obj.name) != locations.end()) LUMINOS_ERROR_FUNC("Item with provided name is already in database!")
		locations.insert(std::pair<std::string, size_t>(obj.name, pointer));
		objects.push_back(obj);
		pointer += obj.Size();
	}

	void Database::Attach(const SerializationModel& mod)
	{
		if (locations.find(mod.Name) != locations.end()) LUMINOS_ERROR_FUNC("Item with provided name is already in database!")
		locations.insert(std::pair<std::string, size_t>(mod.Name, pointer));
		models.push_back(mod);
		pointer += mod.Size();
	}

	size_t Database::Size()
	{
		size_t result = 0;
		result += sizeof(uint32_t);
		result += sizeof(size_t);
		result += sizeof(size_t);
		result += name.length();
		result += sizeof(NULL_TERMINATION);
		result += sizeof(size_t);
		for (auto it : locations)
		{
			result += sizeof(size_t);
			result += it.first.length();
			result += sizeof(NULL_TERMINATION);
			result += sizeof(size_t);
		}
		result += sizeof(size_t);
		result += sizeof(size_t);
		for (auto obj : objects) result += obj.Size();
		for (auto obj : models) result += obj.Size();
		return result;
	}

	void Database::Free()
	{
		for (auto mod : models)  mod.Free();
		for (auto obj : objects) obj.Free();
		if (Data != NULL)
			free(Data);
	}

	unsigned char* Database::Serialize()
	{
		Free();
		const auto size = Size();
		Data = static_cast<unsigned char*>(malloc(size));
		size_t pointer = 0;
		SerializationUtils::WriteToArray(DATABASE, Data, pointer);
		SerializationUtils::WriteToArray(size, Data, pointer);
		const auto name_array = const_cast<char*>(name.c_str());
		const auto namelength = name.length();
		SerializationUtils::WriteToArray(namelength, Data, pointer);
		SerializationUtils::WriteToArray(name_array, Data, pointer, namelength);
		SerializationUtils::WriteToArray(NULL_TERMINATION, Data, pointer);
		const auto objectCount = objects.size();
		SerializationUtils::WriteToArray(objectCount, Data, pointer);
		const auto model_count = models.size();
		SerializationUtils::WriteToArray(model_count, Data, pointer);
		for (auto it : locations)
		{
			const auto obj_name_length = it.first.size();
			const auto obj_name = const_cast<char*>(it.first.c_str());
			const auto location = it.second;
			SerializationUtils::WriteToArray(obj_name_length, Data, pointer);
			SerializationUtils::WriteToArray(obj_name, Data, pointer, obj_name_length);
			SerializationUtils::WriteToArray(NULL_TERMINATION, Data, pointer);
			SerializationUtils::WriteToArray(location, Data, pointer);
		}
		offset = pointer + sizeof pointer;
		SerializationUtils::WriteToArray(offset, Data, pointer); // Data offset
		for (auto obj : objects)
			obj.Serialize(Data, pointer);
		for (auto mod : models)
			mod.Serialize(Data, pointer);
		return Data;
	}

	Database Database::Deserialize(unsigned char* src, bool LoadAll)
	{
		Database result;
		size_t pointer = 0;
		size_t size;
		uint32_t type;

		SerializationUtils::ReadFromArray(src, type, pointer);
		if (type != DATABASE) __debugbreak();
		SerializationUtils::ReadFromArray(src, size, pointer);
		size_t namelength;
		SerializationUtils::ReadFromArray(src, namelength, pointer);
#ifdef DEBUG
		if (size < pointer) __debugbreak();
#endif
		char* name = new char[namelength + 1];
		memset(name, NULL_TERMINATION, namelength + 1);
		SerializationUtils::ReadFromArray(src, name, pointer, namelength + 1);
#ifdef DEBUG
		if (size < pointer) __debugbreak();
#endif
		result.name = name;
		delete[] name;
		name = nullptr;

		size_t objectCount;
		size_t modelCount;
		size_t location;
		SerializationUtils::ReadFromArray(src, objectCount, pointer);
		SerializationUtils::ReadFromArray(src, modelCount, pointer);

#ifdef DEBUG
		if (size < pointer) __debugbreak();
#endif
		for (size_t i = 0; i < objectCount; i++)
		{
			size_t namelength;
			SerializationUtils::ReadFromArray(src, namelength, pointer);
#ifdef DEBUG
			if (size < pointer) __debugbreak();
#endif
			name = new char[namelength + 1];
			memset(name, NULL_TERMINATION, namelength + 1);
			SerializationUtils::ReadFromArray(src, name, pointer, namelength + 1);
			std::string objName = name;
			delete[] name;
			name = nullptr;
			SerializationUtils::ReadFromArray(src, location, pointer);
#ifdef DEBUG
			if (size < pointer) __debugbreak();
#endif
			result.locations.insert(std::pair<std::string, size_t>(objName, location));
		}

		for (size_t i = 0; i < modelCount; i++)
		{
			size_t namelength;
			SerializationUtils::ReadFromArray(src, namelength, pointer);
#ifdef DEBUG
			if (size < pointer) __debugbreak();
#endif
			name = new char[namelength + 1];
			memset(name, NULL_TERMINATION, namelength + 1);
			SerializationUtils::ReadFromArray(src, name, pointer, namelength + 1);
			std::string objName = name;
			delete[] name;
			name = nullptr;
			SerializationUtils::ReadFromArray(src, location, pointer);
#ifdef DEBUG
			if (size < pointer) __debugbreak();
#endif
			result.locations.insert(std::pair<std::string, size_t>(objName, location));
		}

		SerializationUtils::ReadFromArray(src, result.offset, pointer);
		if (LoadAll)
		{
			for (size_t i = 0; i < objectCount; i++)
			{
				uint32_t objtype;
				SerializationUtils::ReadFromArray(src, objtype, pointer);
#ifdef DEBUG
				if (objtype != OBJECT) __debugbreak();
				if (size < pointer) __debugbreak();
#endif
				SerializationObject obj;
				SerializationObject::Deserialize(src, obj, pointer);
#ifdef DEBUG
				if (size < pointer) __debugbreak();
#endif
				result.objects.push_back(obj);
			}

			for (size_t i = 0; i < modelCount; i++)
			{
				uint32_t objtype;
				SerializationUtils::ReadFromArray(src, objtype, pointer);
#ifdef DEBUG
				if (objtype != MODEL) __debugbreak();
				if (size < pointer) __debugbreak();
#endif
				SerializationModel obj;
				SerializationModel::Deserialize(src, obj, pointer);
#ifdef DEBUG
				if (size < pointer) __debugbreak();
#endif
				result.models.push_back(obj);
			}
		}
		result.Data = src;
		return result;
	}

	void Database::SendToFile(const char* filename)
	{
		unsigned char* data = Serialize();
		FILE* file;
		fopen_s(&file, filename, "wb+");
		fwrite(data, sizeof(unsigned char), Size(), file);
		fclose(file);
	}

	void Database::ReadFromFile(Database& db, const char* filename, bool LoadAll)
	{
		FILE* file;
		fopen_s(&file, filename, "rb");
		fseek(file, 0, SEEK_END);
		const auto size = ftell(file);
		fseek(file, 0, SEEK_SET);

		const auto data = new unsigned char[size];
		fread(data, size, 1, file);
#ifdef DEBUG
		if (ferror(file))
		{
			delete[] data;
			__debugbreak();
			return;
		}
#endif
		db = Deserialize(data, LoadAll);
		db.Data = data;
		fclose(file);
	}
} }
