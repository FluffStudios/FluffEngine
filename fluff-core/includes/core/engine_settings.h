#pragma once

#include <common.h>
#include <string>
#include <cereal/external/rapidjson/document.h>

namespace luminos
{
	struct LUMINOS_API WindowSettings
	{
		uint32_t Width = 1920;
		uint32_t Height = 1080;
		uint32_t WindowID = 0;
		std::string Name = "LUMINOS_ENGINE_APPLICATION";
		bool VSync = false;
		bool Fullscreen = false;

		rapidjson::Value Save(rapidjson::MemoryPoolAllocator<> & Allocator) const;
		void Load(const rapidjson::Value & Object, rapidjson::MemoryPoolAllocator<> & Allocator);
	};

	enum LUMINOS_API Quality : uint32_t
	{
		LOW = 0,
		MEDIUM = 1,
		HIGH = 2,
		ULTRA = 3
	};

	enum LUMINOS_API AA : uint32_t
	{
		NONE,
		MSAA
	};

	struct LUMINOS_API GFXSettings
	{
		uint32_t Samples = 1;
		Quality TextureQuality = MEDIUM;
		Quality ShadowQuality = MEDIUM;
		Quality LightingQuality = MEDIUM;
		AA AntiAliasing = NONE;
		float32_t MaximumRenderDistance;

		rapidjson::Value Save(rapidjson::MemoryPoolAllocator<> & Allocator) const;
		void Load(const rapidjson::Value & Object, rapidjson::MemoryPoolAllocator<> & Allocator);
	};

	struct LUMINOS_API EngineSettings
	{
		static EngineSettings CurrentEngineSettings;

		WindowSettings S_Window;
		GFXSettings S_Graphics;

		void ToFile(std::string FileName) const;
		void ReadFile(std::string FileName);
	};
}