#pragma once

#include <vector>
#include <gfx/texture_info.h>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>
#include <common.h>

namespace fluff { namespace gfx {

	class TextureData
	{
		std::vector<unsigned char> Buffer_;
		std::string Name_;
		gfx::TextureCreateInfo CreateInfo_;
		
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(Name_, CreateInfo_, Buffer_);
		}
	public:
		FLUFF_API TextureData();

		FLUFF_API TextureData(std::string Name, std::string File, int NumChannels);
		FLUFF_API TextureData(std::string Name, std::vector<unsigned char> Buffer, gfx::TextureCreateInfo Info);

		std::vector<unsigned char> FLUFF_API GetBuffer() const;
		std::string FLUFF_API GetName() const;
		gfx::TextureCreateInfo FLUFF_API GetCreateInfo() const;
	};

} }