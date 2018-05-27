#pragma once

#include "common.h"

#include "serialization_object.h"
#include "serialization_model.h"
#include <unordered_map>

namespace luminos { namespace utilities
{
	struct LUMINOS_API Database
	{
		std::unordered_map<std::string, size_t> locations;
		std::vector<SerializationObject> objects;
		std::vector<SerializationModel> models;
		std::string name;
		size_t pointer = 0;
		size_t offset = 0;
		unsigned char* Data = nullptr;

		void Find(std::string name, SerializationObject& object);
		void Find(std::string name, SerializationModel& object);
		void Attach(const SerializationObject& obj);
		void Attach(const SerializationModel& mod);
		size_t Size();
		void Free();
		unsigned char* Serialize();
		static Database Deserialize(unsigned char* src, bool LoadAll);
		void SendToFile(const char* filename);
		static void ReadFromFile(Database& db, const char* filename, bool LoadAll);
	};

} }
