#pragma once

#include <unordered_map>
#include <string>
#include <core/ecs/ecs_manager.h>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>

#include <debug.h>

namespace fluff { namespace render {

	enum FLUFF_API AssetType : uint32_t
	{
		LIBRARY_MESH,
		LIBRARY_MATERIAL,
		LIBRARY_PIPELINE,
		LIBRARY_TEXTURE
	};

	struct FLUFF_API AssetLocationLibrary
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

	class FLUFF_API ResourceLibrary
	{
		std::unordered_map<std::string, void *> Textures_;
		std::unordered_map<std::string, void *> GPipelines_;
		std::unordered_map<std::string, void *> Materials_;
		std::shared_ptr<ECSManager> Manager_;

		static ResourceLibrary Instance_;
	public:
		static ResourceLibrary & GetInstance();
		void AddPipeline(std::string Name, void * Pipeline);
		void AddTexture(std::string Name, void * Texture);
		void * GetPipeline(std::string Name);
		void * GetTexture(std::string);
		AssetLocationLibrary LocationLibrary;
	};

} }