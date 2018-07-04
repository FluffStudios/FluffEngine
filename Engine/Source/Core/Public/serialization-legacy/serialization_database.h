#pragma once

#include "common.h"

#include <serialization-legacy/serialization_object.h>
#include <serialization-legacy/serialization_model.h>
#include <unordered_map>

namespace fluff { namespace utilities
{
	struct Database
	{
		std::unordered_map<std::string, size_t> locations;
		std::vector<SerializationObject> objects;
		std::vector<SerializationModel> models;
		std::string name;
		size_t pointer = 0;
		size_t offset = 0;
		unsigned char* Data = nullptr;

		void FLUFF_API Find(std::string name, SerializationObject& object);
		void FLUFF_API Find(std::string name, SerializationModel& object);
		void FLUFF_API Attach(const SerializationObject& obj);
		void FLUFF_API Attach(const SerializationModel& mod);
		size_t FLUFF_API Size();
		void FLUFF_API Free();
		unsigned char FLUFF_API* Serialize();
		static Database Deserialize(unsigned char* src, bool LoadAll);
		void FLUFF_API SendToFile(const char* filename);
		static void ReadFromFile(Database& db, const char* filename, bool LoadAll);
	};

} }
