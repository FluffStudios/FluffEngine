#pragma once

#include <core/ecs/event_manager.h>
#include <core/ecs/system.h>
#include <core/ecs/entity.h>

namespace fluff
{
	class ECSManager
	{
		ecs::EntityManager * EntityManager_;
		ecs::EventManager  * EventManager_;
		ecs::SystemManager * SystemManager_;
		
	public:
		ECSManager()
		{
			EventManager_  = new ecs::EventManager;
			EntityManager_ = new ecs::EntityManager(*EventManager_);
			SystemManager_ = new ecs::SystemManager(*EntityManager_, *EventManager_);
		};

		/*
			Gets a pointer to the ECSManager's entity manager

			Returns entity manager
		*/
		ecs::EntityManager FLUFF_API * GetEntityManager() { return EntityManager_; }

		/*
			Gets a pointer to the ECSManager's event manager

			Returns event manager
		*/
		ecs::EventManager  FLUFF_API * GetEventManager()  { return EventManager_;  }

		/*
			Gets a pointer to the ECSManager's system manager

			Returns system manager
		*/
		ecs::SystemManager FLUFF_API * GetSystemManager() { return SystemManager_; }
	};
}