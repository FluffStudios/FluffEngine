#include <core/assets/mesh_library.h>

namespace fluff {

	std::vector<std::string> MeshLibrary::FileNames_;
	std::unordered_map<std::string, gfx::Mesh *> MeshLibrary::MeshMap_;

	void MeshLibrary::LoadNoAdd(std::string FileName)
	{
		gfx::Mesh * pipeline = new gfx::Mesh;
		ReadFromFile(FileName, *pipeline);
		MeshMap_.insert(std::make_pair(pipeline->Name, pipeline));
	}

	gfx::Mesh * MeshLibrary::Load(std::string FileName)
	{
		gfx::Mesh * pipeline = new gfx::Mesh;
		FileNames_.push_back(FileName);
		ReadFromFile(FileName, *pipeline);
		MeshMap_.insert(std::make_pair(pipeline->Name, pipeline));
		return pipeline;
	}

	gfx::Mesh * MeshLibrary::Get(std::string TextureName)
	{
		auto it = MeshMap_.find(TextureName);
		if (it == MeshMap_.end())
		{
			FLUFF_ERROR_FUNC("MESH COULD NOT BE FOUND");
			return nullptr;
		}
		return it->second;
	}

	void MeshLibrary::Remove(std::string TextureName)
	{
		delete MeshMap_[TextureName];
		MeshMap_.erase(TextureName);
	}

	void MeshLibrary::Clear()
	{
		for (auto i : MeshMap_) delete i.second;
		MeshMap_.clear();
	}

	void MeshLibrary::SaveLibrary(std::string FileName)
	{
		MeshLibrary lib;
		SendToFile(FileName, lib);
	}

	void MeshLibrary::LoadLibrary(std::string FileName)
	{
		MeshLibrary lib;
		ReadFromFile(FileName, lib);
	}

}