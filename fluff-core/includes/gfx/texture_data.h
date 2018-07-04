#pragma once

#include <vector>
#include <gfx/texture_info.h>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>
#include <common.h>

namespace fluff { namespace gfx {

	class FLUFF_API TextureData
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
		TextureData();

		TextureData(std::string Name, std::string File, int NumChannels);
		TextureData(std::string Name, std::vector<unsigned char> Buffer, gfx::TextureCreateInfo Info);

		std::vector<unsigned char> GetBuffer() const;
		std::string GetName() const;
		gfx::TextureCreateInfo GetCreateInfo() const;
	};

} }