#include <gfx/sampler.h>
#include <glew.h>

namespace fluff { namespace gfx
{

	void SetFilterData(unsigned int Target, TextureFilter CreateInfo)
	{
		switch (CreateInfo)
		{
		case FILTER_MIN_NEAREST_MAG_NEAREST:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
		case FILTER_MIN_NEAREST_MAG_LINEAR:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
		case FILTER_MIN_LINEAR_MAG_NEAREST:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
		case FILTER_MIN_LINEAR_MAG_LINEAR:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
		case FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_NEAREST:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
		case FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_LINEAR:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
		case FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_NEAREST:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
		case FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_LINEAR:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
		case FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_NEAREST:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
		case FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_LINEAR:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
		case FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_NEAREST:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
		case FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_LINEAR:
		{
			glSamplerParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glSamplerParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
		case FILTER_ANISOTROPIC:
		{
			GLfloat f_largest;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &f_largest);
			glSamplerParameterf(Target, GL_TEXTURE_MAX_ANISOTROPY_EXT, f_largest);
		}
		}
	}
	
	
	Sampler::Sampler(TextureFilter Filter)
	{
		glGenSamplers(1, &Sampler_);
		SetFilterData(Sampler_, Filter);
	}

	Sampler::~Sampler()
	{
		if (Sampler_ != 0)
		{
			Release();
		}
	}

	void Sampler::Bind(unsigned int Location) const
	{
		glBindSampler(Location, Sampler_);
	}

	void Sampler::Unbind(unsigned int Location) const
	{
		glBindSampler(Location, 0);
	}

	void Sampler::Release()
	{
		glDeleteSamplers(1, &Sampler_);
		Sampler_ = 0;
	}

} }