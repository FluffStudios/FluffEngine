#pragma once

#include <unordered_map>
#include <string>
#include <gfx/pipeline.h>
#include <vector>

#include <core/assets/util.h>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <core/ecs/ecs_manager.h>
#include <common.h>

namespace fluff {

	class PipelineLibrary
	{
		static std::vector<std::string> FileNames_;
		static std::unordered_map<std::string, gfx::GraphicsPipeline*> GraphicsPipelineMap_;

		static void LoadNoAdd(std::string FileName);
		
		friend class cereal::access;
		
		template <class Archive>
		void save(Archive & ar) const
		{
			ar(FileNames_);
		}

		template <class Archive>
		void load(Archive & ar)
		{
			ar(FileNames_);
			for (auto i : FileNames_) LoadNoAdd(i);
		}
	public:
		static gfx::GraphicsPipeline * Load(std::string Name);
		static gfx::GraphicsPipeline * Get(std::string Name);
		static void Remove(std::string Name);
		static void Clear();
		static void SaveLibrary(std::string FileName);
		static void LoadLibrary(std::string FileName);
		static void Create(std::shared_ptr<ECSManager> & pManager);
	};

}