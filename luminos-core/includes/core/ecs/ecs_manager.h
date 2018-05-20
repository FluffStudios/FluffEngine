#pragma once

#include <core/ecs/event_manager.h>
#include <core/ecs/system.h>
#include <core/ecs/entity.h>

namespace luminos
{
	class LUMINOS_API ECSManager
	{
		ecs::EntityManager * EntityManager_;
		ecs::EventManager  * EventManager_;
		ecs::SystemManager * SystemManager_;
		
	public:
		ECSManager()
		{
			EventManager_ = new ecs::EventManager;
			EntityManager_ = new ecs::EntityManager(*EventManager_);
			SystemManager_ = new ecs::SystemManager(*EntityManager_, *EventManager_);
		};

		/*
			Gets a pointer to the ECSManager's entity manager

			Returns entity manager
		*/
		ecs::EntityManager * GetEntityManager() { return EntityManager_; }

		/*
			Gets a pointer to the ECSManager's event manager

			Returns event manager
		*/
		ecs::EventManager  * GetEventManager()  { return EventManager_;  }

		/*
			Gets a pointer to the ECSManager's system manager

			Returns system manager
		*/
		ecs::SystemManager * GetSystemManager() { return SystemManager_; }
	};
}