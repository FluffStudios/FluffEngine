#pragma once

#include <unordered_map>
#include <string>
#include <gfx/texture.h>
#include <gfx/texture_data.h>

#include <core/assets/util.h>
#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <common.h>

namespace fluff {

	class FLUFF_API TextureLibrary
	{
		static std::vector<std::string> FileNames_;
		static std::unordered_map<std::string, gfx::Texture *> TextureMap_;

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
			for (const auto i : FileNames_)
			{
				LoadNoAdd(i);
			}
		}

		static void LoadNoAdd(std::string FileName);
	public:
		static gfx::Texture * Load(std::string FileName);
		static gfx::Texture * Get(std::string TextureName);
		static void Remove(std::string TextureName);
		static void Clear();
		static void SaveLibrary(std::string FileName);
		static void LoadLibrary(std::string FileName);
	};

}