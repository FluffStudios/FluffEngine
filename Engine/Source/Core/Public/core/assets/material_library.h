#pragma once

#include <rendering/material.h>
#include <string>
#include <vector>

#include <cereal/access.hpp>

namespace fluff {

	class FLUFF_API MaterialLibrary
	{
		static std::vector<std::string> FileNames_;
		static std::unordered_map<std::string, render::Material *> MaterialMap_;

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
			for (const auto it : FileNames_) LoadNoAdd(it);
		}
	public:
		static render::Material * Load(std::string FileName);
		static render::Material * Get(std::string TextureName);
		static void Remove(std::string TextureName);
		static void Clear();
		static void SaveLibrary(std::string FileName);
		static void LoadLibrary(std::string FileName);
	};

}