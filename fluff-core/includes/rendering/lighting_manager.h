#pragma once

#include <core/ecs/event_manager.h>
#include <core/ecs/system.h>
#include <core/ecs/entity.h>

namespace fluff { namespace render {
	class FLUFF_API LightingManager
	{
		ecs::EntityManager * EntityManager_;
		ecs::EventManager  * EventManager_;
		ecs::SystemManager * SystemManager_;

		static LightingManager Manager_;
		LightingManager()
		{
			EventManager_ = new ecs::EventManager;
			EntityManager_ = new ecs::EntityManager(*EventManager_);
			SystemManager_ = new ecs::SystemManager(*EntityManager_, *EventManager_);
		};
	public:
		/*
		Gets a pointer to the ECSManager's entity manager

		Returns entity manager
		*/
		static ecs::EntityManager * GetEntityManager() { return Manager_.EntityManager_; }

		/*
		Gets a pointer to the ECSManager's event manager

		Returns event manager
		*/
		static ecs::EventManager  * GetEventManager() { return Manager_.EventManager_; }

		/*
		Gets a pointer to the ECSManager's system manager

		Returns system manager
		*/
		static ecs::SystemManager * GetSystemManager() { return Manager_.SystemManager_; }

		/*
		Disposes of the encapsulated managers
		*/
		static void Dispose()
		{
			if (Manager_.EntityManager_)
			{
				Manager_.EntityManager_;
				Manager_.EntityManager_ = nullptr;
			}
			if (Manager_.EntityManager_)
			{
				delete Manager_.EventManager_;
				Manager_.EventManager_ = nullptr;
			}
			if (Manager_.EntityManager_)
			{
				delete Manager_.SystemManager_;
				Manager_.EventManager_ = nullptr;
			}

		}
	};
} }