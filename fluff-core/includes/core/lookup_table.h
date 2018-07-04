#pragma once

#include <unordered_map>
#include <string>
#include <serialization-legacy/serialization_database.h>
#include <functional>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
namespace fluff {

	class ComponentLookupTable
	{
		static std::unordered_map<std::string, int64_t> IDMap_;

		friend class cereal::access;
		template<class Archive>
		void serialize(Archive & ar)
		{
			ar(ComponentLookupTable::IDMap_);
		}
	public:
		ComponentLookupTable() { }

		template <class Component>
		static int64_t GetID()
		{
			auto it = IDMap_.find(typeid(Component).name());
			if (it == IDMap_.end()) return -1;
			else return it->second;
		}

		template <class Component>
		static void Add()
		{
			if (IDMap_.find(typeid(Component).name()) != IDMap_.end()) return;
			auto id = Component::GetID();
			IDMap_.insert(std::make_pair(typeid(Component).name(), (int64_t) id));
		}

		static uint64_t GetNextSlot()
		{
			return IDMap_.size();
		}
	};

	class GeneratorTable
	{
		static std::unordered_map<std::string, void*(*)()> map;
	public:
		template <class Type>
		static void Add()
		{
#ifdef _DEBUG
			if (map.find(typeid(Type).name()) != map.end())
			{
				FLUFF_ERROR_FUNC("Performance Notification: Map already contains type!")
			}
#endif
			map.insert(std::make_pair(typeid(Type).name(), []{
					return new Type;
				}));
		}

		static void * Generate(const std::string & TypeName);
	};

}