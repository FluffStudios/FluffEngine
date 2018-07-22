#pragma once

#include <core/ecs/event_manager.h>
#include <core/ecs/system.h>
#include <core/ecs/entity.h>

namespace fluff { namespace render {
	class FLUFF_API LightingManager
	{
		std::shared_ptr<ecs::EntityManager> EntityManager_;
		std::shared_ptr<ecs::EventManager> EventManager_;
		std::shared_ptr<ecs::SystemManager> SystemManager_;

		static LightingManager Manager_;
		LightingManager()
		{
			EventManager_  = std::make_shared<ecs::EventManager>();
			EntityManager_ = std::make_shared<ecs::EntityManager>(EventManager_);
			SystemManager_ = std::make_shared<ecs::SystemManager>(EntityManager_, EventManager_);
		};
	public:
		/*
		Gets a pointer to the ECSManager's entity manager

		Returns entity manager
		*/
		static std::shared_ptr<ecs::EntityManager> & GetEntityManager() { return Manager_.EntityManager_; }

		/*
		Gets a pointer to the ECSManager's event manager

		Returns event manager
		*/
		static std::shared_ptr<ecs::EventManager> & GetEventManager() { return Manager_.EventManager_; }

		/*
		Gets a pointer to the ECSManager's system manager

		Returns system manager
		*/
		static std::shared_ptr<ecs::SystemManager> & GetSystemManager() { return Manager_.SystemManager_; }

		/*
		Disposes of the encapsulated managers
		*/
		static void Dispose()
		{

		}
	};
} }