#include <core/assets/material_library.h>

namespace fluff {

	std::vector<std::string> MaterialLibrary::FileNames_;
	std::unordered_map<std::string, render::Material *> MaterialLibrary::MaterialMap_;

	void MaterialLibrary::LoadNoAdd(std::string FileName)
	{
		render::Material * pipeline = new render::Material;
		ReadFromFile(FileName, *pipeline);
		MaterialMap_.insert(std::make_pair(pipeline->Name, pipeline));
	}

	render::Material * MaterialLibrary::Load(std::string FileName)
	{
		render::Material * pipeline = new render::Material;
		FileNames_.push_back(FileName);
		ReadFromFile(FileName, *pipeline);
		MaterialMap_.insert(std::make_pair(pipeline->Name, pipeline));
		return pipeline;
	}

	render::Material * MaterialLibrary::Get(std::string TextureName)
	{
		auto it = MaterialMap_.find(TextureName);
		if (it == MaterialMap_.end())
		{
			FLUFF_ERROR_FUNC("MATERIAL COULD NOT BE FOUND");
			return nullptr;
		}
		return it->second;
	}

	void MaterialLibrary::Remove(std::string TextureName)
	{
		delete MaterialMap_[TextureName];
		MaterialMap_.erase(TextureName);
	}

	void MaterialLibrary::Clear()
	{
		for (auto i : MaterialMap_) delete i.second;
		MaterialMap_.clear();
	}

	void MaterialLibrary::SaveLibrary(std::string FileName)
	{
		MaterialLibrary lib;
		SendToFile(FileName, lib);
	}

	void MaterialLibrary::LoadLibrary(std::string FileName)
	{
		MaterialLibrary lib;
		ReadFromFile(FileName, lib);
	}

}