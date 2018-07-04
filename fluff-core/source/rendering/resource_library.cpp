#include <rendering/resource_library.h>

#include <cereal/external/rapidjson/document.h>
#include <cereal/external/rapidjson/prettywriter.h>
#include <cstdio>

namespace fluff { namespace render {

	ResourceLibrary ResourceLibrary::Instance_;

	ResourceLibrary& ResourceLibrary::GetInstance()
	{
		return Instance_;
	}

	void ResourceLibrary::AddPipeline(std::string Name, void * Pipeline)
	{
		Instance_.GPipelines_.insert(std::make_pair(Name, Pipeline));
	}

	void ResourceLibrary::AddTexture(std::string Name, void * Texture)
	{
		Instance_.Textures_.insert(std::make_pair(Name, Texture));
	}

	void * ResourceLibrary::GetPipeline(std::string Name)
	{
		auto it = GPipelines_.find(Name);
		if (it != GPipelines_.end()) return it->second;
		return nullptr;
	}

	void * ResourceLibrary::GetTexture(std::string Name)
	{
		auto it = Textures_.find(Name);
		if (it != Textures_.end()) return it->second;
		return nullptr;
	}

} }