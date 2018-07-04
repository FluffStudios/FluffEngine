#pragma once

#include <common.h>
#include <string>
#include <cereal/external/rapidjson/document.h>

namespace fluff {

	struct WindowSettings
	{
		uint32_t Width = 1920;
		uint32_t Height = 1080;
		uint32_t WindowID = 0;
		std::string Name = "LUMINOS_ENGINE_APPLICATION";
		bool VSync = false;
		bool Fullscreen = true;

		rapidjson::Value FLUFF_API Save(rapidjson::MemoryPoolAllocator<> & Allocator) const;
		void FLUFF_API Load(const rapidjson::Value & Object, rapidjson::MemoryPoolAllocator<> & Allocator);
	};

	enum Quality : uint32_t
	{
		LOW = 0,
		MEDIUM = 1,
		HIGH = 2,
		ULTRA = 3
	};

	enum AA : uint32_t
	{
		NONE,
		MSAA
	};

	struct GFXSettings
	{
		uint32_t Samples = 1;
		Quality TextureQuality = MEDIUM;
		Quality ShadowQuality = MEDIUM;
		Quality LightingQuality = MEDIUM;
		AA AntiAliasing = NONE;
		float32_t MaximumRenderDistance;

		rapidjson::Value FLUFF_API Save(rapidjson::MemoryPoolAllocator<> & Allocator) const;
		void FLUFF_API Load(const rapidjson::Value & Object, rapidjson::MemoryPoolAllocator<> & Allocator);
	};

	struct EngineSettings
	{
		static EngineSettings CurrentEngineSettings;

		WindowSettings S_Window;
		GFXSettings S_Graphics;

		void FLUFF_API ToFile(std::string FileName) const;
		void FLUFF_API ReadFile(std::string FileName);
	};
}