#include <rendering/screenshot_system.h>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <stb_image_write.h>

#include <string>
#include <time.h>
#include <ctime>
#include <chrono>
#include <sstream>
#include <core/debug_message.h>
#include <glew.h>

namespace luminos { namespace render {

	ScreenshotSystem::ScreenshotSystem(std::shared_ptr<ECSManager> & Manager)
		: Manager_(Manager)
	{	}

	void ScreenshotSystem::Configure(ecs::EventManager & Events)
	{
		Events.SubscribeToEvent<ScreenshotEvent>(*this);
	}

	void ScreenshotSystem::Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double Delta)
	{

	}

	void ScreenshotSystem::Receive(const ScreenshotEvent & Msg)
	{
		std::stringstream stream;
		stream << "SCREENSHOT_";
		std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char t[26];
		if (ctime_s(t, sizeof(t), &time))
		{
			DebugMessage(Manager_, WARNING, "Could not retrieve time.");
			return;
		}
		std::string time_str(t);
		std::replace(time_str.begin(), time_str.end(), ':', '_');
		std::replace(time_str.begin(), time_str.end(), ' ', '_');
		time_str.erase(time_str.end() - 1, time_str.end());
		stream << time_str << ".bmp";

		char * img = static_cast<char*>(malloc(3 * Msg.Width * Msg.Height * sizeof(char)));
		if (img)
		{
			glReadBuffer(GL_FRONT);
			glReadPixels(0, 0, Msg.Width, Msg.Height, GL_RGB, GL_UNSIGNED_BYTE, img);
			stbi_flip_vertically_on_write(true);
			auto res = stbi_write_bmp(stream.str().c_str(), Msg.Width, Msg.Height, 3, img);
			if (!res) DebugMessage(Manager_, WARNING, "Screenshot could not be created.");
			free(img);
		}
		else
		{
			DebugMessage(Manager_, WARNING, "Could not allocate buffer for screenshot.");
		}
	}

} }