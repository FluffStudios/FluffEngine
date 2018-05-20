#include <rendering/material.h>
#include "allocator.h"

namespace luminos { namespace render
{
	
	Material::Material(GraphicsPipeline * Pipeline)
	{
		this->PipelineHandle_ = Pipeline;
		ModelUniform_ = new Uniform(MAT4, GetShader()->GetHandle(), "modl", nullptr);
	}

	Material::~Material()
	{
		Uniforms_.clear();
		Textures_.clear();
	}

	bool Material::operator==(const Material& Other) const
	{
		return PipelineHandle_->GetShader() == Other.PipelineHandle_->GetShader();
	}

	bool Material::operator!=(const Material& Other) const
	{
		return PipelineHandle_->GetShader() != Other.PipelineHandle_->GetShader();
	}

	Shader* Material::GetShader() const
	{
		return PipelineHandle_->GetShader();
	}

	GraphicsPipeline* Material::GetPipeline() const
	{
		return PipelineHandle_;
	}

	void Material::Update() const
	{
		for (auto & uni : Uniforms_)
		{
			switch (uni->GetType())
			{
			case FLOAT:
			{
				glUniform1fv(uni->GetLocation(), 1, (GLfloat*)uni->GetValue());
				break;
			}
			case INT:
			{
				glUniform1iv(uni->GetLocation(), 1, (GLint*)uni->GetValue());
				break;
			}
			case UINT:
			{
				glUniform1uiv(uni->GetLocation(), 1, (GLuint*)uni->GetValue());
				break;
			}
			case VEC2:
			{
				glUniform2fv(uni->GetLocation(), 1, (GLfloat*)uni->GetValue());
				break;
			}
			case VEC3:
			{
				glUniform3fv(uni->GetLocation(), 1, (GLfloat*)uni->GetValue());
				break;
			}
			case VEC4:
			{
				glUniform4fv(uni->GetLocation(), 1, (GLfloat*)uni->GetValue());
				break;
			}
			case IVEC2:
			{
				glUniform2iv(uni->GetLocation(), 1, (GLint*)uni->GetValue());
				break;
			}
			case IVEC3:
			{
				glUniform3iv(uni->GetLocation(), 1, (GLint*)uni->GetValue());
				break;
			}
			case IVEC4:
			{
				glUniform4iv(uni->GetLocation(), 1, (GLint*)uni->GetValue());
				break;
			}
			case UIVEC2:
			{
				glUniform2uiv(uni->GetLocation(), 1, (GLuint*)uni->GetValue());
				break;
			}
			case UIVEC3:
			{
				glUniform3uiv(uni->GetLocation(), 1, (GLuint*)uni->GetValue());
				break;
			}
			case UIVEC4:
			{
				glUniform4uiv(uni->GetLocation(), 1, (GLuint*)uni->GetValue());
				break;
			}
			case MAT2:
			{
				glUniformMatrix2fv(uni->GetLocation(), 1, GL_FALSE, (GLfloat*)uni->GetValue());
				break;
			}
			case MAT3:
			{
				glUniformMatrix3fv(uni->GetLocation(), 1, GL_FALSE, (GLfloat*)uni->GetValue());
				break;
			}
			case MAT4:
			{
				glUniformMatrix4fv(uni->GetLocation(), 1, GL_FALSE, (GLfloat*)uni->GetValue());
				break;
			}
			}
		}
		BindTextures();
	}

	void Material::BindTextures() const
	{
		for (auto & tex : Textures_) tex->Upload();
	}

	std::shared_ptr<Uniform> & Material::CreateUniform(UniformType Type, char * UniformName, void * InitialValue)
	{
		auto uni = std::shared_ptr<Uniform>(new Uniform(Type, GetShader()->GetHandle(), UniformName, InitialValue));
		Uniforms_.push_back(uni);
		return Uniforms_.back();
	}

	std::shared_ptr<TextureUniform> & Material::CreateTextureUniform(TextureType Type, Texture * Tex, uint32_t BindLocation, char * Name)
	{
		auto tex = std::shared_ptr<TextureUniform>(new TextureUniform(Type, GetShader()->GetHandle(), Tex->GetHandle(), BindLocation, Name));
		Textures_.push_back(tex);
		return Textures_.back();
	}

	std::shared_ptr<TextureUniform> & Material::CreateTextureUniform(TextureType Type, uint32_t * Tex, uint32_t BindLocation, char * Name)
	{
		auto tex = std::shared_ptr<TextureUniform>(new TextureUniform(Type, GetShader()->GetHandle(), Tex, BindLocation, Name));
		Textures_.push_back(tex);
		return Textures_.back();
	}

	std::vector<TextureUniform*> Material::GetTextures() const
	{
		std::vector<TextureUniform*> res;
		for (auto & i : Textures_) res.push_back(i.get());
		return res;
	}
} }