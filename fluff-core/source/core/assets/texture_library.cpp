#include <core/assets/texture_library.h>

namespace luminos {

	std::unordered_map<std::string, gfx::Texture *> TextureLibrary::TextureMap_;
	std::vector<std::string> TextureLibrary::FileNames_;

	void TextureLibrary::LoadNoAdd(std::string FileName)
	{
		TextureData data;
		ReadFromFile(FileName, data);
		TextureMap_.insert(std::make_pair(data.GetName(), new gfx::Texture2D(data)));
	}

	gfx::Texture * TextureLibrary::Load(std::string FileName)
	{
		TextureData data;
		ReadFromFile(FileName, data);
		TextureMap_.insert(std::make_pair(data.GetName(), new gfx::Texture2D(data)));
		FileNames_.push_back(FileName);
		return TextureMap_.find(data.GetName())->second;
	}

	gfx::Texture * TextureLibrary::Get(std::string TextureName)
	{
		auto it = TextureMap_.find(TextureName);
		if (it == TextureMap_.end())
		{
			LUMINOS_ERROR_FUNC("TEXTURE COULD NOT BE FOUND");
			return nullptr;
		}
		return it->second;
	}

	void TextureLibrary::Remove(std::string TextureName)
	{
		delete TextureMap_[TextureName];
		TextureMap_.erase(TextureName);
	}

	void TextureLibrary::Clear()
	{
		for (auto i : TextureMap_) delete i.second;
		TextureMap_.clear();
		FileNames_.clear();
	}

	void TextureLibrary::LoadLibrary(std::string FileName)
	{
		TextureLibrary lib;
		ReadFromFile(FileName, lib);
	}

	void TextureLibrary::SaveLibrary(std::string FileName)
	{
		TextureLibrary lib;
		SendToFile(FileName, lib);
	}

}