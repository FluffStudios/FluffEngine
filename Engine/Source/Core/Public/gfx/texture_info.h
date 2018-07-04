#pragma once

#include <common.h>

#include <cereal/access.hpp>

namespace fluff { namespace gfx
{
	enum FLUFF_API TextureFilter : uint32_t
	{
		FILTER_MIN_NEAREST_MAG_NEAREST,
		FILTER_MIN_NEAREST_MAG_LINEAR,
		FILTER_MIN_LINEAR_MAG_NEAREST,
		FILTER_MIN_LINEAR_MAG_LINEAR,
		FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_NEAREST,
		FILTER_MIN_NEAREST_MIPMAP_NEAREST_MAG_LINEAR,
		FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_NEAREST,
		FILTER_MIN_LINEAR_MIPMAP_NEAREST_MAG_LINEAR,
		FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_NEAREST,
		FILTER_MIN_NEAREST_MIPMAP_LINEAR_MAG_LINEAR,
		FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_NEAREST,
		FILTER_MIN_LINEAR_MIPMAP_LINEAR_MAG_LINEAR,
		FILTER_ANISOTROPIC
	};

	enum FLUFF_API TextureFormat : uint32_t
	{
		RGBA32,
		RGBA16,
		RGBA8,
		RGBA,
		RGB32,
		RGB16,
		RGB8,
		RGB
	};

	struct FLUFF_API AnisotropicFilter
	{
		bool Enabled = false;
		float Min = 0;
		float Max = 0;
	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(Enabled, Min, Max);
		}
	};

	struct FLUFF_API TextureCreateInfo
	{
		TextureFilter Filter;
		TextureFormat Format;
		AnisotropicFilter AnisoFilter;
		int Width;
		int Height;
		int Depth;
		unsigned int Levels = 0;
		float LodBias = 0;
		float MinimumLod = -1000;
		float MaximumLod = 1000;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive & ar) const
		{
			ar(Filter, Format, AnisoFilter, Width, Height, Depth, Levels, LodBias, MinimumLod, MaximumLod);
		}

		template <class Archive>
		void load(Archive & ar)
		{
			uint32_t filter;
			uint32_t format;
			ar(filter, format, AnisoFilter, Width, Height, Depth, Levels, LodBias, MinimumLod, MaximumLod);
			Filter = static_cast<TextureFilter>(filter);
			Format = static_cast<TextureFormat>(format);
		}
	};

} }