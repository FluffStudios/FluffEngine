#pragma once

#include <rendering/screenshot_event.h>
#include <core/ecs/system.h>
#include <core/ecs/ecs_manager.h>
#include <memory>

namespace fluff { namespace render {

	struct ScreenshotSystem : public ecs::System<ScreenshotSystem>, public ecs::Receiver<ScreenshotSystem>
	{
		std::shared_ptr<ECSManager> & Manager_;
		/*
			Default constructor

			Creates new screenshot system
		*/
		FLUFF_API ScreenshotSystem(std::shared_ptr<ECSManager> & Manager);

		/*
			Configures Screenshot System to listen for
			screenshot events
		*/
		void FLUFF_API Configure(ecs::EventManager & Events);

		/*
			Ticks through events

			Entitites - Entity Manager associated with screenshot system
			Events - Event Manager associated with screenshot system
			TimeDelta - time since last update
		*/
		void FLUFF_API Update(ecs::EntityManager & Entities, ecs::EventManager & Events, double TimeDelta) override;

		/*
			Creates screenshot

			Msg - Screenshot Event trigger
		*/
		void FLUFF_API Receive(const ScreenshotEvent & Msg);
	};

} }