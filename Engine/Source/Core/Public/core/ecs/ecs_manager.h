#pragma once

#include <core/ecs/event_manager.h>
#include <core/ecs/system.h>
#include <core/ecs/entity.h>

namespace fluff
{
	class FLUFF_API ECSManager
	{
		std::shared_ptr<ecs::EntityManager> EntityManager_;
		std::shared_ptr<ecs::EventManager>  EventManager_;
		std::shared_ptr<ecs::SystemManager> SystemManager_;
		
	public:
		ECSManager()
		{
			EventManager_  = std::make_shared<ecs::EventManager>();
			EntityManager_ = std::make_shared<ecs::EntityManager>(EventManager_);
			SystemManager_ = std::make_shared<ecs::SystemManager>(EntityManager_, EventManager_);
		};

		/*
			Gets a pointer to the ECSManager's entity manager

			Returns entity manager
		*/
		std::shared_ptr<ecs::EntityManager> & GetEntityManager() { return EntityManager_; }

		/*
			Gets a pointer to the ECSManager's event manager

			Returns event manager
		*/
		std::shared_ptr<ecs::EventManager> & GetEventManager()  { return EventManager_;  }

		/*
			Gets a pointer to the ECSManager's system manager

			Returns system manager
		*/
		std::shared_ptr<ecs::SystemManager> & GetSystemManager() { return SystemManager_; }
	};
}