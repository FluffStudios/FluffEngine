#include <core/engine_settings.h>

#include <cereal/external/rapidjson/filewritestream.h>
#include <cereal/external/rapidjson/prettywriter.h>

#include <cereal/external/rapidjson/filereadstream.h>

#include <iostream>

namespace fluff
{
	EngineSettings EngineSettings::CurrentEngineSettings;

	rapidjson::Value WindowSettings::Save(rapidjson::MemoryPoolAllocator<> & Allocator) const
	{
		using namespace rapidjson;

		Value val;
		val.SetObject();

		Value width;
		width.SetUint(Width);

		Value height;
		height.SetUint(Height);

		Value win_id;
		win_id.SetUint(WindowID);

		Value name;
		name.SetString(Name.c_str(), Name.length(), Allocator);

		Value vsync;
		vsync.SetBool(VSync);

		Value fullscreen;
		fullscreen.SetBool(Fullscreen);

		val.AddMember("width", width, Allocator);
		val.AddMember("height", height, Allocator);
		val.AddMember("window_id", win_id, Allocator);
		val.AddMember("window_name", name, Allocator);
		val.AddMember("vsync", vsync, Allocator);
		val.AddMember("fullscreen", fullscreen, Allocator);

		return val;
	}

	void WindowSettings::Load(const rapidjson::Value & Object, rapidjson::MemoryPoolAllocator<>& Allocator)
	{
		Width = Object["width"].GetUint();
		Height = Object["height"].GetUint();
		WindowID = Object["window_id"].GetUint();
		Name = Object["window_name"].GetString();
		VSync = Object["vsync"].GetBool();
		Fullscreen = Object["fullscreen"].GetBool();
	}

	rapidjson::Value GFXSettings::Save(rapidjson::MemoryPoolAllocator<>& Allocator) const
	{
		using namespace rapidjson;

		Value val;
		val.SetObject();

		Value samples;
		samples.SetUint(Samples);

		Value tq;
		tq.SetUint(TextureQuality);

		Value sq;
		sq.SetUint(ShadowQuality);

		Value lq;
		lq.SetUint(LightingQuality);

		Value aa;
		aa.SetUint(AntiAliasing);

		Value mrd;
		mrd.SetFloat(MaximumRenderDistance);

		val.AddMember("samples", samples, Allocator);
		val.AddMember("texture_quality", tq, Allocator);
		val.AddMember("shadows_quality", sq, Allocator);
		val.AddMember("lighting_quality", lq, Allocator);
		val.AddMember("antialiasing", aa, Allocator);
		val.AddMember("max_render_distance", mrd, Allocator);

		return val;
	}

	void GFXSettings::Load(const rapidjson::Value & Object, rapidjson::MemoryPoolAllocator<>& Allocator)
	{
		Samples = Object["samples"].GetUint();
		TextureQuality = static_cast<Quality>(Object["texture_quality"].GetUint());
		ShadowQuality = static_cast<Quality>(Object["shadows_quality"].GetUint());
		LightingQuality = static_cast<Quality>(Object["lighting_quality"].GetUint());
		AntiAliasing = static_cast<AA>(Object["antialiasing"].GetUint());
		MaximumRenderDistance = Object["max_render_distance"].GetFloat();
	}

	void EngineSettings::ToFile(std::string FileName) const
	{
		using namespace rapidjson;

		Document doc;
		doc.SetObject();
		Value root;
		root.SetObject();
		root.AddMember(StringRef("window_settings", strlen("window_settings")), S_Window.Save(doc.GetAllocator()), doc.GetAllocator());
		root.AddMember(StringRef("graphics_settings", strlen("graphics_settings")), S_Graphics.Save(doc.GetAllocator()), doc.GetAllocator());
		doc.AddMember("settings", root, doc.GetAllocator());

		FILE * fp;
		fopen_s(&fp, FileName.c_str(), "wt");
		char output_buffer[65536];
		FileWriteStream os(fp, output_buffer, sizeof(output_buffer));
		PrettyWriter<FileWriteStream> writer(os);
		doc.Accept(writer);;
		fclose(fp);
	}

	void EngineSettings::ReadFile(std::string FileName)
	{
		using namespace rapidjson;

		Document doc;
		
		FILE * fp;
		fopen_s(&fp, FileName.c_str(), "rt");

		char input_buffer[65536];
		FileReadStream is(fp, input_buffer, sizeof(input_buffer));

		doc.ParseStream(is);
		fclose(fp);

		this->S_Window.Load(doc["settings"]["window_settings"], doc.GetAllocator());
		this->S_Graphics.Load(doc["settings"]["graphics_settings"], doc.GetAllocator());
	}
}