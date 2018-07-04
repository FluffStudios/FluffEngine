#pragma once

#define ALBEDOMAP		0
#define NORMALMAP		1
#define METALLICMAP		2
#define ROUGHNESSMAP	3
#define AOMAP			4

#include <gfx/pipeline.h>
#include <gfx/texture.h>
#include <rendering/uniform.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <rendering/resource_library.h>
#include <core/assets/pipeline_library.h>

namespace fluff { namespace render {

	using namespace fluff::gfx;

	class Material
	{
		GraphicsPipeline * PipelineHandle_;
		std::vector<std::shared_ptr<Uniform>> Uniforms_;
		std::vector<std::shared_ptr<TextureUniform>> Textures_;
		Uniform * ModelUniform_;
		bool Transparent_ = false;

		friend class cereal::access;

		template <class Archive>
		void save(Archive & ar) const
		{
			ar(Uniforms_, Textures_, PipelineHandle_->GetName());
		}

		template <class Archive>
		void load(Archive & ar)
		{
			std::string pipeline;
			ar(Uniforms_, Textures_, pipeline);
			PipelineHandle_ = PipelineLibrary::Get(pipeline);
			for (auto uni : Uniforms_)
			{
				uni->GetLocation(this->PipelineHandle_->GetShader()->GetHandle());
			}

			for (auto tex : Textures_)
			{
				tex->GetLocation(this->PipelineHandle_->GetShader()->GetHandle());
			}
			ModelUniform_ = new Uniform(MAT4, PipelineHandle_->GetShader()->GetHandle(), "modl");
		}
	public:
		std::string Name;

		FLUFF_API Material() { }

		/*
			Creates material from a graphics pipeline

			Pipeline -  Pipeline to create from
		*/
		explicit FLUFF_API Material(GraphicsPipeline * Pipeline);
		
		/*
			Destructor
		*/
		FLUFF_API ~Material();

		/*
			Checks if two materials use the same pipeline and shader

			Returns if materials are equivalent
		*/
		bool FLUFF_API operator ==(const Material& Other) const;

		/*
			Checks if two materials do not use the same piepeline or shader

			Returns if materials are not equivalent
		*/
		bool FLUFF_API operator !=(const Material& Other) const;

		/*
			Gets a pointer to the encapsulated shader
			
			Returns pointer to shader
		 */
		Shader FLUFF_API * GetShader() const;

		/*
			Gets a pointer to the encapsulated pipeline
			
			Returns pointer to pipeline
		 */
		GraphicsPipeline FLUFF_API * GetPipeline() const;

		/*
			Updates the uniform values to the shader
		 */
		void FLUFF_API Update() const;

		/*
			Binds textures for usage
		*/
		void FLUFF_API BindTextures() const;

		/*
			Creates a new texture uniform

			Type - type of uniform
			UniformName - name of uniform in shader
			InitialValue - beginning value of uniform
			Returns a pointer to the uniform just created
		*/
		std::shared_ptr<Uniform> FLUFF_API & CreateUniform(UniformType Type, char * UniformName, void * InitialValue = nullptr);

		/*
			Creates a new texture uniform

			Type - Type of texture
			Tex - Pointer to texture object
			BindLocation - Place to bind in the shader
			Name - Name of uniform
		*/
		std::shared_ptr<TextureUniform> FLUFF_API & CreateTextureUniform(TextureType Type, Texture * Tex, uint32_t BindLocation, char * Name);

		/*
			Creates a new texture uniform

			Type - Type of texture
			Tex - Pointer to texture ID
			BindLocation - Place to bind in the shader
			Name - Name of uniform
		*/
		std::shared_ptr<TextureUniform> FLUFF_API & CreateTextureUniform(TextureType Type, uint32_t * Tex, uint32_t BindLocation, char * Name);

		/*
			Gets a pointer to all of the texture uniforms

			Returns texture uniforms
		*/
		std::vector<TextureUniform*> FLUFF_API GetTextures() const;

		inline std::shared_ptr<Uniform> & GetUniform(const std::string & Name)
		{
			for (auto i : Uniforms_)
				if (strcmp(i->GetName(), Name.c_str()) == 0)
				{
					return i;
				}
			return std::shared_ptr<Uniform>();
		}

		inline std::shared_ptr<TextureUniform> & GetTextureUniform(const std::string & Name)
		{
			for (auto i : Textures_)
				if (i->Name_ == Name)
				{
					return i;
				}
			return std::shared_ptr<TextureUniform>();
		}

		/*
			Sets the model matrix of the shader

			Modl - Pointer to model matrix data
		*/
		void FLUFF_API SetModelMatrix(void * Modl) const;

		/*
			Sets if the materials is transparent

			Transparent - is transparent
		*/
		inline void SetTransparent(const bool Transparent)
		{
			Transparent_ = Transparent;
		}

		/*
			Gets if the material is transparent

			Returns if transparent
		*/
		inline bool IsTransparent() const
		{
			return Transparent_;
		}
	};

} }