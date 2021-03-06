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
		ScreenshotSystem(std::shared_ptr<ECSManager> & Manager);

		/*
			Configures Screenshot System to listen for
			screenshot events
		*/
		void Configure(std::shared_ptr<ecs::EventManager> & Events) override;

		/*
			Ticks through events

			Entitites - Entity Manager associated with screenshot system
			Events - Event Manager associated with screenshot system
			TimeDelta - time since last update
		*/
		void Update(std::shared_ptr<ecs::EntityManager> & Entities, std::shared_ptr<ecs::EventManager> & Events, double TimeDelta) override;

		/*
			Creates screenshot

			Msg - Screenshot Event trigger
		*/
		void Receive(const ScreenshotEvent & Msg);
	};

} }