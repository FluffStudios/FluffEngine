#include <core/texture_data.h>
#include <stb_image.h>

namespace luminos {
	
	TextureData::TextureData()
	{
		Name_ = "";
		Buffer_.clear();
	}

	TextureData::TextureData(std::string Name, std::string File, int NumChannels)
	{
		CreateInfo_ = {
			gfx::FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_LINEAR,
			gfx::RGBA8,
			{
				true,
				-0.6f,
				2.0f
			},
			0,
			0,
			0,
			8,
			-0.6f,
			-50.0f,
			5.0f
		};

		Name_ = Name;
		stbi_set_flip_vertically_on_load(true);
		int bpp;
		int channels;
		stbi_info(File.c_str(), &CreateInfo_.Width, &CreateInfo_.Height, &channels);
		auto buf = stbi_load(File.c_str(), &CreateInfo_.Width, &CreateInfo_.Height, &bpp, NumChannels);
		LUMINOS_ASSERT(buf)
		Buffer_.resize(CreateInfo_.Width * CreateInfo_.Height * NumChannels * (bpp / channels));
		memcpy_s(Buffer_.data(), Buffer_.size(), buf, Buffer_.size());
		stbi_image_free(buf);
	}

	TextureData::TextureData(std::string Name, std::vector<unsigned char> Buffer, gfx::TextureCreateInfo CreateInfo)
		: Name_(Name), Buffer_(Buffer), CreateInfo_(CreateInfo)
	{	}

	std::vector<unsigned char> TextureData::GetBuffer() const
	{
		return Buffer_;
	}

	std::string TextureData::GetName() const
	{
		return Name_;
	}

	gfx::TextureCreateInfo TextureData::GetCreateInfo() const
	{
		return CreateInfo_;
	}

}