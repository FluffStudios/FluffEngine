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
#include <core/debug/debug_message.h>
#include <glew.h>

namespace fluff { namespace render {

	ScreenshotSystem::ScreenshotSystem(std::shared_ptr<ECSManager> & Manager)
		: Manager_(Manager)
	{	}

	void ScreenshotSystem::Configure(std::shared_ptr<ecs::EventManager> & Events)
	{
		Events->SubscribeToEvent<ScreenshotEvent>(*this);
	}

	void ScreenshotSystem::Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double Delta)
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
			debug::DebugMessage(Manager_, debug::DebugErrorType::ILLEGAL_DATA, debug::DebugSeverity::WARN, static_cast<size_t>(__LINE__), std::string(__FILE__), "Cannot get date.");
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
			if (!res) FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::WARN, "Could not create screenshot.");
			free(img);
		}
		else
		{
			FLUFF_LOG(debug::DebugErrorType::ILLEGAL_STATE, debug::DebugSeverity::WARN, "Could not allocate buffer for screenshot.");
		}
	}

} }