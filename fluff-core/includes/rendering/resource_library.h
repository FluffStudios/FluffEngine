#pragma once

#include <unordered_map>
#include <string>
#include <core/ecs/ecs_manager.h>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

#include <debug.h>

namespace fluff { namespace render {

	enum AssetType : uint32_t
	{
		LIBRARY_MESH,
		LIBRARY_MATERIAL,
		LIBRARY_PIPELINE,
		LIBRARY_TEXTURE
	};

	struct AssetLocationLibrary
	{
		std::unordered_map<std::string, AssetType> AssetTypes;
	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive & ar)
		{
			ar(AssetTypes);
		}
	};

	class ResourceLibrary
	{
		std::unordered_map<std::string, void *> Textures_;
		std::unordered_map<std::string, void *> GPipelines_;
		std::unordered_map<std::string, void *> Materials_;
		std::shared_ptr<ECSManager> Manager_;

		static ResourceLibrary Instance_;
	public:
		static ResourceLibrary & GetInstance();
		void FLUFF_API AddPipeline(std::string Name, void * Pipeline);
		void FLUFF_API AddTexture(std::string Name, void * Texture);
		void FLUFF_API * GetPipeline(std::string Name);
		void FLUFF_API * GetTexture(std::string);
		AssetLocationLibrary LocationLibrary;
	};

} }