#include <gfx/texture.h>

namespace luminos { namespace gfx {

	std::unordered_map<GLuint, std::string> Texture::IDMap_;

	GLint GetFormat(TextureFormat Format)
	{
		switch (Format)
		{
		case RGBA32: return GL_RGBA32F;
		case RGBA16: return GL_RGBA16F;
		case RGBA8: return GL_RGBA8;
		case RGBA: return GL_RGBA;
		case RGB32: return GL_RGB32F;
		case RGB16: return GL_RGB16;
		case RGB8: return GL_RGB8;
		case RGB: return GL_RGB;
		default: return 0;
		}
	}

	void SetTextureFilter(unsigned int Target, TextureCreateInfo CreateInfo)
	{
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		if (major >= 4 && minor >= 5)
		{
			switch (CreateInfo.Filter)
			{
			case FILTER_MIN_NEAREST_MAG_NEAREST:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_NEAREST_MAG_LINEAR:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_LINEAR_MAG_NEAREST:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_LINEAR_MAG_LINEAR:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_NEAREST:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_LINEAR:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_NEAREST:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_LINEAR:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_NEAREST:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_LINEAR:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_NEAREST:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_LINEAR:
			{
				glTextureParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTextureParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_ANISOTROPIC:
			{
				GLfloat f_largest;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &f_largest);
				glTextureParameterf(Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, f_largest);
			}
			}
		}
		else
		{
			switch (CreateInfo.Filter)
			{
			case FILTER_MIN_NEAREST_MAG_NEAREST:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_NEAREST_MAG_LINEAR:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_LINEAR_MAG_NEAREST:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_LINEAR_MAG_LINEAR:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_NEAREST:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_LINEAR:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_NEAREST:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_LINEAR:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_NEAREST:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_LINEAR:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_NEAREST:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}
			case FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_LINEAR:
			{
				glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}
			case FILTER_ANISOTROPIC:
			{
				GLfloat f_largest;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &f_largest);
				glTexParameterf(Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, f_largest);
			}
			}
		}
	}

	Texture2D::Texture2D(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo)
	{
		this->CreateInfo_ = CreateInfo;
		Initialize(Data);
	}

	Texture2D::Texture2D(std::vector<unsigned char> Data, TextureCreateInfo CreateInfo)
	{
		this->CreateInfo_ = CreateInfo;
		Initialize(Data);
	}

	Texture2D::Texture2D(TextureData & Data)
	{
		this->CreateInfo_ = Data.GetCreateInfo();
		Initialize(Data.GetBuffer());
		this->SetName(Data.GetName());
	}

	void Texture2D::Initialize(std::vector<unsigned char*> &Data)
	{
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		auto width = CreateInfo_.Width;
		auto height = CreateInfo_.Height;

		if (major >= 4 && minor >= 5)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &Id_);
			glTextureStorage2D(Id_, CreateInfo_.Levels, GL_RGBA8, width, height);
			
			for (auto i = 0; i < static_cast<GLint>(CreateInfo_.Levels); i++)
			{
				if (i >= Data.size())
				{
				}
				else
				{
					glTextureSubImage2D(Id_, i, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, Data[i]);
				}
				width /= 2;
				height /= 2;
			}
			SetTextureFilter(Id_, CreateInfo_);
			if (CreateInfo_.AnisoFilter.Enabled)
			{
				float32_t anisoMax;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoMax);
				glTextureParameterf(Id_, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f > anisoMax ? anisoMax : 4.0f);
			}
			glTextureParameterf(Id_, GL_TEXTURE_MIN_LOD, CreateInfo_.MinimumLod);
			glTextureParameterf(Id_, GL_TEXTURE_MAX_LOD, CreateInfo_.MaximumLod);
			glTextureParameterf(Id_, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
			if (CreateInfo_.Levels > 1)
			{
				glTextureParameterf(Id_, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
				glGenerateTextureMipmap(Id_);
			}
		}
		else
		{
			glGenTextures(1, &Id_);
			
			glBindTexture(GL_TEXTURE_2D, Id_);
			glTexStorage2D(GL_TEXTURE_2D, CreateInfo_.Levels, GL_RGBA8, width, height);

			for (auto i = 0; i < static_cast<GLint>(CreateInfo_.Levels); i++)
			{
				if (i >= Data.size())
				{
				}
				else
				{
					glTexSubImage2D(GL_TEXTURE_2D, static_cast<int>(i), 0, 0, static_cast<size_t>(width), static_cast<size_t>(height), GL_RGBA, GL_UNSIGNED_BYTE, Data[i]);
				}
				width /= 2;
				height /= 2;
			}
			SetTextureFilter(GL_TEXTURE_2D, CreateInfo_);
			if (CreateInfo_.AnisoFilter.Enabled)
			{
				float32_t anisoMax;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoMax);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f > anisoMax ? anisoMax : 4.0f);
			}
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, CreateInfo_.MinimumLod);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, CreateInfo_.MaximumLod);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
			if (CreateInfo_.Levels > 1)
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}

		IDMap_.insert(std::make_pair(Id_, Name_));
		Enable();
	}

	void Texture2D::Initialize(std::vector<unsigned char> Data)
	{
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		auto width = CreateInfo_.Width;
		auto height = CreateInfo_.Height;

		if (major >= 4 && minor >= 5)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &Id_);
			glTextureStorage2D(Id_, CreateInfo_.Levels, GL_RGBA8, width, height);

			for (auto i = 0; i < static_cast<GLint>(CreateInfo_.Levels); i++)
			{
				if (i >= Data.size())
				{
				}
				else
				{
					glTextureSubImage2D(Id_, i, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, Data.data());
				}
				width /= 2;
				height /= 2;
			}
			SetTextureFilter(Id_, CreateInfo_);
			if (CreateInfo_.AnisoFilter.Enabled)
			{
				float32_t anisoMax;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoMax);
				glTextureParameterf(Id_, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f > anisoMax ? anisoMax : 4.0f);
			}
			glTextureParameterf(Id_, GL_TEXTURE_MIN_LOD, CreateInfo_.MinimumLod);
			glTextureParameterf(Id_, GL_TEXTURE_MAX_LOD, CreateInfo_.MaximumLod);
			glTextureParameterf(Id_, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
			if (CreateInfo_.Levels > 1)
			{
				glTextureParameterf(Id_, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
				glGenerateTextureMipmap(Id_);
			}
		}
		else
		{
			glGenTextures(1, &Id_);

			glBindTexture(GL_TEXTURE_2D, Id_);
			glTexStorage2D(GL_TEXTURE_2D, CreateInfo_.Levels, GL_RGBA8, width, height);

			for (auto i = 0; i < static_cast<GLint>(CreateInfo_.Levels); i++)
			{
				if (i >= Data.size())
				{
				}
				else
				{
					glTexSubImage2D(GL_TEXTURE_2D, static_cast<int>(i), 0, 0, static_cast<size_t>(width), static_cast<size_t>(height), GL_RGBA, GL_UNSIGNED_BYTE, Data.data());
				}
				width /= 2;
				height /= 2;
			}
			SetTextureFilter(GL_TEXTURE_2D, CreateInfo_);
			if (CreateInfo_.AnisoFilter.Enabled)
			{
				float32_t anisoMax;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoMax);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f > anisoMax ? anisoMax : 4.0f);
			}
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, CreateInfo_.MinimumLod);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, CreateInfo_.MaximumLod);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
			if (CreateInfo_.Levels > 1)
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}

		IDMap_.insert(std::make_pair(Id_, Name_));
		Enable();
	}

	void Texture2D::Enable()
	{
		glBindTexture(GL_TEXTURE_2D, Id_);
	}

	void Texture2D::Disable()
	{

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture3D::Texture3D(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo) 
	{
		this->CreateInfo_ = CreateInfo;
		Initialize(Data);
	}

	void Texture3D::Initialize(std::vector<unsigned char*> &Data)
	{
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		auto width = CreateInfo_.Width;
		auto height = CreateInfo_.Height;
		auto depth = CreateInfo_.Depth;

		if (major >= 4 && minor >= 5)
		{
			glCreateTextures(GL_TEXTURE_3D, 1, &Id_);
			glTextureStorage3D(Id_, CreateInfo_.Levels, GetFormat(CreateInfo_.Format), width, height, depth);
			SetTextureFilter(Id_, CreateInfo_);
			glTextureParameterf(Id_, GL_TEXTURE_MIN_LOD, CreateInfo_.MinimumLod);
			glTextureParameterf(Id_, GL_TEXTURE_MAX_LOD, CreateInfo_.MaximumLod);
			glTextureParameterf(Id_, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
			for (auto i = 0; i < static_cast<GLint>(CreateInfo_.Levels); i++)
			{
				if (i >= Data.size())
				{
					glTextureSubImage3D(Id_, i, 0, 0, 0, width, height, depth, GetFormat(CreateInfo_.Format), GL_UNSIGNED_BYTE, nullptr);
				}
				else
				{
					glTextureSubImage3D(Id_, i, 0, 0, 0, width, height, depth, GetFormat(CreateInfo_.Format), GL_UNSIGNED_BYTE, Data[i]);
				}
				width /= 2;
				height /= 2;
				depth /= 2;
			}
		}
		else
		{
			glGenTextures(1, &Id_);
			glBindTexture(GL_TEXTURE_3D, Id_);
			glTexStorage3D(GL_TEXTURE_3D, CreateInfo_.Levels, GL_RGBA8, width, height, depth);
			SetTextureFilter(GL_TEXTURE_3D, CreateInfo_);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_LOD, CreateInfo_.MinimumLod);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAX_LOD, CreateInfo_.MaximumLod);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
			for (auto i = 0; i < static_cast<GLint>(CreateInfo_.Levels); i++)
			{
				if (i >= Data.size())
				{
					glTexSubImage3D(GL_TEXTURE_3D, i, 0, 0, 0, width, height, depth, GetFormat(CreateInfo_.Format), GL_FLOAT, nullptr);
				}
				else
				{
					glTexSubImage3D(GL_TEXTURE_3D, i, 0, 0, 0, width, height, depth, GetFormat(CreateInfo_.Format), GL_UNSIGNED_BYTE, Data[i]);
				}
				width /= 2;
				height /= 2;
				depth /= 2;
			}
		}

		IDMap_.insert(std::make_pair(Id_, Name_));
		Enable();
	}

	void Texture3D::Enable()
	{
		glBindTexture(GL_TEXTURE_3D, Id_);
	}

	void Texture3D::Disable()
	{
		glBindTexture(GL_TEXTURE_3D, 0);
	}

	TextureCubeMap::TextureCubeMap(std::vector<unsigned char*> &Data, TextureCreateInfo CreateInfo)
	{
		this->CreateInfo_ = CreateInfo;
		Initialize(Data);
	}

	void TextureCubeMap::Initialize(std::vector<unsigned char*> &Data)
	{
#ifdef DEBUG
		if (Data.size() != 6) __debugbreak();
#endif
		glGenTextures(1, &Id_);
		const auto width = CreateInfo_.Width;
		const auto height = CreateInfo_.Height;
		Enable();
		SetTextureFilter(GL_TEXTURE_CUBE_MAP, CreateInfo_);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, CreateInfo_.MinimumLod);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, CreateInfo_.MaximumLod);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, CreateInfo_.LodBias);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GetFormat(CreateInfo_.Format), width, height);
		for (auto i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GetFormat(CreateInfo_.Format), width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data[i]);
		}

		IDMap_.insert(std::make_pair(Id_, Name_));
		Disable();
	}

	void TextureCubeMap::Enable()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, Id_);
	}

	void TextureCubeMap::Disable()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

} }