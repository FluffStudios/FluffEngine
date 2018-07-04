#pragma once

#include <serial/archives/iarchive.h>
#include <serial/types/iserializable.h>

#include <unordered_map>
#include <string>

namespace fluff { namespace serial {

	class Database
	{
		IArchive & Archive;
	public:
		Database(const IArchive & Archive);
		~Database();
		void Add(std::shared_ptr<ISerializable> & Obj);
	};

} }