#include <rendering/uniform.h>

namespace luminos { namespace render {

	Uniform::Uniform(UniformType Type, uint32_t ShaderHandle, char * UniformName, void * InitialValue)
	{
		Type_ = Type;
		Size_ = GetSize();
		Name_ = new char[strlen(UniformName) + 1];
		strcpy_s(Name_, strlen(UniformName) + 1, UniformName);
		Name_[strlen(UniformName)] = '\0';
		Location_ = glGetUniformLocation(ShaderHandle, Name_);
		Shader_ = ShaderHandle;
		Value_ = malloc(Size_);
		if (InitialValue) memcpy(Value_, InitialValue, Size_);
	}

	Uniform::~Uniform()
	{
		free(Value_);
		delete[] Name_;
	}

	UniformType Uniform::GetType() const
	{
		return Type_;
	}

	const char * const Uniform::GetName() const
	{
		return Name_;
	}

	void * Uniform::GetValue() const
	{
		return Value_;
	}

	int32_t Uniform::GetLocation() const
	{
		return Location_;
	}

	uint32_t Uniform::GetShaderHandle() const
	{
		return Shader_;
	}

	void Uniform::SetValue(const void * Value) const
	{
		LUMINOS_ASSERT(Value != nullptr)
		memcpy(Value_, Value, Size_);
	}

	size_t Uniform::GetSize() const
	{
		switch (Type_)
		{
			case FLOAT:  return sizeof(float);
			case INT:    return sizeof(int32_t);
			case UINT:   return sizeof(uint32_t);
			case VEC2:   return sizeof(float) * 2;
			case VEC3:   return sizeof(float) * 3;
			case VEC4:   return sizeof(float) * 4;
			case IVEC2:  return sizeof(int32_t) * 2;
			case IVEC3:  return sizeof(int32_t) * 3;
			case IVEC4:  return sizeof(int32_t) * 4;
			case UIVEC2: return sizeof(uint32_t) * 2;
			case UIVEC3: return sizeof(uint32_t) * 3;
			case UIVEC4: return sizeof(uint32_t) * 4;
			case MAT2:   return sizeof(float) * 2 * 2;
			case MAT3:   return sizeof(float) * 3 * 3;
			case MAT4:	 return sizeof(float) * 4 * 4;
			default:
			{
				LUMINOS_ERROR_FUNC("INVALID ENUM - UNIFORMTYPE");
				LUMINOS_ASSERT();
			}
		}
	}

	TextureUniform::TextureUniform(TextureType Type, uint32_t ShaderHandle, uint32_t TextureHandle, int32_t BindLocation, char * Name)
	{
		int32_t max_loc;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_loc);
		LUMINOS_ASSERT(max_loc > BindLocation && BindLocation >= 0);
		Type_ = Type;
		ShaderHandle_ = ShaderHandle;
		Location_ = glGetUniformLocation(ShaderHandle_, Name);
		Name_ = Name;
		TextureHandle_ = new uint32_t;
		*TextureHandle_ = TextureHandle;
		NeedsCleanUp_ = true;
		BindLocation_ = BindLocation;
	}

	TextureUniform::TextureUniform(TextureType Type, uint32_t ShaderHandle, uint32_t * TextureHandle, int32_t BindLocation, char * Name)
	{
		int32_t max_loc;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_loc);
		LUMINOS_ASSERT(max_loc > BindLocation && BindLocation >= 0);
		Type_ = Type;
		ShaderHandle_ = ShaderHandle;
		Location_ = glGetUniformLocation(ShaderHandle_, Name);
		Name_ = Name;
		TextureHandle_ = TextureHandle;
		BindLocation_ = BindLocation;
	}

	TextureUniform::~TextureUniform()
	{
		if (NeedsCleanUp_) delete TextureHandle_;
	}

	void TextureUniform::Upload() const
	{
		glUniform1i(Location_, BindLocation_);
		glActiveTexture(GL_TEXTURE0 + BindLocation_);
		glBindTexture(GL_TEXTURE_2D, *TextureHandle_);
	}

} }