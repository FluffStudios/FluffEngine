#pragma once

#include <common.h>
#include <vector>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <gfx/texture.h>
#include <core/assets/texture_library.h>

namespace fluff { namespace render {

	enum UniformType : uint32_t
	{
		FLOAT,
		INT,
		UINT,
		VEC2,
		VEC3,
		VEC4, 
		IVEC2,
		IVEC3,
		IVEC4,
		UIVEC2,
		UIVEC3,
		UIVEC4,
		MAT2,
		MAT3,
		MAT4
	};

	class FLUFF_API Uniform
	{
	public:
		int32_t Location_;
		uint32_t Shader_;
		char *  Name_;
		void *  Value_;
		UniformType Type_;

		size_t Size_;
	private:
		friend class cereal::access;
		
		template <class Archive>
		void save(Archive & ar) const
		{
			std::vector<char> Data;
			Data.resize(GetSize());
			memcpy(Data.data(), Value_, GetSize());

			ar(std::string(Name_), Data, Type_);
		}

		template <class Archive>
		void load(Archive & ar)
		{
			std::vector<char> Data;
			std::string name;

			uint32_t Type;

			ar(name, Data, Type);

			Type_ = static_cast<UniformType>(Type);

			Value_ = malloc(GetSize());
			memcpy(Value_, Data.data(), GetSize());

			Name_ = new char[name.length() + 1];
			strcpy_s(Name_, name.length() + 1, name.c_str());
			Name_[name.length()] = '\0';
		}
	public:
		Uniform() { }

		/*
			Creates new uniform

			Type - type of uniform
			ShaderHandle - shader ID
			UniformName - Name of uniform
			InitialValue - inital value in shader
		*/
		Uniform(UniformType Type, uint32_t ShaderHandle, char * UniformName, void * InitialValue = nullptr);

		/*
			Destructor
		*/
		~Uniform();

		/*
			Gets the type of the uniform

			Returns uniform type
		*/
		UniformType GetType() const;
		
		/*
			Gets the name of the uniform

			Returns uniform name
		*/
		const char * const GetName() const;

		/*
			Gets uniform value

			Returns uniform value
		*/
		void * GetValue() const;

		/*
			Gets the location of the uniform

			Returns uniform location
		*/
		int32_t GetLocation() const;

		/*
			Gets the shader handle

			Returns shader handle
		*/
		uint32_t GetShaderHandle() const;

		/*
			Sets the uniform's value

			Value - new value
		*/
		void SetValue(const void * Value) const;

		/*
			Gets value of uniform

			Return pointer to value
		*/
		template<typename T>
		T* GetValue() const;

		void GetLocation(uint32_t ShaderID);
	private:
		size_t GetSize() const;
	};

	enum TextureType
	{
		TEXTURE2D = 0x0DE1,
		TEXTURE3D = 0x806F,
		TEXTURECUBEMAP = 0x8513
	};

	class TextureUniform
	{
	public:
		uint32_t * TextureHandle_;
		uint32_t ShaderHandle_;
		uint32_t Location_;
		int32_t BindLocation_;
		std::string Name_;
		TextureType Type_;
		bool NeedsCleanUp_ = false;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive & ar) const
		{
			ar(BindLocation_, Name_, std::string(gfx::Texture::GetName(*TextureHandle_)));
		}

		template <class Archive>
		void load(Archive & ar)
		{
			std::string tex_name;
			ar(BindLocation_, Name_, tex_name);
			auto tex = TextureLibrary::Get(tex_name);
			TextureHandle_ = &(tex->GetHandle());
		}
	public:
		TextureUniform() { }

		/*
			Creates new texture uniform

			Type - type of texture
			ShaderHandle - ID of shader
			TextureHandle - ID of handle
			BindLocation - Location in shader to bind to
			Name - name of uniform
		*/
		TextureUniform(TextureType Type, uint32_t ShaderHandle, uint32_t TextureHandle, int32_t BindLocation, char * Name);
		
		/*
			Creates new texture uniform

			Type - type of texture
			ShaderHandle - ID of shader
			TextureHandle - Pointer to ID of handle
			BindLocation - Location in shader to bind to
			Name - name of uniform
		*/
		TextureUniform(TextureType Type, uint32_t ShaderHandle, uint32_t * TextureHandle, int32_t BindLocation, char * Name);
		
		/*
			Destructor
		*/
		~TextureUniform();

		/*
			Uploads texture to shader
		*/
		void Upload() const;

		void GetLocation(uint32_t ShaderID);
	};
}
	template<typename T>
	inline T * render::Uniform::GetValue() const
	{
		return static_cast<T*>(GetValue());
	}
}