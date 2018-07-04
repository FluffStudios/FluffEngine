#pragma once

#include <gfx/mesh.h>
#include <core/assets/util.h>
#include <string>
#include <vector>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

#include <common.h>
#include <core/ecs/ecs_manager.h>

namespace fluff {

	class FLUFF_API MeshLibrary
	{
		static std::vector<std::string> FileNames_;
		static std::unordered_map<std::string, gfx::Mesh *> MeshMap_;

		static void LoadNoAdd(std::string FileName);

		friend class cereal::access;
		
		template <class Archive>
		void save(Archive & ar) const
		{
			ar(FileNames_);
		}

		template <class Archive>
		void load(Archive & ar) const
		{
			ar(FileNames_);
			for (auto it : FileNames_) LoadNoAdd(it);
		}
	public:
		static gfx::Mesh * Load(std::string FileName);
		static gfx::Mesh * Get(std::string TextureName);
		static void Remove(std::string TextureName);
		static void Clear();
		static void SaveLibrary(std::string FileName);
		static void LoadLibrary(std::string FileName);
	};

}